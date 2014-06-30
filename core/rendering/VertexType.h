#pragma once
#include "../typedefs.h"
#include "PrimitiveType.h"

namespace core
{
	class VertexElementType
	{
	public:
		enum Enum {
			BYTE = 0,
			UNSIGNED_BYTE,
			SHORT,
			UNSIGNED_SHORT,
			INT,
			UNSIGNED_INT,
			FLOAT,
			DOUBLE,

			SIZE
		};
		
		/*!
			\brief Convert the supplied enum into a OpenGL enum
		*/
		static GLenum Parse(Enum e);
	};

	//
	struct VertexElementDesc
	{
		// The location specifies in what field the specific 
		// element is put when drawing the geometry in the graphics program.
		// This is mapped to the layout location in the vertex shader, for example: {@code layout(location = 0) in vec3 position}
		uint32 location;

		// The size of the current buffer field type. Use {@code sizeof(Type)}, for example: {@code sizeof(Vector2)}
		uint32 fieldSize;

		// The number of elements in this field. For example a Vector2 contains 2 elements (X and Y).
		uint32 numElementsInField;

		// What datatype should the element be handled as in the shader. the Vector3 type has float values, which means
		// that this field should be VertexElementType::FLOAT
		VertexElementType::Enum type;

		// Is the supplied value normalized or not?
		bool normalized;
	};

	//
	// Structure used to define how vertex buffer data is structured on the graphics card. End the declaration array with a 0 element.
	//
	// You create a vertex declaration like this:
	// {@code
	//  struct PositionTexCoordData
	//	{
	//		Vector3 position;
	//		Vector2 texCoord;
	//	};
	//
	//	static const VertexDesc PositionTexCoordDataVertexDeclaration = {
	//		{ 
	//			{ 0, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
	//			{ 1, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
	//			0 
	//		},
	//		PrimitiveType::TRIANGLE
	//	};
	// }
	struct VertexDesc
	{
		VertexElementDesc elements[8];
		PrimitiveType::Enum primitiveType;
	};

	//
	// Check if the supplied element descriptor should be handled as an integer type
	extern bool HandleAsIntegerType(const VertexElementDesc& elemDesc);
}
