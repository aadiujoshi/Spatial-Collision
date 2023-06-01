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


//phys::object::object_param op = { 0 };

//op.x = 2;
//op.y = 2;
//op.radius = 0.9;

//phys::object obj(op);

//sp::rect r = {4, 1, 2, 2};

//std::cout << obj.overlap_rect(r);

//if (1) return 0;