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