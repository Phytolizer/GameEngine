#pragma once

#include "Color.hpp"
#include "Error.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

class Engine
{
  private:
    bool m_active;
    SDL_Renderer *m_renderer;

  public:
    Engine();
    ~Engine();
    Engine(const Engine &) = delete;
    Engine(Engine &&);
    Engine &operator=(const Engine &) = delete;
    Engine &operator=(Engine &&);

    void Loop();
    virtual void HandleEvent(const SDL_Event &event);
    virtual void Draw() = 0;

    void Clear(const Color &clearColor);
};
