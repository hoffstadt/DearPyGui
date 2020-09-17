#pragma once

namespace Marvel {

	class mvAppItem;

	bool AddItemWithRuntimeChecks(mvAppItem* item, const char* parent, const char* before);

}