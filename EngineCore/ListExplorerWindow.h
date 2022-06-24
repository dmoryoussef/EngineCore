class ListExplorerWindow : public UIWindow
{
	BaseNode* m_pBaseNode;
	
	class BaseNodeButton : public UIButton
	{
		private:
			BaseNode* m_pBaseNode;
			UIState* m_pUIState;

			void onStateChange()
			{
				//	open/close child window if activated
				addEvent(new GuiEvent(this));

				//	update state of entity
				if (m_pUIState)
					m_pUIState->setState(m_nState);
			}

			void onEvent(_Event* pEvent)
			{
				UIButton::onEvent(pEvent);

				switch (pEvent->m_eType)
				{
					
					case GUI_EVENT:
					{
						if (m_pUIState)
						{
							if (GuiEvent* pGuiEvent = pEvent->get<GuiEvent>())
							{
								if (pGuiEvent->getComponent<UIState>() == m_pUIState)
								{
									int nNewState = m_pUIState->getState();
									if (m_nState != nNewState)
									{
										//	setState(nNewState);
									}
								}
							}
						}
						
						break;
					}
				}
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

			case DELETE_BASENODE_EVENT:
			{
				/*BaseNode* pBaseNode = pEvent->get<DeleteBaseNodeEvent>()->getNode();
				while (isIterating())
				{
					if (getCurrent<BaseNodeButton>()->getBaseNode() == pBaseNode)
					{
						removeCurrent();
					}
				}*/
				updateButtons();
				break;
			}

			case GUI_EVENT:
			{
				if (GuiEvent* pGuiEvent = pEvent->get<GuiEvent>())
				{
					if (BaseNodeButton* pButton = pGuiEvent->getComponent<BaseNodeButton>())
					{
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
												//	use event system?
												removeCurrent();
											}
										}
									}
									break;
								}

							}
						}
					}
				}
				

				break;
			}
		}
	}

	void constructComponent()
	{
		if (m_pBaseNode)
		{
			if (m_pBaseNode->getTotal() == 0)
				set(m_pBaseNode->toString(), 2, 2, FG_WHITE);
		}
	}

	void updateButtons()
	{
		int nMinHeight = 15;
		m_nHeight = nMinHeight;

		if (m_pBaseNode->getTotal() > nMinHeight - 3)
			nMinHeight = m_pBaseNode->getTotal() + 3;

		resize(m_nWidth, nMinHeight);

		clearList();

		if (m_pBaseNode)
		{
			while (m_pBaseNode->isIterating())
			{
				addComponent(new BaseNodeButton(m_pBaseNode->getCurrent()));
			}
		}

	}


public:
	ListExplorerWindow(BaseNode *pNode) :
		m_pBaseNode(pNode),
		UIWindow(20, 15, 80, 0)
	{
		registerListener(NEW_BASENODE_EVENT);
		registerListener(DELETE_BASENODE_EVENT);
		registerListener(GUI_EVENT);

		updateButtons();
	};

	BaseNode* getBaseNode() { return m_pBaseNode; }
};