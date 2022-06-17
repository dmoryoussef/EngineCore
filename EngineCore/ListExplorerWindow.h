class ListExplorerWindow : public UIWindow
{
	class BaseNodeButton : public UIButton
	{
		private:
			BaseNode* m_pBaseNode;
			UIState* m_pUIState;

			void onStateChange()
			{
				//	open child window
				addEvent(new GuiEvent(this));
				//	update state of entity
				if (m_pUIState)
					m_pUIState->setState(m_nState);
			}

		public:
			BaseNodeButton(BaseNode* pBaseNode) :
				m_pBaseNode(pBaseNode),
				m_pUIState(NULL),
				UIButton(pBaseNode->getName()) 
			{
				if (UIState* pUIState = m_pBaseNode->getChild<UIState>())
					m_pUIState = pUIState;
			};

			BaseNode* getBaseNode()
			{
				return m_pBaseNode;
			}
	};

	class SingleSelectButtonComponent : public _UIComponent
	{
		private:
			UIButton *m_pCurrent;

			void onEvent(_Event* pEvent)
			{
				_UIComponent::onEvent(pEvent);

				switch (pEvent->m_eType)
				{
					case GUI_EVENT:
					{	
						if (GuiEvent *pGuiEvent = pEvent->get<GuiEvent>())
						{
							if (UIButton* pButton = pGuiEvent->getComponent<UIButton>())
							{
								if (pButton->getParent() == this)
								{
									//	if current button is different from active button, update everything
									if (pButton->getState() == LEFT_RELEASED)
									if (pButton != m_pCurrent)
									{

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
				_UIComponent() {};

	};


	void onEvent(_Event* pEvent)
	{
		UIWindow::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case NEW_BASENODE_EVENT:
			{
				updateButtons();
				break;
			}

			case GUI_EVENT:
			{
				if (BaseNodeButton* pButton = pEvent->get<GuiEvent>()->getComponent<BaseNodeButton>())
					if (pButton->getParent() == this)
					{
						switch (pButton->getState())
						{
							//	open new window
							case LEFT_RELEASED :
							{
								ListExplorerWindow* pWindow = new ListExplorerWindow(pButton->getBaseNode());
								pWindow->setAlignment(ALIGN_OUTSIDE_LEFT);
								addComponent(pWindow);
								break;
							}
							//	close child windows
							case DEFAULT:
							{
								while (isIterating())
								{
									if (ListExplorerWindow* pWindow = getCurrent <ListExplorerWindow>())
									{
										if (pWindow->getBaseNode() == pButton->getBaseNode())
										{
											//	delete window(which will also delete any child windows)
											removeCurrent();
										}
									}
								}
								break;
							}

						}
					}

				break;
			}
		}
	}

	void updateButtons()
	{
		int nMinHeight = 15;
		if (m_pBaseNode->getTotal() > nMinHeight)
			nMinHeight = m_pBaseNode->getTotal();

		m_nHeight = nMinHeight;

		clearList();
		if (m_pBaseNode)
			while (m_pBaseNode->isIterating())
			{
				addComponent(new BaseNodeButton(m_pBaseNode->getCurrent()));
			}
	}

	BaseNode* m_pBaseNode;

public:
	ListExplorerWindow(BaseNode *pNode) :
		m_pBaseNode(pNode),
		UIWindow(20, 15, 80, 0)
	{
		registerListener(NEW_BASENODE_EVENT);
		registerListener(GUI_EVENT);
		updateButtons();
	};

	BaseNode* getBaseNode() { return m_pBaseNode; }
};