
#include "header_paths.h"
#include object_h
#include update_event_h
#include paint_event_h

#include "SDL2/SDL.h"
#include <math.h>
#include <iostream>

namespace phys {

	object::object(object_param& param) :
			sprite(*gfx::texture::green_circle){

		x = param.x;
		y = param.y;
		/*prev_x = x;
		prev_y = y;*/
		radius = param.radius;
		vx = param.vx;
		vy = param.vy;
		mass = param.mass;
		color = param.color;
		collision_checked = false;
	};

	object::~object() {

	}

	void object::update(event::update_event& e) {

		float delta_t = (e.ns_tick / 1000000000.0f);

		float dx = vx * delta_t;
		float dy = vy * delta_t;

		//forward checking for collision with walls or other objects to avoid entanglement / clipping
		x += dx;
		y += dy;

		bool x_invalid = false;
		bool y_invalid = false;

		//assume walls are of the same material (same COR)
		if (x - radius < 0 || x + radius > sp::UNITS_WIDTH) {
			vx *= -phys::coeff_restitution;
			x_invalid = true;
		}

		if (y - radius < 0) {
			vy *= -phys::coeff_restitution;
			y_invalid = true;
		}
		else {
			vy += delta_t * phys::gravity;
		}

		//collision
		if (!collision_checked) {
			std::vector<std::reference_wrapper<phys::object>> collide;
			e.spatial_partition.collision(*this, collide);

			if (collide.size() > 0)
				x_invalid = y_invalid = true;

			for (size_t i = 0; i < collide.size(); i++) {

				object& o2 = collide[i];

				/*float v_1f_x = (-phys::coeff_restitution * o2.mass * (vx - o2.vx) - (mass * vx + o2.mass * o2.vx))
					/ -(mass + 1);

				float v_1f_y = (-phys::coeff_restitution * o2.mass * (vy - o2.vy) - (mass * vy + o2.mass * o2.vy))
					/ -(mass + 1);

				float v_2f_x = (phys::coeff_restitution * mass * (vx - o2.vx) + (mass * vx + o2.mass * o2.vx))
					/ (o2.mass + 1);

				float v_2f_y = (phys::coeff_restitution * mass * (vy - o2.vy) + (mass * vy + o2.mass * o2.vy))
					/ (o2.mass + 1);*/

				/*float v_1f_x = (vx + o2.vx) / 2;
				float v_1f_y = (vy + o2.vy) / 2;
				float v_2f_x = v_1f_x;
				float v_2f_y = v_1f_y;*/

				//float ef_vx


				float theta = atan2f(o2.y - y, o2.x - x);
				float cos_ = cos(theta);
				float sin_ = sin(theta);

				//effective velocities after accounting for relative collision theta
				/*float v_1xie = 1;
				float v_1yie = 1;
				float v_2xie = 1;
				float v_2yie = 1;*/

				float p_tix = (mass * vx + o2.mass * o2.vx);
				float p_tiy = (mass * vy + o2.mass * o2.vy);

				float v_1f_x = (-phys::coeff_restitution * o2.mass * (o2.vx - vx) - p_tix) / 
												(mass + o2.mass);
								
				float v_1f_y = (-phys::coeff_restitution * o2.mass * (o2.vy - vy) - p_tiy) /
												(mass + o2.mass);

				float v_2f_x = (p_tix - mass * vx) / o2.mass;

				float v_2f_y = (p_tiy - mass * vy) / o2.mass;


				v_1f_x *= cos_;
				v_1f_y *= sin_;
				v_2f_x *= cos_;
				v_2f_y *= sin_;

				vx = v_1f_x;
				vy = v_1f_y;
				o2.vx = v_2f_x;
				o2.vy = v_2f_y;
			}
		}

		//undo
		if (x_invalid)
			x -= dx;
		if (y_invalid)
			y -= dy;
	}

	void object::paint(event::paint_event& e) {
		int px, py;
		gfx::renderer::to_pixel(x, y, &px, &py);
		int rad;
		gfx::renderer::to_pixel(radius, 1, &rad, &rad);
		rad *= radius;
		SDL_Rect pos = { px - rad, e.window->getHeight() - py - rad, rad*2 , rad*2};
		e.renderer->render_texture(pos, sprite);
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