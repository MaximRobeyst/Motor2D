#pragma once
#include "GameObject.h"

enum class Event;
class Observer
{
public:
	virtual ~Observer() {};
	virtual void Notify(const dae::GameObject& gameObject, const Event& action) = 0;
};

