#pragma once
#include "GameObject.h"

enum class Event;
class Observer
{
public:
	virtual ~Observer() {};
	virtual void Notify(const dae::GameObject& gameObject, const Event& action) = 0;
	int GetId() const { return m_Id; }

protected:
	void SetId(int id) { m_Id = id; }

private:
	int m_Id{};
};

