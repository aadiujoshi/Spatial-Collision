#pragma once
#include <SDL2/SDL.h>

namespace gfx {
	class renderer {
	private:
	public:
		SDL_Renderer* handle;

		renderer(SDL_Renderer*);
		~renderer();
	};
}