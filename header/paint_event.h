#pragma once

#include <SDL2/SDL.h>
#include "window.h"
#include "renderer.h"

namespace gfx {
	class renderer;
	class window;
}

namespace event {
	class paint_event {
	private:
	public:
		gfx::renderer* renderer;;
		gfx::window* window;

		paint_event(gfx::window*);
		~paint_event();
	};
}