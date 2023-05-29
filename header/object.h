#pragma once

#include "paint_event.h"
#include "update_event.h"

namespace event {
	class paint_event;
	class update_event;
}

namespace phys {
	class object {
	private:

		float theta;
		float v;
		float mass;

		float x, y;
		float radius;

		int color;
	public:
		//keep object creation concise
		typedef struct _object_param {
			float x, y;
			float radius;

			float theta;
			float v;
			float mass;

			int color;
		} object_param;

		object(object_param&);
		~object();

		void update(event::update_event&);
		void paint(event::paint_event&);

		inline float getVx() {
			return v * cosf(theta);
		};

		inline float getVy() {
			return v * sinf(theta);
		};
	};
}