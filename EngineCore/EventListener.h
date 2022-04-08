#include <queue>
#include <mutex>

class _Event
{
public:
	int m_eType;

	_Event(int eType)
	{
		m_eType = eType;
	};
	~_Event() {};

	template <typename T> T* get()
	{
		return static_cast<T*> (this);
	}

};

class EventListener
{
protected:
	static map<int, vector<EventListener*>> Listeners;
	static queue<_Event*> Events;
	static mutex threadLock;

	//static map<int, map<EventListener*, eventCallBack>> CallBackListeners;

	virtual void onEvent(_Event* pEvent) {}
	void execute()
	{
		//m_fCallback();
	}

public:
	EventListener() {};
	~EventListener()
	{
		unregisterAll(this);
	}

	void operator()()
	{
		while (true)
		{
			
			if (Events.size() > 0)
			{				
				dispatchEvent();
			}
				
		}
	}

	void unregisterAll(EventListener* pListener)
	{
		map<int, vector<EventListener*>>::iterator itEventType;
		itEventType = Listeners.begin();
		while (itEventType != Listeners.end())									//	for each event type
		{
			for (int nI = 0; nI < itEventType->second.size(); nI++)				//	for each Listener
			{
				if (itEventType->second[nI] == pListener)
				{
					itEventType->second.erase(itEventType->second.begin() + nI);	//	remove
				}
			}
			itEventType++;
		}
	}

	void addCallbackListener(EventListener* pListener, int event, void (*callback)())
	{
		//CallBackListeners[event].insert(make_pair(pListener, callback));
	}

	void unregisterListener(EventListener* pListener, int Event)
	{
		map<int, vector<EventListener*>>::iterator Listener;
		Listener = Listeners.find(Event);
		if (Listener != Listeners.end())									//  if Listeners found
		{
			for (int nI = 0; nI < Listener->second.size(); nI++)			//	for each Listener
			{
				if (Listener->second[nI] == pListener)						//	compare
				{
					Listener->second.erase(Listener->second.begin() + nI);	//	remove
					OutputDebugStringA("unregistered listener. \n");
				}
			}
		}
	}

	void registerListener(EventListener* pListener, int event)
	{
		Listeners[event].push_back(pListener);
	}

	void registerListener(int event)
	{
		Listeners[event].push_back(this);
	}


	void addEvent(_Event* Event)
	{
		Events.push(Event);
	}

	void dispatchCallbackEvents()
	{
		//for (int nI = 0; nI < Events.size(); nI++)						// For each event in Queue(vector)
		//{
		//	//map<int, map<EventListener*, eventCallBack>>::iterator CallBackListener;
		//	//CallBackListener = CallBackListeners.find(Events[nI]->m_eType);
		//	//map<EventListener*, eventCallBack>::iterator Listener = CallBackListener->second.begin();
		//	//while (Listener != CallBackListener->second.end())
		//	{
		//		//	Listener->second;
		//	}

		//	delete Events[nI];											//	Delete event pointer data (does not remove element from vector)
		//}
		//Events.clear();													//	Now clear Events
	}

	void dispatchEvent()
	{

		threadLock.lock();

		map<int, vector<EventListener*>>::iterator Listener;
		_Event *pEvent = Events.front();

		Listener = Listeners.find(pEvent->m_eType);
		if (Listener != Listeners.end())							//  if Listeners found
		{
			for (int nJ = 0; nJ < Listener->second.size(); nJ++)	//	for each Listener
			{
				Listener->second[nJ]->onEvent(pEvent);				//	handle
			}
		}

		Events.pop();
		delete pEvent;

		threadLock.unlock();
	}

	void dispatchEvents()
	{
		//for (int nI = 0; nI < Events.size(); nI++)
		//{
		//	map<int, vector<EventListener*>>::iterator Listener;
		//	Listener = Listeners.find(Events[nI]->m_eType);				//	check for listeners for this event
		//	if (Listener != Listeners.end())							//  if Listeners found
		//	{
		//		for (int nJ = 0; nJ < Listener->second.size(); nJ++)	//	for each Listener
		//		{
		//			Listener->second[nJ]->onEvent(Events[nI]);			//	handle
		//		}
		//	}

		//	delete Events[nI];											//	delete event pointer data (does not remove element from vector)
		//}

		//Events.clear();													//	clear vector
	}

	void deleteListeners()
	{
		map<int, vector<EventListener*>>::iterator Listener;
		// for each key in map
		Listener = Listeners.begin();

		while (Listener != Listeners.end())
		{
			// delete elements in vector
			{
				Listener->second.clear();
				OutputDebugStringA("Deleting Listener. \n");
			}

			Listener++;
		}
	}
};

//typedef void (*eventCallBack)();
//map<int, map<EventListener*, eventCallBack>> EventListener::CallBackListeners;

map<int, vector<EventListener*>> EventListener::Listeners;
queue<_Event*> EventListener::Events;
mutex EventListener::threadLock;