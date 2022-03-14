#pragma once

class GameObject;
class Action;
class Observer
{
public:
	virtual ~Observer() {};
	virtual void OnNotify(const GameObject&, const Action&) = 0;
};

