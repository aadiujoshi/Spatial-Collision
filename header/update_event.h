#pragma once

#include <vector>
#include "renderer.h"
#include "object.h"
#include "spatial_partition.h"
#include "fwd_dec.h"

namespace event {
	class update_event {
	private:
	public:
		static const char SPATIAL_HASHING = 1;
		static const char QUADTREE = 2;

		const char partition_type;

		//can either be node or hashmap
		sp::spatial_partition& spatial_partition;

		long long ns_tick;
		std::vector<phys::object>& objs;
		
		update_event(long long, std::vector<phys::object>&, char, sp::spatial_partition&);
		~update_event();
	};
}