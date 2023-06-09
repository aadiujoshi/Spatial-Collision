#include "header_paths.h"
#include spatial_partition_h

namespace sp {
	spatial_hashing::spatial_hashing(std::vector<phys::object>& objs) : spatial_partition(objs) {

		blocks = std::make_unique<std::unique_ptr<std::unordered_set<phys::object*, object_hash, object_equal>[]>[]>(WIDTH_PARTITION);

		for (size_t i = 0; i < WIDTH_PARTITION; i++) {
			blocks[i] = std::make_unique<std::unordered_set<phys::object*, object_hash, object_equal>[]>(HEIGHT_PARTITION);
		}
	}

	spatial_hashing::~spatial_hashing() {

	}

	void spatial_hashing::collision(phys::object&, std::vector<std::reference_wrapper<phys::object>>&) {

	}

	void spatial_hashing::paint(event::paint_event&) {

	}

	void spatial_hashing::insert(phys::object& ins_obj) {

	}

	void spatial_hashing::refresh() {

	}

	void spatial_hashing::clear() {

	}
}