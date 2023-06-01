#pragma once

#include "paint_event.h"
#include "update_event.h"
#include "fwd_dec.h"
#include "texture.h"
#include "spatial_partition.h"
#include <math.h>

namespace phys {
	inline float gravity = (float)(30);

	class object {
	private:
		gfx::texture& sprite;

	public:
		//keep object creation concise
		typedef struct _object_param {
			float x, y;
			float radius;

			float vx, vy;
			float mass;

			float coeff_restitution;

			int color;
		} object_param;

		float x, y;
		float radius;

		float vx, vy;
		float mass;

		float coeff_restitution;

		int color;

		object(object_param&);
		~object();

		void update(event::update_event&);
		void paint(event::paint_event&);
		bool collide(object&);

		bool overlap_rect(sp::rect& rect);
	};
}