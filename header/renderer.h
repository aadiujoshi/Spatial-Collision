#pragma once

#include <SDL2/SDL.h>
#include "texture.h"
#include "spatial_partition.h"

namespace gfx {

	inline float ppu_x = 1;
	inline float ppu_y = 1;

	class renderer {
	private:
	public:
		SDL_Renderer* handle;
		gfx::window& window;

		renderer(SDL_Renderer*, gfx::window&);
		~renderer();

		void render_texture(int px, int py, gfx::texture& tex);
		void draw_circle(int, int, int, int);

		static void unpack_rgb(int, Uint8*, Uint8*, Uint8*, Uint8*);
		static void to_pixel(float ux, float uy, int* px, int* py);
		static void to_units(int px, int py, float* ux, float* uy);
	};
}