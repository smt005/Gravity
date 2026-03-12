// ◦ Xyz ◦

#include "SpaceManager.h"
#include "Space.h"
#include "MainThreadSpace.h"


Space& SpaceManager::Current()
{
	if (!currentSpace) {
		return Get<Space>(true);
	}
	return *currentSpace;
}

void SpaceManager::Update()
{
	Current().Update();
}
