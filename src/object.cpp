
#include "header_paths.h"
#include object_h
#include update_event_h
#include paint_event_h

#include "SDL2/SDL.h"
#include <math.h>

namespace phys {

	object::object(object_param& param) :
			sprite(*gfx::texture::green_circle){

		x = param.x;
		y = param.y;
		radius = param.radius;
		vx = param.vx;
		vy = param.vy;
		mass = param.mass;
		coeff_restitution = param.coeff_restitution;
		color = param.color;
	};

	object::~object() {

	}

	void object::update(event::update_event& e) {

		float delta_t = (e.ns_tick / 1000000000.0f);

		x += vx * delta_t;
		y += vy * delta_t;

		vy += delta_t * phys::gravity;
	}

	void object::paint(event::paint_event& e) {
		int px, py;
		gfx::renderer::to_pixel(x, y, &px, &py);
		e.renderer->render_texture(px, py, sprite);
	}

	bool object::collide(object& other) {
		return	(x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) <= 
				((other.radius + radius) * (other.radius + radius));
	}

	bool object::overlap_rect(sp::rect& rect) {
		float cdist_x = abs(x - rect.x);
		float cdist_y = abs(y - rect.y);

		if (cdist_x > (rect.w / 2 + radius)) { return false; }
		if (cdist_y > (rect.h / 2 + radius)) { return false; }

		if (cdist_x <= (rect.w / 2)) { return true; }
		if (cdist_y <= (rect.h / 2)) { return true; }

		float cornerDistance_sq =	(cdist_x - rect.w / 2) * (cdist_x - rect.w / 2) + 
									(cdist_y - rect.h / 2) * (cdist_y - rect.h / 2);

		return (cornerDistance_sq <= (radius * radius));
	}
}