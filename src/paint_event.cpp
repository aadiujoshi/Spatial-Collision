#include "header_paths.h"
#include paint_event_h
#include renderer_h
#include window_h

namespace event {
	paint_event::paint_event(gfx::window* window) : 
			renderer(window->renderer), window(window){
	};

	paint_event::~paint_event() {

	};
}