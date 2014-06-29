#include "../Memory.h"
#include "ResourceObject.h"
#include <atomic>
using namespace core;

namespace {
	uint32 GenUID() {
		static std::atomic<uint32> uid(0);
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
