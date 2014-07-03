#include "../Memory.h"
#include "ResourceObject.h"
#include <atomic>
using namespace core;

namespace {
	static std::atomic<uint32> uid(0);
	uint32 GenUID() {
		return ++uid;
	}
}

ResourceObject::ResourceObject()
: mUID(GenUID())
{

}

ResourceObject::~ResourceObject()
{

}
