#include "MiniginPCH.h"
#include "Achievements.h"
#include "GameObject.h"

enum class Event
{
	EVENT_ACTOR_DIED,
	EVENT_ACTOR_FELL,
	EVENT_ACTOR,
};

void Achievements::Notify(const dae::GameObject& /*gameObject*/, const Event& event)
{
	switch (event)
	{
	case Event::EVENT_ACTOR_DIED:
		Unlock(Achievement::FirstBlook);
		break;
	case Event::EVENT_ACTOR_FELL:
		// ...
		break;
	}
}

void Achievements::Unlock(Achievement /*achievement*/)
{
}
