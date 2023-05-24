#include <vector>
#include "renderer.h"
#include "object.h"

namespace event {
	class update_event {
	private:
	public:
		std::vector<phys::object>& objs;

		update_event(std::vector<phys::object>&);
		~update_event();
	};
}