class UILayout :
	public _Component
{
private:
	int* m_nLayout;

	int m_nHeight;
	int m_nWidth;
	int m_nCellWidth;
	int m_nCellHeight;

	int m_nCellWidthBuffer;
	int m_nCellHeightBuffer;

public:
	UILayout(int nWidth, int nHeight, int nCellWidthBuffer, int nCellHeightBuffer) :
		m_nLayout(new int[nHeight * nWidth]),
		m_nHeight(nHeight),
		m_nWidth(nWidth),
		m_nCellHeightBuffer(nCellHeightBuffer),
		m_nCellWidthBuffer(nCellWidthBuffer),
		_Component()
	{
		clearLayout();
	};

	void clearLayout()
	{
		for (int nY = 0; nY < m_nHeight; nY++)
			for (int nX = 0; nX < m_nWidth; nX++)
			{
				m_nLayout[nX + m_nWidth * nY] = 0;
			}

		clearList();
	}

	void updateLayout()
	{
		//	reset position
		//	get max cell size
		//	set new position


		/*

		while (isIterating())
		{
			int nY = Position.Y;
			itn nX = Position.X;
			_AComponent *pComponent = getCurrent<_AComponent>();

			pComponent->setPosition(nY / m_nCellHeight, nX / m_nCellWidth);

		}

		while (isIterating())
		{
			_AComponent *pComponent = getCurrent<_AComponent>();

			if (pComponent->getSize().X > m_nCellWidth)
				m_nCellWidth = pComponent->getSize().X;
			if (pComponent->getSize().Y > m_nCellHeight)
				m_nCellHeight = pComponent->getSize().Y;

		}
		int nY = 0;
		int nX = 0;
		while (isIterating())
		{
			_AComponent *pComponent = getCurrent<_AComponent>();

			int nY = pComponent->getPosition().Y / m_nCellHeight;
			int nX = pComponent->getPosition().X / m_nCellWidth;

			pComponent->setPosition(nY * m_nCellHeight, nX * m_nCellWidth);
		}*/
	}

	//void addComponent(_Component* pComponent, int nX, int nY)
	//{
	//	if (nY < m_nHeight && nX < m_nWidth)
	//		if (m_nLayout[nX + m_nWidth * nY] == 0)
	//		{
	//			if (pComponent->getSize().X > m_nCellWidth)
	//				m_nCellWidth = m_nCellWidthBuffer + pComponent->getSize().X;
	//			if (pComponent->getSize().Y > m_nCellHeight)
	//				m_nCellHeight = m_nCellHeightBuffer + pComponent->getSize().Y;

	//			pComponent->setPosition(nY * m_nCellHeight, nX * m_nCellWidth);

	//			m_nLayout[nX + m_nWidth * nY] == 1;

	//			addChild(pComponent);
	//		}

	//	setSize(m_nWidth * m_nCellWidth, m_nHeight * m_nCellHeight);
	//}
};