
#include "header_paths.h"
#include object_h
#include update_event_h
#include paint_event_h

#include "SDL2/SDL.h"

namespace phys {

	object::object(object_param& param) {
		x = param.x;
		y = param.y;
		radius = param.radius;
		theta = param.theta;
		v = param.v;
		mass = param.mass;
		color = param.color;
	};

	object::~object() {

	}

	void object::update(event::update_event& e) {
		if(y <= 600)
			y++;
	}

	void object::paint(event::paint_event& e) {
		e.renderer->drawCircle(x, y, radius, color);
	}
}