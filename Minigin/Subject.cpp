#include "MiniginPCH.h"
#include "Subject.h"
#include "Observer.h"

Subject::Subject()
	: m_NrOfObservers{ 0 }
{
}

Subject::~Subject()
{
	m_NrOfObservers = 0;
}

void Subject::AddObserver(Observer* observer)
{
	assert(observer != nullptr);

	m_Observers[m_NrOfObservers] = observer;
	++m_NrOfObservers;
}

void Subject::RemoveObserver(Observer* observer)
{
	for (int i = 0; i < m_NrOfObservers; ++i)
	{
		if (m_Observers[i] == observer)
		{
			std::swap(m_Observers[i], m_Observers[m_NrOfObservers]);
			delete m_Observers[m_NrOfObservers];
			m_Observers[m_NrOfObservers] = nullptr;

			--m_NrOfObservers;
			break;
		}
	}
}

void Subject::Notify(const dae::GameObject& actor, Event event)
{
	for (int i = 0; i < m_NrOfObservers; ++i)
	{
		m_Observers[i]->Notify(actor, event);
	}
}
