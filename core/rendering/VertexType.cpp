#include "../Memory.h"
#include "VertexType.h"
using namespace core;

namespace {
	bool IsIntegerType(const VertexElementDesc& elemDesc)
	{
		const VertexElementType::Enum type = elemDesc.type;
		return type == VertexElementType::INT || type == VertexElementType::UNSIGNED_INT || type == VertexElementType::BYTE
			|| type == VertexElementType::UNSIGNED_BYTE || type == VertexElementType::SHORT || type == VertexElementType::UNSIGNED_SHORT;
	}
}

bool core::HandleAsIntegerType(const VertexElementDesc& elemDesc)
{
	return IsIntegerType(elemDesc) && !elemDesc.normalized;
}
