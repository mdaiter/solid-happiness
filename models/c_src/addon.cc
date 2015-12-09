#include <node.h>
#include "specialized_map.h"

namespace demo {

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
	SpecializedObjects::PriorityMap::Init(exports);
}

NODE_MODULE(addon, InitAll)

}
