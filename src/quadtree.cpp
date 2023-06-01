#include "header_paths.h"
#include <unordered_set>
#include <type_traits>
#include <functional>
#include <vector>
#include <memory>
#include <iostream>
#include spatial_partition_h
#include object_h

namespace sp {
	quadtree::node::node(int _subdiv, float _x, float _y) {
		subdiv = _subdiv;
		x = _x;
		y = _y;
	}

	quadtree::node::~node() {
	}

	void quadtree::node::clear() {
		if (!branches) {
			if (contained_objs)
				contained_objs.reset();
			return;
		}

		for (size_t i = 0; i < 4; i++) {
			branches[i].get()->clear();
		}
		branches.reset();
		if (contained_objs)
			contained_objs.reset();
	}

	quadtree::quadtree(std::vector<phys::object>& objs) : spatial_partition(objs) {
		root = std::make_unique<node>(0, sp::UNITS_WIDTH/2, sp::UNITS_HEIGHT/2);
	}

	quadtree::~quadtree() {

	}

	void quadtree::collision(phys::object compare, std::vector<std::reference_wrapper<phys::object>>& col) {

	}

	void quadtree::paint(event::paint_event& e) {
		SDL_SetRenderDrawColor(e.renderer->handle, 255, 255, 255, 255);
		paint0(*root, e);
	}

	void quadtree::paint0(quadtree::node& node, event::paint_event& e) {
		SDL_Renderer* rend = e.renderer->handle;

		int px, py, pw, ph;
		gfx::renderer::to_pixel(node.x, node.y, &px, &py);
		gfx::renderer::to_pixel(sp::UNITS_WIDTH / pow2[node.subdiv], sp::UNITS_HEIGHT / pow2[node.subdiv], &pw, &ph);
		SDL_Rect b = {px - pw / 2, py - ph / 2, pw, ph};

		SDL_RenderDrawRect(rend, &b);

		if (node.branches) {
			for (int i = 0; i < 4; i++) {
				paint0(*node.branches[i].get(), e);
			}
		}
	}

	void quadtree::refresh() {
		std::vector<std::reference_wrapper<phys::object>> to_insert;

		search_unbound(*root, to_insert);

		for (std::reference_wrapper<phys::object>& ob : to_insert) {
			insert_obj(*root, ob.get());
		}

	}

	int quadtree::search_unbound(node& node, std::vector<std::reference_wrapper<phys::object>>& unbound) {
		if (node.contained_objs) {
			sp::rect rect = { node.x, node.y, sp::UNITS_WIDTH / pow2[node.subdiv], sp::UNITS_HEIGHT / pow2[node.subdiv] };

			std::vector<phys::object*> invalid;
			auto& node_objs = *node.contained_objs.get();

			for (phys::object* po : node_objs) {
				if (! po->overlap_rect(rect)) {
					invalid.push_back(po);
					unbound.push_back(*po);
				}
			}

			for (auto ob : invalid){
				node_objs.erase(ob);
			}

			node.obj_count = node_objs.size();

			return invalid.size();
		}

		if (!node.branches)
			return 0;

		int lost_objs = 0;
		for (size_t i = 0; i < 4; i++) {
			lost_objs += search_unbound(*node.branches[i].get(), unbound);
		}

		node.obj_count -= lost_objs;

		//release branch data
		if (node.obj_count == 0) {
			for (size_t i = 0; i < 4; i++) {
				node.branches[i].reset();

			}
			node.branches.reset();
		}

		return lost_objs;
	}

	void quadtree::insert(phys::object& ins_obj) {
		insert_obj(*root, ins_obj);
	}

	void quadtree::insert_obj(node& node, phys::object& ins_obj) {

		float div = pow2[node.subdiv];
		//bounds
		sp::rect rect = { node.x, node.y, sp::UNITS_WIDTH / div, sp::UNITS_HEIGHT / div };

		if (!ins_obj.overlap_rect(rect))
			return;

		node.obj_count++;

		if (node.subdiv == sp::quadtree::MAX_DETAIL) {
			if (!node.contained_objs) {
				node.contained_objs = std::make_unique<std::unordered_set<phys::object*, object_hash, object_equal>>();
			}
			node.contained_objs.get()->insert(&ins_obj);

			return;
		}

		if (ins_obj.overlap_rect(rect)) {
			if (!node.branches) {
				node.branches = std::make_unique<std::unique_ptr<quadtree::node>[]>(4);
				int xm = 1;
				int ym = -1;
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 2; j++) {
						node.branches[i * 2 + j] = std::make_unique<quadtree::node>(
							node.subdiv + 1,
							//current boundary center pos + (left or right * quarter of boundary);
							node.x + xm * (rect.w / 4.0f),
							node.y + ym * (rect.h / 4.0f));

						xm *= -1;
					}
					ym *= -1;
				}
			}

			for (size_t i = 0; i < 4; i++) {
				insert_obj(*node.branches[i].get(), ins_obj);
			}
		}
	}

	void quadtree::clear() {
		root->clear();
	}
}


//if (node.subdiv == sp::quadtree::MAX_DETAIL) {

//	return;
//}

//float div = pow2[node.subdiv];
////bounds
//sp::rect rect = { node.x, node.y, sp::UNITS_WIDTH / div, sp::UNITS_HEIGHT / div };

//float halfWidth = rect.w / 2.0f;
//float halfHeight = rect.h / 2.0f;

////std::cout << "\t " << node.subdiv << std::endl;

////std::cout << node.x << "  " << node.y << "  " << node.subdiv << "  " << sp::UNITS_WIDTH / div << "  " << sp::UNITS_HEIGHT / div << std::endl;

//for (auto& obj : objs) {
//	if (obj.overlap_rect(rect)) {
//		node.branches = std::make_unique<std::unique_ptr<quadtree::node>[]>(4);
//		int xm = 1;
//		int ym = -1;
//		for (int i = 0; i < 2; i++) {
//			for (int j = 0; j < 2; j++) {
//				node.branches[i * 2 + j] = std::make_unique<quadtree::node>(
//					node.subdiv + 1,
//					//current boundary center pos + (left or right * quarter of boundary);
//					node.x + xm * (halfWidth / 2.0f),
//					node.y + ym * (halfHeight / 2.0f));

//				xm *= -1;
//			}
//			ym *= -1;
//		}

//		for (size_t i = 0; i < 4; i++) {
//			fill_root(*node.branches[i].get());
//		}

//		break;
//	}
//}