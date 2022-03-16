#pragma once
#include "GameObject.h"

class Observer;
enum class Event;

const int MAX_OBSERVERS = 16;

class Subject
{
public:
	Subject();
	~Subject();

	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

	void Notify(const dae::GameObject& actor, Event event);

private:
	Observer* m_Observers[MAX_OBSERVERS]{};
	int m_NrOfObservers{};
};

