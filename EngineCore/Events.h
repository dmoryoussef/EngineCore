enum EVENT_TYPE
{
	KEYBOARD_EVENT,
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
	COMMAND_EVENT,
	EDITOROBJECT_EVENT
};

class EditorObjectEvent : public _Event
{
private:
	vector<Vector2> Objects;

public:
	EditorObjectEvent(Vector2 prevMin, Vector2 prevMax, Vector2 curMin, Vector2 curMax) :
		_Event(EDITOROBJECT_EVENT) 
	{
		addObject(prevMin, prevMax, curMin, curMax);
	};

	EditorObjectEvent() :
		Objects(),
		_Event(EDITOROBJECT_EVENT) {};

	void addObject(Vector2 prevMin, Vector2 prevMax, Vector2 curMin, Vector2 curMax)
	{
		Objects.push_back(prevMin); 
		Objects.push_back(prevMax);
		Objects.push_back(curMin);
		Objects.push_back(curMax);
	}

	vector<Vector2> getObjects() { return Objects; }
};

class SelectionSquareEvent : public _Event
{
private:
	Vector2 vMin;
	Vector2 vMax;
	Vector2 vStart;
	Vector2 vStop;
	bool bReleased;

	Vector2 vPrevMin;
	Vector2 vPrevMax;

public:
	SelectionSquareEvent(Vector2 min, Vector2 max, Vector2 start, Vector2 stop, bool active, Vector2 prevMin = {0, 0}, Vector2 prevMax = {0, 0}) :
		vMin(min),
		vMax(max),
		vStart(start),
		vStop(stop),
		vPrevMin(prevMin),
		vPrevMax(prevMax),
		bReleased(active), 
		_Event(SELECTIONSQUARE_EVENT) {};

	Vector2 getMax()
	{
		return vMax;
	}

	Vector2 getMin()
	{
		return vMin;
	}

	Vector2 getStart()
	{
		return vStart;
	}

	Vector2 getStop()
	{
		return vStop;
	}

	bool isReleased()
	{
		return bReleased;
	}
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
		_Event(KEYBOARD_EVENT) {};

	bool isKeyDown(char key)
	{
		return (m_bKeyDown && key == m_chKey);
	}

	bool isKeyUp(char key)
	{
		return (!m_bKeyDown && key == m_chKey);
	}

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