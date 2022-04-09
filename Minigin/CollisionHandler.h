#pragma once
#include "box2d.h"

class CollisionHandler : public b2ContactListener
{
public:
	void BeginContact(b2Contact* pContact) override;
	void EndContact(b2Contact* pContact) override;

};
