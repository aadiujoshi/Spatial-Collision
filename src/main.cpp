#include <iostream>

#include "header_paths.h"
#include renderer_h
#include window_h
#include object_h
#include spatial_partition_h

#define WIN32_LEAN_AND_MEAN 0xBEEF

int main(int argc, char* argv[]) {
	gfx::window window(600, 600);
	window.start_window();
}