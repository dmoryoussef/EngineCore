class ListExplorerWindow : public UIWindow
{
	class BaseNodeButton : public UIButton
	{
		private:
			BaseNode* m_pParentWindow;
			BaseNode* m_pBaseNode;

			void onStateChange()
			{
				addEvent(new GuiEvent(this));
				if (UIState* pUIState = m_pBaseNode->getChild<UIState>())
					pUIState->setState(m_nState);
			}

		public:
			BaseNodeButton(BaseNode* pBaseNode) :
				m_pBaseNode(pBaseNode),
				m_pParentWindow(NULL),
				UIButton(pBaseNode->getName()) {};

			BaseNode* getParentWindow()
			{
				return m_pParentWindow;
			}
			BaseNode* getBaseNode()
			{
				return m_pBaseNode;
			}
	};

	class BaseNodeComboButton
	{
		// only one can be selected at a time
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
					//		get state
						switch (pButton->getState())
						{
							case LEFT_RELEASED :
							{
								ListExplorerWindow* pWindow = new ListExplorerWindow(pButton->getBaseNode());
								pWindow->setAlignment(ALIGN_OUTSIDE_LEFT);
								addComponent(pWindow);
								break;
							}
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
		UIWindow(20, 15, 75, 10)
	{
		registerListener(NEW_BASENODE_EVENT);
		registerListener(GUI_EVENT);
		updateButtons();
	};

	BaseNode* getBaseNode() { return m_pBaseNode; }
};