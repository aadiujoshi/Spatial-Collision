#include <SDL2/SDL.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <iostream>

#include "header_paths.h"
#include renderer_h
#include window_h
#include object_h

#define WIN32_LEAN_AND_MEAN 0xBEEF

int main(int argc, char* argv[]) {
	//std::thread wt(app_threads::window_thread);
	//std::thread ut(app_threads::update_thread);

	//wt.detach();
	//ut.join();

	gfx::window window(600, 600);
	window.start_window();
}
