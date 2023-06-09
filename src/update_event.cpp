
#include "header_paths.h"
#include update_event_h

namespace event {

	update_event::update_event(	long long ns_tick, 
								std::vector<phys::object>& objs, 
								char partition_type, sp::spatial_partition& spatial_partition) :
		ns_tick(ns_tick), 
		objs(objs), 
		partition_type(partition_type), 
		spatial_partition(spatial_partition) {};

	update_event::~update_event() {

	};
};