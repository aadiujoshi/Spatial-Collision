#pragma once

#include "SDL2/SDL.h"
#include <vector>
#include <string>
#include "fwd_dec.h"

namespace gfx {
	class texture {
	public:
		//preloaded
		static gfx::texture* green_circle;

	private:
		std::vector<Uint32> buffer;
		SDL_Surface* surf_handle;

	public:
		SDL_Texture* tex_handle;
		Uint32 width, height;

		texture(const std::string& filepath, gfx::renderer& renderer);
		~texture();
	};
}