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

	template <typename T> T* get()
	{
		//	safety check is done elsewhere
		return static_cast<T*> (this);
	}

};

class CallBack
{
public:
	CallBack(void (*callback)())
	{
		handleEvent = callback;
	};

	void (*handleEvent)() = 0;
};

class EventListener
{
protected:
	static map<int, vector<EventListener*>> Listeners;
	static queue<_Event*> Events;
	static mutex threadLock;

	typedef void (*eventCallBack)();
	static map<int, map<EventListener*, CallBack*>> CallBackListeners;

	virtual void onEvent(_Event* pEvent) {}

	static int totalEvents;

public:
	EventListener() {};
	~EventListener()
	{
		unregisterAll();
	}

	//void operator()()
	//{
	//	while (true)
	//	{
	//		
	//		if (Events.size() > 0)
	//		{				
	//			dispatchEvent();
	//		}
	//			
	//	}
	//}

	void unregisterAll()
	{
		map<int, vector<EventListener*>>::iterator itEventType;
		itEventType = Listeners.begin();
		while (itEventType != Listeners.end())									//	for each event type
		{
			for (int nI = 0; nI < itEventType->second.size(); nI++)				//	for each Listener
			{
				if (itEventType->second[nI] == this)
				{
					itEventType->second.erase(itEventType->second.begin() + nI);	//	remove
				}
			}
			itEventType++;
		}
	}

	void registerCallbackListener(int nEvent, CallBack *callBack)
	{
		CallBackListeners[nEvent].insert(make_pair(this, callBack));
	}

	void unregisterListener(EventListener* pListener, int nEvent)
	{
		map<int, vector<EventListener*>>::iterator Listener;
		Listener = Listeners.find(nEvent);
		if (Listener != Listeners.end())									//  if Listeners found
		{
			for (int nI = 0; nI < Listener->second.size(); nI++)			//	for each Listener
			{
				if (Listener->second[nI] == pListener)						//	compare
				{
					Listener->second.erase(Listener->second.begin() + nI);	//	remove
				}
			}
		}
	}

	void registerListener(EventListener* pListener, int nEvent)
	{
		Listeners[nEvent].push_back(pListener);
	}

	void registerListener(int nEvent)
	{
		Listeners[nEvent].push_back(this);
	}


	void addEvent(_Event* Event)
	{
		totalEvents++;
		Events.push(Event);
	}

	void dispatchCallbackEvent(_Event *pEvent)
	{
		// find list of listeners for given event type
		map<int, map<EventListener*, CallBack*>>::iterator CallBackListenerType;
		CallBackListenerType = CallBackListeners.find(pEvent->m_eType);	

		if (CallBackListenerType != CallBackListeners.end())	// listener found
		{
			map<EventListener*, CallBack*>::iterator Listener = CallBackListenerType->second.begin();

			while (Listener != CallBackListenerType->second.end())
			{
				Listener->second->handleEvent();
			}
		}
	}

	

	void dispatchEvent(_Event *pEvent)
	{
		map<int, vector<EventListener*>>::iterator Listener;
		Listener = Listeners.find(pEvent->m_eType);
		if (Listener != Listeners.end())							//  if Listeners found
		{
			for (int nJ = 0; nJ < Listener->second.size(); nJ++)	//	for each Listener
			{
				Listener->second[nJ]->onEvent(pEvent);				//	handle
			}
		}
	}

	void dispatchEvents()
	{
		while (Events.size() > 0)
		{
			_Event* pEvent = Events.front();
			dispatchEvent(pEvent);
			dispatchCallbackEvent(pEvent);
			Events.pop();
			delete pEvent;
		}
		
	}

	//void dispatchEvents()
	
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

map<int, map<EventListener*, CallBack*>> EventListener::CallBackListeners;


map<int, vector<EventListener*>> EventListener::Listeners;
queue<_Event*> EventListener::Events;
int EventListener::totalEvents;