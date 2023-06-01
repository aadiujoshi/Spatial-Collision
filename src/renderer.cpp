#include "SDL2/SDL.h"
#include "header_paths.h"
#include <iostream>
#include renderer_h
#include texture_h
#include spatial_partition_h

namespace gfx {
	renderer::renderer(SDL_Renderer* _handle, gfx::window& window) : window(window) {
		handle = _handle;
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		SDL_SetRenderDrawBlendMode(handle, SDL_BLENDMODE_BLEND);
		gfx::ppu_x = window.getWidth() / sp::UNITS_WIDTH;
		gfx::ppu_y = window.getHeight() / sp::UNITS_HEIGHT;

		//std::cout << gfx::ppu_x << "  " << gfx::ppu_y << std::endl;
	};

	renderer::~renderer() {
	};

	void renderer::render_texture(int px, int py, gfx::texture& tex) {
		SDL_Rect pos = { px - tex.width / 2, py - tex.height / 2, tex.width, tex.height };
		//SDL_Rect pos = { px - tex.width / 2, py - tex.height / 2, 4, 4 };

		SDL_RenderCopy(handle, tex.tex_handle, nullptr, &pos);
	}

	void renderer::draw_circle(int x, int y, int pxr, int color) {
		Uint8 r, g, b, a;
		unpack_rgb(color, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(handle, r, g, b, a);

		//my own circle rendering algorithm
		for (int x0 = x - pxr; x0 <= x; x0++) {
			for (int y0 = y - pxr; y0 <= y; y0++) {
				float dist = sqrtf((x0 - x) * (x0 - x) + (y0 - y) * (y0 - y));
				if (dist <= pxr) {
					//quadrant II and III
					SDL_RenderDrawLine(handle, x0, y0, x0, y - (y0 - y));
					//quadrant I and IV
					SDL_RenderDrawLine(handle, x - (x0 - x), y0, x - (x0 - x), y - (y0 - y));
					break;
				}
			}
		}
	}

	void renderer::unpack_rgb(int color, Uint8* r, Uint8* g, Uint8* b, Uint8* a) {
		*r = (color >> 0) & 0xff;
		*g = (color >> 8) & 0xff;
		*b = (color >> 16) & 0xff;
		*a = (color >> 24) & 0xff;
	}

	void renderer::to_pixel(float ux, float uy, int* px, int* py) {
		*px = (int)round(ux * ppu_x);
		*py = (int)round(uy * ppu_y); 
	}

	void renderer::to_units(int px, int py, float* ux, float* uy) {
		*ux = (1.0f / ppu_x) * px;
		*uy = (1.0f / ppu_y) * py;
	}
}





//traverse quadrant II
//for (int x0 = x - pxr; x0 <= x; x0++) {
//	for (int y0 = y - pxr; y0 <= y; y0++) {
//		float dist = sqrtf( (x0 - x) * (x0 - x) + (y0 - y) * (y0 - y));
//		if (dist <= pxr) {
//			SDL_RenderDrawPoint(handle, x0, y0);
//			SDL_RenderDrawPoint(handle, x - (x0 - x), y0);
//			SDL_RenderDrawPoint(handle, x0, y - (y0 - y));
//			SDL_RenderDrawPoint(handle, x - (x0 - x), y - (y0 - y));
//		}
//	}
//}