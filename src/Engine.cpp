#include "GameEngine/Engine.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <fmt/format.h>

Engine::Engine(std::string_view title, int windowWidth, int windowHeight)
    : m_active(true), m_window(nullptr), m_renderer(nullptr),
      m_randomEngine(std::random_device{}()), m_width(windowWidth),
      m_height(windowHeight), m_timer(1.0f / 60)
{
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret != 0)
    {
        throw Error{fmt::format("SDL init failed: {}", SDL_GetError())};
    }
    m_window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                windowHeight, SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr)
    {
        throw Error{
            fmt::format("Creating SDL window failed: {}", SDL_GetError())};
    }
    m_renderer = SDL_CreateRenderer(
        m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        throw Error{fmt::format(
            "Creating SDL HW-accelerated renderer failed: {}", SDL_GetError())};
    }
}

Engine::~Engine()
{
    if (m_active)
    {
        SDL_Quit();
    }
}

Engine::Engine(Engine &&other)
    : m_active(other.m_active), m_timer(other.m_timer)
{
    other.m_active = false;
}

Engine &Engine::operator=(Engine &&other)
{
    if (&other != this)
    {
        if (m_active)
        {
            SDL_Quit();
            m_active = false;
        }
        else
        {
            m_active = other.m_active;
            m_timer = other.m_timer;
            other.m_active = false;
        }
    }
    return *this;
}

void Engine::Loop()
{
    bool run = true;
    while (run)
    {
        SDL_Event e;
        while ((SDL_PollEvent(&e)))
        {
            if (e.type == SDL_QUIT)
            {
                run = false;
            }
            HandleEvent(e);
        }

        if (m_timer.Tick())
        {
            Update();
        }

        int ret = SDL_RenderClear(m_renderer);
        if (ret != 0)
        {
            throw Error{fmt::format("Clearing SDL renderer failed: {}",
                                    SDL_GetError())};
        }
        Draw();
        SDL_RenderPresent(m_renderer);
    }
}

void Engine::HandleEvent(const SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_WINDOWEVENT:
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            m_width = event.window.data1;
            m_height = event.window.data2;
            break;
        }
        break;
    }
}

void Engine::Clear(const Color &clearColor)
{
    int ret = SDL_SetRenderDrawColor(m_renderer, clearColor.r, clearColor.g,
                                     clearColor.b, clearColor.a);
    if (ret != 0)
    {
        throw Error{fmt::format("Setting SDL renderer draw color failed: {}",
                                SDL_GetError())};
    }
    SDL_RenderClear(m_renderer);
}

