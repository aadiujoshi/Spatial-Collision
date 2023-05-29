#include "header_paths.h"
#include renderer_h

#include "SDL2/SDL.h"

namespace gfx {
	renderer::renderer(SDL_Renderer* _handle) {
		handle = _handle;
	};

	renderer::~renderer() {
	};

	void renderer::drawCircle(int x, int y, int pxr, int color) {
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

	void renderer::to_pixel(int x, int y, int* nx, int* ny) {
		*nx = x;
		*ny = y;
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