class SingleSelectButtonComponent : public _UIComponent
{
private:
	UIButton* m_pCurrent;

	void onEvent(_Event* pEvent)
	{
		_UIComponent::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case GUI_EVENT:
			{
				if (GuiEvent* pGuiEvent = pEvent->get<GuiEvent>())
				{
					if (UIButton* pButton = pGuiEvent->getComponent<UIButton>())
					{
						if (pButton->getParent() == this)
						{
							//	if current button is different from active button, update everything
							if (pButton->getState() == LEFT_RELEASED)
							{
								if (m_pCurrent != NULL)
									if (pButton != m_pCurrent)
										m_pCurrent->setState(DEFAULT);
									
								m_pCurrent = pButton;
							}
						}
					}

				}
				break;
			}

		}
	}

public:
	SingleSelectButtonComponent() :
		m_pCurrent(NULL),
		_UIComponent() {};

};