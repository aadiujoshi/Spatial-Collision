#pragma once

#include <SDL2/SDL.h>
#include "renderer.h"

namespace gfx {
	class window {
	private:
	public:
		SDL_Window* handle;

		window();
		~window();

		int getWidth();
		int getHeight();

		void render(gfx::renderer);
	};
}