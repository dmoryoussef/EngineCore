class _EntityComponent :
	public BaseNode,
	public EventListener
{
private:
	string m_strDescription;		//	describes what this component does
									//	can be part of pop up window or detail window
public:
	_EntityComponent(string strName) :
		BaseNode(strName) {};

	void setDescription(string strDescription)
	{
		m_strDescription = strDescription;
	}

	string getDescription()
	{
		return m_strDescription;
	}

	virtual void addComponent(_EntityComponent *pComponent)
	{
		addChild(pComponent);
	}
};