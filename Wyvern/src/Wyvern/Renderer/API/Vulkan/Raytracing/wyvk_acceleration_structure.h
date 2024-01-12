#pragma once
#include "Wyvern/Core.h"

namespace Wyvern {

	class WYVKAccelerationStructure
	{
	public:

		struct BLASInput
		{
			std::vector<VkAccelerationStructureGeometryKHR>       geometry;
			std::vector<VkAccelerationStructureBuildRangeInfoKHR> buildRangeInfo;
			VkBuildAccelerationStructureFlagsKHR                  flags{ 0 };
		};

	private:

	};

}