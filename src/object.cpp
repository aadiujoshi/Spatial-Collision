
#include "header_paths.h"
#include object_h
#include update_event_h
#include paint_event_h

namespace phys {
	bool object::moveTo(float nx, float ny, event::update_event) {

	};

	object::object(object_param& param) {
		x = param.x
		y = param.y
		radius = param.radius
		theta = param.theta
		v = param.v
		mass = param.mass
	};

	object::~object() {

	}

	void object::update(event::update_event e) {

	}

	void object::paint(event::paint_event e) {

	}

}