#pragma once

#include <SDL2/SDL.h>

namespace gfx {
	class renderer {
	private:
	public:
		const int ppu_x = 10, ppu_y = 10;

		SDL_Renderer* handle;

		renderer(SDL_Renderer*);
		~renderer();

		void drawCircle(int, int, int, int);
		void unpack_rgb(int, Uint8*, Uint8*, Uint8*, Uint8*);
		void to_pixel(int x, int y, int* nx, int* ny);
	};
}