#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "renderer.h"
#include "paint_event.h"
#include "object.h"
#include "fwd_dec.h"
#include "spatial_partition.h"

namespace gfx {
	inline int screen_width = 0, screen_height = 0;
	inline long long tick_speed = 1000000 * (1/5.0f);

	void ns_delay(long long ns);

	class window {
	private:
		bool render_partition;
		std::unique_ptr<sp::spatial_partition> partition_method;
		int capture_x, capture_y;
		char partition_type;
		volatile bool quit = false;

		long long tick_delay_ns;
	public:
		SDL_Window* handle;
		gfx::renderer* renderer;
		SDL_MouseButtonEvent mouse;
		std::vector<phys::object> objs;

		window(int, int);
		~window();
		void start_window();

		void main_loop();

		void render_update(event::paint_event&);

		int getWidth();
		int getHeight();
	};
}