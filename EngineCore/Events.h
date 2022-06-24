enum EVENT_TYPE
{
	CONSOLE_KEYBOARD_EVENT,
	CONSOLE_MOUSE_EVENT,
	GAMEPAD_EVENT,
	GUI_EVENT,
	NEW_BASENODE_EVENT,
	DELETE_BASENODE_EVENT,
	TRANSFORMUPDATE_EVENT,
	MOUSEWORLD_EVENT,
	SELECTIONSQUARE_EVENT,
	BASENODE_EVENT,
	ACTION_EVENT,
	COMMAND_EVENT
};


class GuiEvent : public _Event
{
private:
	BaseNode* m_pComponent;

public:
	GuiEvent(BaseNode* pComponent) :
		m_pComponent(pComponent),
		_Event(GUI_EVENT) {};

	template <typename T>
	T *getComponent()
	{
		return dynamic_cast<T*>(m_pComponent);
	}

};

class DeleteBaseNodeEvent : public _Event
{
private:
	BaseNode* m_pBaseNode;

public:
	DeleteBaseNodeEvent(BaseNode* pNode) :
		m_pBaseNode(pNode),
		_Event(DELETE_BASENODE_EVENT) {};

	BaseNode* getNode() { return m_pBaseNode; }
};

class NewBaseNodeEvent : public _Event
{
private:
	BaseNode* m_pBaseNode;

public:
	NewBaseNodeEvent(BaseNode* pNode) :
		m_pBaseNode(pNode),
		_Event(NEW_BASENODE_EVENT) {};

	BaseNode *getNode() { return m_pBaseNode; }
};

class CommandEvent : public _Event
{
private:
	BaseNode* m_pParent;
	_Command *m_pCommand;

public:
	CommandEvent(BaseNode *pParent, _Command *pCommand) :
		m_pParent(pParent),
		m_pCommand(pCommand),
		_Event(COMMAND_EVENT) {};

	BaseNode* getParent() { return m_pParent; }
	_Command *getCommand() { return m_pCommand; }
};

class ActionEvent : public _Event
{
private:
	BaseNode* m_pBaseNode;
public:
	ActionEvent(BaseNode* pBaseNode) :
		m_pBaseNode(pBaseNode),
		_Event(ACTION_EVENT) {}

	BaseNode* getNode()
	{
		return m_pBaseNode;
	}
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

class GamePadEvent :
	public _Event
{
private:

	GamePad m_Controller;

public:
	GamePadEvent(GamePad controller) :
		m_Controller(controller),
		_Event(GAMEPAD_EVENT) {}

	GamePad getState()
	{
		return m_Controller;
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