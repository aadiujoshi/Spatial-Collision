#pragma once

#include <vector>
#include <memory>
#include "object.h"

namespace sp {
	class spatial_partition {
	private:
	public:
		spatial_partition();
		~spatial_partition();

		virtual void collision(phys::object, std::vector<phys::object>&);
	};

	class spatial_hashing : public spatial_partition {
	private:
	public:
		spatial_hashing();
		~spatial_hashing();

		void collision(phys::object, std::vector<phys::object>&) override;
	};

	class quadtree : public spatial_partition {
		class node {
		private:
		public:
			//center coordinates in units
			int x, y;
			std::unique_ptr<sp::quadtree::node> branch;

			node();
			~node();
		};

	private:
	public:
		void collision(phys::object, std::vector<phys::object>&) override;
	};
}