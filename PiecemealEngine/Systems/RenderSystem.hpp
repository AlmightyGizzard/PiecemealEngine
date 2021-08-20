#pragma once
#include "Core/System.hpp"
#include "SDL.h"


class RenderSystem : public System {
public:

	SDL_Renderer* renderer;
	SDL_Window* window;

	void Init(SDL_Renderer* Arenderer, SDL_Window* Awindow);

	void Update();
};