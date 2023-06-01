#pragma once

#include <SDL2/SDL.h>
#include "window.h"
#include "renderer.h"
#include "fwd_dec.h"

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