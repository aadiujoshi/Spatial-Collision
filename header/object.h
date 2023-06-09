#pragma once

#include "paint_event.h"
#include "update_event.h"
#include "fwd_dec.h"
#include "texture.h"
#include "spatial_partition.h"
#include <math.h>

namespace phys {
	inline float gravity = (float)(-100);
	//use this one, same for all objects
	inline float coeff_restitution = (float)(0.7);

	/*
						OLD FORMULA
						OLD FORMULA
						OLD FORMULA

			-e*m_2*(v_1 - v_2) - (m_1*v_1 + m_2*v_2)
	v_1f =  ----------------------------------------
						   -(m_1 + 1)

			e*m_1*(v_1 - v_2) + m_1*v_1 + m_2*v_2
	v_2f =  ----------------------------------------
							(m_2 + 1)


			REVISED REVISED REVISED REVISED REVISED
			working

		formulas for final velocities after collision of 2 objects
		using coefficient of restitution and conservation of momentum

			-e*m_2(v_2 - v_1) - (m_1*v_1 + m_2*v_2)
	v_1f =  ---------------------------------------
						(m_1 + m_2)

			m_1*v_1 + m_2*v_2 - m_1*v_1
	v_2f =  ---------------------------
						m_2

	*/

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

			int color;
		} object_param;

		//float prev_x, prev_y;
		float x, y;
		float radius;

		float vx, vy;
		float mass;

		int color;

		bool collision_checked;

		object(object_param&);
		~object();

		void update(event::update_event&);
		void paint(event::paint_event&);
		bool collide(object&);

		bool overlap_rect(sp::rect& rect);
	};
}