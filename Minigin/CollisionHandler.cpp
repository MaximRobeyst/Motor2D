#include "MiniginPCH.h"
#include "CollisionHandler.h"
#include "RigidbodyComponent.h"

void CollisionHandler::BeginContact(b2Contact* pContact)
{
	auto pRigidbody1 = reinterpret_cast<dae::RigidbodyComponent*>(pContact->GetFixtureA()->GetBody()->GetUserData().pointer);
	auto pRigidbody2 = reinterpret_cast<dae::RigidbodyComponent*>(pContact->GetFixtureB()->GetBody()->GetUserData().pointer);

	pRigidbody1->OnBeginContact(pRigidbody2, pContact);
}

void CollisionHandler::EndContact(b2Contact* pContact)
{
	auto pRigidbody1 = reinterpret_cast<dae::RigidbodyComponent*>(pContact->GetFixtureA()->GetBody()->GetUserData().pointer);
	auto pRigidbody2 = reinterpret_cast<dae::RigidbodyComponent*>(pContact->GetFixtureB()->GetBody()->GetUserData().pointer);

	pRigidbody1->OnEndContact(pRigidbody2, pContact);
}
