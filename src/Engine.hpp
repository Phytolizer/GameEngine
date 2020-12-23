#pragma once

#include "Color.hpp"
#include "Error.hpp"
#include "MatrixStack.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <glm/glm.hpp>
#include <string_view>

class Engine
{
  private:
    bool m_active;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    MatrixStack m_matrixStack;

  public:
    Engine(std::string_view title, int windowWidth, int windowHeight);
    ~Engine();
    Engine(const Engine &) = delete;
    Engine(Engine &&);
    Engine &operator=(const Engine &) = delete;
    Engine &operator=(Engine &&);

    void Loop();
    virtual void HandleEvent(const SDL_Event &event);
    virtual void Draw() = 0;

    void Clear(const Color &clearColor);
    void DrawLine(const glm::vec2 &start, const glm::vec2 &end,
                  const Color &color);
    void DrawRectangle(const glm::vec2 &topLeft, const glm::vec2 &bottomRight,
                       const Color &color);

    void DrawCircle(const glm::vec2 &center, float radius, const Color &color);
    void Translate(const glm::vec2 &where);
    void Rotate(float theta);
    void Scale(const glm::vec2 &scale);
    void Scale(float scale);
};