void Engine::DrawLine(const glm::vec2 &start, const glm::vec2 &end,
                      const Color &color)
{
    glm::vec4 start4(start, 0.0f, 1.0f);
    glm::vec4 end4(end, 0.0f, 1.0f);

    start4 = m_matrixStack.Top() * start4;
    end4 = m_matrixStack.Top() * end4;

    int ret = aalineRGBA(m_renderer, start4.x, start4.y, end4.x, end4.y,
                         color.r, color.g, color.b, color.a);
    if (ret != 0)
    {
        throw Error{fmt::format("Drawing anti-aliased line failed: {}",
                                SDL_GetError())};
    }
}
void Engine::DrawRectangle(const glm::vec2 &topLeft,
                           const glm::vec2 &bottomRight, const Color &color)
{
    auto tl4 = glm::vec4(topLeft, 0, 1);
    auto br4 = glm::vec4(bottomRight, 0, 1);
    const glm::vec4 bottomLeft = {topLeft.x, bottomRight.y, 0, 1};
    const glm::vec4 topRight = {bottomRight.x, topLeft.y, 0, 1};
    auto tl = m_matrixStack.Top() * tl4;
    auto br = m_matrixStack.Top() * br4;
    auto bl = m_matrixStack.Top() * bottomLeft;
    auto tr = m_matrixStack.Top() * topRight;

    Sint16 xs[] = {static_cast<Sint16>(tl.x), static_cast<Sint16>(tr.x),
                   static_cast<Sint16>(br.x), static_cast<Sint16>(bl.x)};
    Sint16 ys[] = {static_cast<Sint16>(tl.y), static_cast<Sint16>(tr.y),
                   static_cast<Sint16>(br.y), static_cast<Sint16>(bl.y)};

    int ret =
        polygonRGBA(m_renderer, xs, ys, 4, color.r, color.g, color.b, color.a);
    if (ret != 0)
    {
        throw Error{
            fmt::format("Drawing rectangle failed: {}", SDL_GetError())};
    }
}
void Engine::FillRectangle(const glm::vec2 &topLeft,
                           const glm::vec2 &bottomRight, const Color &color)
{
    auto tl4 = glm::vec4(topLeft, 0, 1);
    auto br4 = glm::vec4(bottomRight, 0, 1);
    const glm::vec4 bottomLeft = {topLeft.x, bottomRight.y, 0, 1};
    const glm::vec4 topRight = {bottomRight.x, topLeft.y, 0, 1};
    auto tl = m_matrixStack.Top() * tl4;
    auto br = m_matrixStack.Top() * br4;
    auto bl = m_matrixStack.Top() * bottomLeft;
    auto tr = m_matrixStack.Top() * topRight;

    Sint16 xs[] = {static_cast<Sint16>(tl.x), static_cast<Sint16>(tr.x),
                   static_cast<Sint16>(br.x), static_cast<Sint16>(bl.x)};
    Sint16 ys[] = {static_cast<Sint16>(tl.y), static_cast<Sint16>(tr.y),
                   static_cast<Sint16>(br.y), static_cast<Sint16>(bl.y)};

    int ret = filledPolygonRGBA(m_renderer, xs, ys, 4, color.r, color.g,
                                color.b, color.a);
    if (ret != 0)
    {
        throw Error{
            fmt::format("Drawing filled rectangle failed: {}", SDL_GetError())};
    }
}

void Engine::DrawCircle(const glm::vec2 &center, float radius,
                        const Color &color)
{
    glm::vec4 center4(center, 0.0f, 1.0f);
    center4 = m_matrixStack.Top() * center4;
    int ret = circleRGBA(m_renderer, center4.x, center4.y, radius, color.r,
                         color.g, color.b, color.a);
    if (ret != 0)
    {
        throw Error{fmt::format("Drawing circle failed: {}", SDL_GetError())};
    }
}

void Engine::FillCircle(const glm::vec2 &center, float radius,
                        const Color &color)
{
    glm::vec4 center4(center, 0.0f, 1.0f);
    center4 = m_matrixStack.Top() * center4;
    int ret = filledCircleRGBA(m_renderer, center4.x, center4.y, radius,
                               color.r, color.g, color.b, color.a);
    if (ret != 0)
    {
        throw Error{
            fmt::format("Drawing filled circle failed: {}", SDL_GetError())};
    }
}

void Engine::ResetMatrix()
{
    m_matrixStack.Reset();
}
void Engine::PushMatrix()
{
    m_matrixStack.Push();
}
void Engine::PopMatrix()
{
    m_matrixStack.Pop();
}
void Engine::Translate(const glm::vec2 &where)
{
    m_matrixStack.Translate(where);
}
void Engine::Rotate(float theta)
{
    m_matrixStack.Rotate(theta);
}
void Engine::Scale(const glm::vec2 &scale)
{
    m_matrixStack.Scale(scale);
}
void Engine::Scale(float scale)
{
    Scale({scale, scale});
}
float Engine::Random(float min, float max)
{
    return std::uniform_real_distribution<float>(min, max)(m_randomEngine);
}
int Engine::WindowWidth()
{
    return m_width;
}
int Engine::WindowHeight()
{
    return m_height;
}
float Engine::Map(float x, float min, float max, float newMin, float newMax)
{
    return (x - min) * (newMax - newMin) / (max - min) + newMin;
}
void Engine::SetInterval(float interval)
{
    m_timer = Timer(interval);
}
