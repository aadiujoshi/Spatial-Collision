#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "renderer.h"
#include "paint_event.h"
#include "object.h"

namespace event {
	class paint_event;
	class update_event;
}
namespace phys {
	class object;
}

namespace gfx {
	class window {
	private:
		int capture_x, capture_y;
		char partition_type;
		volatile bool quit = false;
	public:
		SDL_Window* handle;
		gfx::renderer* renderer;
		SDL_MouseButtonEvent mouse;
		std::vector<phys::object> objs;

		window(int, int);
		~window();
		void start_window();

		void update_loop();

		void main_loop();

		void render(event::paint_event&);

		int getWidth();
		int getHeight();
	};
}