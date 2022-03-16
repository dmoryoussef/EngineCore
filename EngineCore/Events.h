enum EVENT_TYPE
{
	CONSOLE_KEYBOARD_EVENT,
	CONSOLE_MOUSE_EVENT,
	GAMEPAD_EVENT,
	GUI_EVENT,
	NEW_BASENODE_EVENT,
	TRANSFORMUPDATE_EVENT,
	MOUSEWORLD_EVENT,
	SELECTIONSQUARE_EVENT,
	BASENODE_EVENT
};

class KeyboardEvent :
	public _Event
{
private:
	char m_chKey;
	bool m_bKeyDown;
public:
	KeyboardEvent(char chKey, bool bKeyDown) :
		m_chKey(chKey),
		m_bKeyDown(bKeyDown),
		_Event(CONSOLE_KEYBOARD_EVENT) {};

	bool isKeyDown()
	{
		return m_bKeyDown;
	}
	char getKey() { return m_chKey; }
};

class MouseEvent :
	public _Event
{
private:
	MouseState State;

public:
	MouseEvent(MouseState state) :
		State(state),
		_Event(CONSOLE_MOUSE_EVENT)
	{}

	MouseState getState()
	{
		return State;
	}
};

class MouseWorldEvent : public _Event
{
private:
	Vector2 WorldPosition;
	MouseState State;

public:
	MouseWorldEvent(Vector2 position, MouseState state) :
		WorldPosition(position),
		State(state),
		_Event(MOUSEWORLD_EVENT) {};

	Vector2 getWorldPosition()
	{
		return WorldPosition;
	}

	MouseState getState()
	{
		return State;
	}
};

class BaseNodeEvent :
	public _Event
{
private:
	BaseNode* m_pBaseNode;

public:
	BaseNodeEvent(BaseNode* pBaseNode) :
		m_pBaseNode(pBaseNode),
		_Event(BASENODE_EVENT) {};

	BaseNode* getBaseNode()
	{
		return m_pBaseNode;
	}

	template <typename T> T* getBaseNode()
	{
		/*if (typeid(*m_pBaseNode) == typeid(T))
			return static_cast<T*>(m_pBaseNode);

		return NULL;*/
		return dynamic_cast<T*>(m_pBaseNode);
	}
};