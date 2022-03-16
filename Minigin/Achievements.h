#pragma once
#include "Observer.h"

class Achievements : public Observer
{
public:

	void Notify(const dae::GameObject& gameObject, const Event& action) override;

private:
	enum class Achievement
	{
		FirstBlook
	};

	void Unlock(Achievement achievement);
};

