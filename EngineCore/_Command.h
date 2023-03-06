class _Command
{
private:
	int m_id;

public:
	_Command(int id) :
		m_id(id) {};

	int getType() { return m_id; }

	template <typename T> T *get()
	{
		//	safety check is done elsewhere
		return static_cast<T*>(this);
	}
};
enum COMMANDS
{
	ACTION,	//	general action
	ACCELERATE,
	ROTATE,
	FIRE,
	MOVE
};

class AccelerateCommand : public _Command
{
private:
	Vector2 m_vForce;

public:
	AccelerateCommand(Vector2 f) :
		m_vForce(f),
		_Command(ACCELERATE) {};

	Vector2 getForce() { return m_vForce; }
};

class RotateCommand : public _Command
{
private:
	Vector2 m_vRotate;

public:
	RotateCommand(Vector2 r) :
		m_vRotate(r),
		_Command(ROTATE) {};

	Vector2 getVector() { return m_vRotate; }
};

class ActionCommand : public _Command
{
private:
	Vector2 vTarget;

public:
	ActionCommand() :
		_Command(FIRE) {};

	ActionCommand(Vector2 target) :
		vTarget(target),
		_Command(ACTION) {};

	Vector2 getTarget() { return vTarget; }
};
