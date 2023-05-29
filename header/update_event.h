#pragma once

#include <vector>
#include "renderer.h"
#include "object.h"

namespace event {
	class update_event {
	private:
	public:
		static const char SPATIAL_HASHING = 1;
		static const char QUADTREE = 2;

		const char partition_type;

		//can either be node or hashmap
		void* spatial_partition;

		long long ns_tick;
		int swidth, sheight;
		std::vector<phys::object>& objs;

		update_event(long long, int, int, std::vector<phys::object>&, char, void*);
		~update_event();
	};
}