#include <SDL2/SDL.h>
#include <lodepng.h>
#include <string>
#include <iostream>
#include "header_paths.h"
#include renderer_h
#include texture_h

namespace gfx {
	texture* texture::green_circle = nullptr;

	texture::texture(const std::string& filepath, gfx::renderer& renderer) {

		std::vector<unsigned char> raw;

		unsigned int res = lodepng::decode(raw, width, height, filepath);

		if (res != 0) {
			std::cout << "could not load img" << std::endl;
			__debugbreak();
		}

		for (size_t i = 0; i < raw.size(); i+=4) {
			Uint32 f = 0;
			f |= raw[i + 0] << 0;
			f |= raw[i + 1] << 8;
			f |= raw[i + 2] << 16;
			f |= raw[i + 3] << 24;

			//std::cout << (int)raw[i] << "  " << (int)raw[i+1] << "  " << (int)raw[i+2] << "  " << (int)raw[i+3] << std::endl;

			buffer.push_back(f);
		}

		surf_handle = SDL_CreateRGBSurfaceFrom(	buffer.data(), width, height, 
									32, width * sizeof(Uint32), 
									0xFF, 0xFF00, 0xFF0000, 0xFF000000);

		tex_handle = SDL_CreateTextureFromSurface(renderer.handle, surf_handle);
	}

	texture::~texture() {
		SDL_FreeSurface(surf_handle);
		SDL_DestroyTexture(tex_handle);
	}
}