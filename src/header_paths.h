#pragma once

#define header_path <C:/Users/aadiu/Desktop/Programming Files/Personal Projects/C++ Projects/Spatial Collision/header/

#define __conc(a, b) a##b
#define __comp(a, b) __conc(a, b>)

#define window_h				__comp(header_path, window.h)
#define renderer_h				__comp(header_path, renderer.h)
#define object_h				__comp(header_path, object.h)
#define paint_event_h			__comp(header_path, paint_event.h)
#define update_event_h			__comp(header_path, update_event.h)