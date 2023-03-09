class GetTarget : public LeafNode
{
private:

public:
	GetTarget() :
		LeafNode("Target") {}; 

	int execute(float fDeltaTime)
	{
		if (m_nState == RUNNING)
		{
			if (Blackboard->m_pTarget == NULL)
			{
				//	get valid target
				//	IDEA: Factions
				//	basenode bucket that hold sparate groups of entities
				//	inside the data bucket
				//	or faction component that identifies seperate groups
				BaseNode* Entities = Blackboard->EntityList;
				for (BaseNode* current = Entities->getStart(); current != NULL; current = current->getNext())
				{
					if (current != Blackboard->m_pSelf)
					{
						if (current->getChild<UserController>())
						{
							Blackboard->m_pTarget = current;
							return SUCCESS;
						}
					}
				}
				//	no valid target, try again next tick
				return RUNNING;
			}
			else
			{
				//	currently has target
				return SUCCESS;
			}
		}
		//	if no valid target
		return FAILURE;
	}
};