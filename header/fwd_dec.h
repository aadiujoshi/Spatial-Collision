#pragma once

//this class exists because c++ is weird

namespace event {
	class paint_event;
	class update_event;
}

namespace phys {
	class object;
}

namespace sp {
	class spatial_partition;
	class quadtree;
	class spatial_hashing;
	typedef struct _rect rect;
}

namespace gfx {
	class renderer;
	class window;
	class texture;
}