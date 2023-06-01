#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <array>
#include "object.h"
#include "fwd_dec.h"
#include "paint_event.h"

namespace sp {
	inline const float UNITS_WIDTH = 100;
	inline const float UNITS_HEIGHT = 100;

	const int pow2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

	typedef struct _rect {
		float x;
		float y;
		float w;
		float h;
	} rect;

	typedef struct _object_equal {
		bool operator()(const phys::object* comp1, const phys::object* comp2) const {
			return comp1 == comp2;
		};
	} object_equal;

	typedef struct _object_hash {
		size_t operator()(const phys::object* objRef) const {
			return std::hash<int>{}(reinterpret_cast<size_t>(objRef));
		}
	} object_hash;

	class spatial_partition {
	private:
	protected:
		std::vector<phys::object>& objs;
	public:
		spatial_partition(std::vector<phys::object>& objs) : objs(objs) {};

		virtual ~spatial_partition() {};

		virtual void collision(phys::object, std::vector<std::reference_wrapper<phys::object>>&) = 0;
		virtual void paint(event::paint_event&) = 0;
		virtual void insert(phys::object& ins_obj) = 0;
		virtual void refresh() = 0;
		virtual void clear() = 0;
	};

	class spatial_hashing : public spatial_partition {
	private:
	public:
		spatial_hashing(std::vector<phys::object>&);
		~spatial_hashing() override;

		void collision(phys::object, std::vector<std::reference_wrapper<phys::object>>&) override;
		void paint(event::paint_event&) override;
		void insert(phys::object& ins_obj) override;
		void refresh() override;
		void clear() override;
	};

	class quadtree : public spatial_partition {
	public:
		const static int MAX_DETAIL = 5;

		class node {
		private:
		public:
			int obj_count;
			int subdiv;
			//center coordinates in units
			float x, y;
			std::unique_ptr<std::unique_ptr<sp::quadtree::node>[]> branches;
			//only enabled if the node subdivision is at max detail
			std::unique_ptr<std::unordered_set<phys::object*, object_hash, object_equal>> contained_objs;

			node(int subdiv, float x, float y);
			~node();
			void clear();
		};

	private:
		std::unique_ptr<node> root;

	public:
		quadtree(std::vector<phys::object>&);
		~quadtree() override;

		void collision(phys::object, std::vector<std::reference_wrapper<phys::object>>&) override;
		void paint(event::paint_event&) override;
		void paint0(quadtree::node& node, event::paint_event& e);
		void insert(phys::object& ins_obj) override;
		void refresh() override;
		void insert_obj(node& node, phys::object& insert_obj);
		int search_unbound(node& node, std::vector<std::reference_wrapper<phys::object>>&);
		void clear() override;
	};
}