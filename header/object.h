#pragma once

namespace phys {
	class object {
		//keep object creation concise
		typedef struct _object_param {
			float x, y;
			float radius;

			float theta;
			float v;
			float mass;
		} object_param;

	private:
		float theta;
		float v;
		float mass;

		float x, y;
		float radius;

	public:
		object();
		~object();
		void update();
		void paint();

		inline float getVx() {
			return v * cosf(theta);
		};

		inline float getVy() {
			return v * sinf(theta);
		};
	};
}