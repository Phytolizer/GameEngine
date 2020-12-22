#include "Engine.hpp"
#include <SDL2/SDL.h>
#include <fmt/format.h>

Engine::Engine() : m_active(true)
{
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret != 0)
    {
        throw Error{fmt::format("SDL init failed: {}", SDL_GetError())};
    }
}

Engine::~Engine()
{
    if (m_active)
    {
        SDL_Quit();
    }
}

Engine::Engine(Engine &&other) : m_active(other.m_active)
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
