#pragma once

namespace core
{
	class FontAlign
	{
	public:
		enum Enum {
			LEFT = 0,
			CENTER,
			RIGHT
		};

		static const Enum DEFAULT = LEFT;
	};
}
