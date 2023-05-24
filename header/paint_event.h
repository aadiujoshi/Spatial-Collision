#pragma once
#include <SDL2/SDL.h>

namespace event {
	class paint_event {
	private:
	public:
		SDL_Renderer* renderer;

		paint_event(SDL_Renderer);
		~paint_event();
	};
}