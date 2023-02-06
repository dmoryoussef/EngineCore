class ConnectingLine : public EventListener
{
private:
	Vector2 vStart;
	Vector2 vEnd;

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		vEnd = pEvent->getWorldPosition();
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
					break;
		}
	}

public:
	ConnectingLine(Vector2 s) :
		vStart(s),
		vEnd(s)
	{
		registerListener(MOUSEWORLD_EVENT);
	}

	void render(Render2D* r)
	{
		r->DrawLine(vStart, vEnd);
		r->DrawCircle(vStart.X, vStart.Y, 1, { PIXEL_SOLID, FG_WHITE });
		r->DrawCircle(vEnd.X, vEnd.Y, 1, { PIXEL_SOLID, FG_WHITE });
	}

};