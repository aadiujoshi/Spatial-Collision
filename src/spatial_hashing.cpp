#include "header_paths.h"
#include "..\header\spatial_partition.h"
#include spatial_partition_h

namespace sp {
	spatial_hashing::spatial_hashing(std::vector<phys::object>& objs) : spatial_partition(objs) {

	}
	spatial_hashing::~spatial_hashing()
	{
	}
	void spatial_hashing::collision(phys::object, std::vector<std::reference_wrapper<phys::object>>&)
	{
	}
	void spatial_hashing::paint(event::paint_event&)
	{
	}
	void spatial_hashing::insert(phys::object& ins_obj)
	{
	}
	void spatial_hashing::refresh()
	{
	}
	void spatial_hashing::clear()
	{
	}
}