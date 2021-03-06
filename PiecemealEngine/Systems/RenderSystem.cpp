#include "RenderSystem.hpp"

#include "Core/Conductor.hpp"
#include "Components/ComponentTest.hpp"
#include <iostream>

extern Conductor conductor;

void RenderSystem::Init(SDL_Renderer* Arenderer, SDL_Window* Awindow)
{
	renderer = Arenderer;
	window = Awindow;

	SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
	SDL_RenderClear(renderer);
}

void RenderSystem::Update()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
	SDL_RenderClear(renderer);

	for (auto const& entity : involvedEntities) {
		//std::cout << conductor.GetEntitySignature(entity) << std::endl;
		auto& renderable = conductor.GetComponent<Renderable>(entity);
		auto& transform = conductor.GetComponent<Transform>(entity);
		
		SDL_Rect rect;
		rect.x = transform.position.x;
		rect.y = transform.position.y;
		rect.w = transform.scale.x;
		rect.h = transform.scale.y;

		SDL_SetRenderDrawColor(renderer, renderable.color.x, renderable.color.y, renderable.color.z, 255);

		SDL_RenderFillRect(renderer, &rect);

		if (conductor.HasComponent<RigidBody>(entity)) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
}
