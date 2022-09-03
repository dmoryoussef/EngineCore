bool isPointvCircle(Vector2 p, Vector2 cir, float rad, float er = 0)
{
	return (hypotenuse(p, cir) <= rad + er);
}

bool isPointvQuad(Vector2 p, Vector2 min, Vector2 max)
{
	return (min <= p && max >= p);
}

bool isPointvLine(Vector2 p, Vector2 a, Vector2 b, float er = 0)
{
	float pa = hypotenuse(a, p);
	float pb = hypotenuse(b, p);
	return ((pa + pb) - hypotenuse(a, b) <= er);
}

bool isLinevLine()
{
	return false;
}

bool isQuadvQuad(Vector2 minA, Vector2 maxA, Vector2 minB, Vector2 maxB)
{
	return (isPointvQuad(minA, minB, maxB) || isPointvQuad(maxA, minB, maxB) || isPointvQuad({ maxA.X, minA.Y }, minB, maxB) || isPointvQuad({ minA.X, maxA.Y }, minB, maxB));
}

class EditablePoly2D : public EventListener
{
private:
	struct Vertice
	{
		Vector2 position;
		bool mouseOver;

		Vertice(Vector2 p)
		{
			position = p;
			mouseOver = false;
		}
	};
	
	struct Side
	{
		Vertice* A;
		Vertice* B;
		bool mouseOver;

		Side(Vertice* a, Vertice* b)
		{
			A = a;
			B = b;
			mouseOver = false;
		}

	};

	bool m_bMouseOver;
	bool m_bOverlap;

	vector<Side*> Sides;
	vector<Vertice*> Verts;

	Side* ActiveSide;
	Vertice* ActiveVert;
	Vector2 vPrevMouse;

	Side* top;
	Side* left;
	Side* bottom;
	Side* right;

	Vector2 vPreviousMin;
	Vector2 vPreviousMax;

	vector<EditablePoly2D*> Edges; 

	void onMouseOverVerts(Vector2 pos, bool bButton)
	{
		for (auto v : Verts)
		{
			v->mouseOver = false;
			if (isPointvCircle(pos, v->position, 1.0f))
			{
				v->mouseOver = true;
				if (!ActiveVert)
					ActiveVert = v;
			}
		}

		if (ActiveVert && !bButton)
		{
			ActiveVert = NULL;
		}
	}

	void onMouseOverSides(Vector2 pos, bool bButton)
	{
		Vector2 diffMouse = pos - vPrevMouse;
		for (auto s : Sides)
		{
			s->mouseOver = false;
			if (isPointvLine(pos, s->A->position, s->B->position, 0.01f))
			{
				s->mouseOver = true;
				if (!ActiveSide)
					ActiveSide = s;
			}
		}

		/*if (bButton)
		{
			if (ActiveSide)
			{
				vPreviousMin = getMin();
				vPreviousMax = getMax();

				if (ActiveSide == top || ActiveSide == bottom)
				{
					ActiveSide->A->position.Y = ActiveSide->A->position.Y + diffMouse.Y;
					ActiveSide->B->position.Y = ActiveSide->B->position.Y + diffMouse.Y;
				}
				else
				{
					ActiveSide->A->position.X = ActiveSide->A->position.X + diffMouse.X;
					ActiveSide->B->position.X = ActiveSide->B->position.X + diffMouse.X;
				}

				if ((int)vPreviousMin.X != (int)getMin().X || (int)vPreviousMin.Y != (int)getMin().Y ||
					(int)vPreviousMax.X != (int)getMax().X || (int)vPreviousMax.Y != (int)getMax().Y)
					addEvent(new EditorObjectEvent(vPreviousMin, vPreviousMax, getMin(), getMax()));
				
			}
		}*/

		if (ActiveSide && !bButton)
		{
			ActiveSide = NULL;
		}
	}

	void onMouseOverPoly(Vector2 pos, bool bButton)
	{
		m_bMouseOver = isPointvQuad(pos, getMin(), getMax());
		
		if (!ActiveVert && bButton)
		{
			if (m_bMouseOver)
			{
				vPreviousMin = getMin();
				vPreviousMax = getMax();

				Vector2 mouseDiff = pos - vPrevMouse;
				for (auto v : Verts)
				{
					v->position = (v->position + mouseDiff);
				}

				if (moved())
					addEvent(new EditorObjectEvent(vPreviousMin, vPreviousMax, getMin(), getMax()));
			}
		}
	}


	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		Vector2 mouse = pEvent->getWorldPosition();

		//	onMouseOverVerts(mouse, pEvent->getState().bLeftButtonDown);
		onMouseOverSides(mouse, pEvent->getState().bLeftButtonDown);
		//	onMouseOverPoly(mouse, pEvent->getState().bRightButtonDown);

		vPrevMouse = mouse;
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}

public:
	EditablePoly2D(Vector2 min, Vector2 max) :
		m_bOverlap(false),
		m_bMouseOver(false),
		ActiveSide(NULL),
		ActiveVert(NULL)
	{
		registerListener(MOUSEWORLD_EVENT);

		Vertice *topLeft = new Vertice(min);
		Vertice *topRight = new Vertice({ max.X, min.Y });
		Vertice *bottomRight = new Vertice(max);
		Vertice *bottomLeft = new Vertice({ min.X, max.Y });

		Verts.push_back(topLeft);
		Verts.push_back(topRight);
		Verts.push_back(bottomLeft);
		Verts.push_back(bottomRight);
		
		top = new Side(topLeft, topRight);
		left = new Side(topLeft, bottomLeft);
		bottom = new Side(bottomLeft, bottomRight);
		right = new Side(topRight, bottomRight);

		Sides.push_back(top);
		Sides.push_back(left);
		Sides.push_back(bottom);
		Sides.push_back(right);
	};

	~EditablePoly2D()
	{
		for (auto s : Sides)
		{
			delete s;
		}

		Sides.clear();

		for (auto v : Verts)
		{
			delete v;
		}
		Verts.clear();
	}

	Vector2 getMin()
	{
		if (top->A->position < bottom->B->position)
			return (top->A->position);
		else
			return bottom->A->position;
	}
	Vector2 getMax()
	{
		if (top->A->position < bottom->B->position)
			return (bottom->B->position);
		else
			return (top->B->position);
	}
	
	Vector2 getPrevMin() { return vPreviousMin; }
	Vector2 getPrevMax() { return vPreviousMax; }

	void setOverlap(bool overlap) { m_bOverlap = overlap; }
	void setMouseOver(bool mouseOver)
	{
		m_bMouseOver = mouseOver;
	}

	void updateSides(Vector2 mouse, bool button)
	{
		Vector2 mouseDiff = mouse - vPrevMouse;

		if (button)
		{
			if (ActiveSide)
			{
				vPreviousMin = getMin();
				vPreviousMax = getMax();

				if (ActiveSide == top || ActiveSide == bottom)
				{
					ActiveSide->A->position.Y = ActiveSide->A->position.Y + mouseDiff.Y;
					ActiveSide->B->position.Y = ActiveSide->B->position.Y + mouseDiff.Y;
				}
				else
				{
					ActiveSide->A->position.X = ActiveSide->A->position.X + mouseDiff.X;
					ActiveSide->B->position.X = ActiveSide->B->position.X + mouseDiff.X;
				}

				if ((int)vPreviousMin.X != (int)getMin().X || (int)vPreviousMin.Y != (int)getMin().Y ||
					(int)vPreviousMax.X != (int)getMax().X || (int)vPreviousMax.Y != (int)getMax().Y)
					addEvent(new EditorObjectEvent(vPreviousMin, vPreviousMax, getMin(), getMax()));

			}
		}
	}

	void updatePosition(Vector2 mouseDiff)
	{
		vPreviousMin = getMin();
		vPreviousMax = getMax();

		for (auto v : Verts)
		{
			v->position = (v->position + mouseDiff);
		}
	}

	void addEdge(EditablePoly2D* edge)
	{
		Edges.push_back(edge);
	}

	bool moved()
	{
		return ((int)vPreviousMin.X != (int)getMin().X || (int)vPreviousMin.Y != (int)getMin().Y ||
			(int)vPreviousMax.X != (int)getMax().X || (int)vPreviousMax.Y != (int)getMax().Y);
	}

	bool isMouseOver()
	{
		return m_bMouseOver;
	}
	bool isOverlap() { return m_bOverlap; }

	void render(Render2D* renderer)
	{
		for (auto e : Edges)
		{
			renderer->DrawLine(getMin(), e->getMax(), { PIXEL_SOLID, FG_LIGHTBLUE });
		}

		for (auto s : Sides)
		{
			if (s->mouseOver)
				renderer->DrawLine(s->A->position, s->B->position, { PIXEL_SOLID, FG_DARKGREEN });
			else
			{
				if (m_bMouseOver)
					renderer->DrawLine(s->A->position, s->B->position, { PIXEL_SOLID, FG_LIGHTBLUE });
				else
					renderer->DrawLine(s->A->position, s->B->position, { PIXEL_SOLID, FG_DARKBLUE });
			}

			
			if (m_bOverlap)
				renderer->DrawLine(s->A->position, s->B->position, { PIXEL_SOLID, FG_YELLOW });
			
			
		}

		for (auto v : Verts)
		{
			if (v->mouseOver)
				renderer->DrawCircle(v->position.X, v->position.Y, 2.0f, { PIXEL_SOLID, FG_LIGHTBLUE });
			else
				renderer->DrawCircle(v->position.X, v->position.Y, 2.0f, { PIXEL_SOLID, FG_DARKBLUE });
		}

		if (ActiveSide)
			renderer->DrawLine(ActiveSide->A->position, ActiveSide->B->position, { PIXEL_SOLID, FG_LIGHTGREEN });

		if (ActiveVert)
			renderer->DrawCircle(ActiveVert->position.X, ActiveVert->position.Y, 2.0f, { PIXEL_SOLID, FG_LIGHTGREEN });


	}
};


class PolyList : public EventListener
{
private:
	vector<EditablePoly2D*> Polys;
	EditablePoly2D* currentPoly;
	Vector2 vPrevMouse;

	void onSelectionLineEvent(SelectionLineEvent* pEvent)
	{
		//	if start is over a poly
		//		set poly to "selected color" for visual feedback
		EditablePoly2D* start = NULL;
		EditablePoly2D* end = NULL;
		for (auto p : Polys)
		{
			if (isPointvQuad(pEvent->getStart(), p->getMin(), p->getMax()))
			{
				p->setMouseOver(true);
				start = p;
			}
		}

		// if end of over a poly
		//	if poly is not equal to start poly
		//	 set end poly to different color for visual feedback
		for (auto p : Polys)
		{
			if (isPointvQuad(pEvent->getStop(), p->getMin(), p->getMax()))
			{
				p->setMouseOver(true);
				end = p;
			}
		}
		
		//		if is released
		//			set edges
		if (pEvent->isReleased())
			if (start && end)
				if (start != end)
				{
					start->addEdge(end);
					end->addEdge(start);
				}

	}

	void onSelectionSquareEvent(SelectionSquareEvent* pEvent)
	{
		if (pEvent->isReleased())
		{
			addNewPoly(new EditablePoly2D(pEvent->getMin(), pEvent->getMax()));
			
			EditorObjectEvent* pObjEvent = new EditorObjectEvent();
			for (auto p : Polys)
			{
				pObjEvent->addObject(p->getPrevMin(), p->getPrevMax(), p->getMin(), p->getMax());
			}

			addEvent(pObjEvent);
		}
	}

	void onMouseOverPoly(Vector2 mouse, bool button)
	{
		for (auto p : Polys)
		{
			if (isPointvQuad(mouse, p->getMin(), p->getMax()))
			{
				p->setMouseOver(true);
				if (button && !currentPoly)
				{
					currentPoly = p;
				}
			}
			else
				p->setMouseOver(false);
		}

		if (!button)
			currentPoly = NULL;

		if (currentPoly)
		{
			currentPoly->updatePosition(mouse - vPrevMouse);
			//	if previous position != new position
			//	send event
			collisionTest();

			if (currentPoly->moved())
			{
				EditorObjectEvent* pEvent = new EditorObjectEvent();
				for (auto p : Polys)
				{
					pEvent->addObject(p->getPrevMin(), p->getPrevMax(), p->getMin(), p->getMax());

				}

				addEvent(pEvent);
			}
		}
	}

	void collisionTest()
	{
		for (auto p : Polys) p->setOverlap(false);

		for (int a = 0; a < Polys.size(); a++)
		{
			for (int b = a + 1; b < Polys.size(); b++)
			{
				if (isQuadvQuad(Polys[a]->getMin(), Polys[a]->getMax(), Polys[b]->getMin(), Polys[b]->getMax()))
				{
					Polys[a]->setOverlap(true);
					Polys[b]->setOverlap(true);
				}
			}
		}
	}

	void onMouseOverSides(Vector2 mouse, bool button)
	{

	}

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		Vector2 mouse = pEvent->getWorldPosition();

		//	onMouseOverVerts(mouse, pEvent->getState().bLeftButtonDown);
		
		//	onMouseOverSides(mouse, pEvent->getState().bLeftButtonDown);
		onMouseOverPoly(mouse, pEvent->getState().bRightButtonDown);

		vPrevMouse = mouse;
	}

	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		switch (pEvent->getKey())
		{
			case 0:	// delete poly
			{
				int i = 0;
				for (auto p : Polys)
				{
					if (p->isMouseOver())
					{
						auto erase = Polys.begin() + i;
						Polys.erase(erase);
						delete p;
					}
					i++;
				}
			}
			break;

			case 'w': // pull poly to the top
			{
				if (currentPoly)
				{
					int i = 0;
					for (auto p : Polys)
					{
						if (p == currentPoly)
							Polys.erase(Polys.begin() + i);
						i++;
					}
					Polys.push_back(currentPoly);

					EditorObjectEvent* pEvent = new EditorObjectEvent();
					for (auto p : Polys)
					{
						pEvent->addObject(p->getPrevMin(), p->getPrevMax(), p->getMin(), p->getMax());
					}

					addEvent(pEvent);
				}
			}
		}
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case SELECTIONLINE_EVENT: onSelectionLineEvent(pEvent->get<SelectionLineEvent>());
				break;
			case SELECTIONSQUARE_EVENT: onSelectionSquareEvent(pEvent->get<SelectionSquareEvent>());
				break;
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}

	void addNewPoly(EditablePoly2D *poly)
	{
		Polys.push_back(poly);
	}

public:
	PolyList() : 
		currentPoly(NULL),
		vPrevMouse()
	{
		registerListener(KEYBOARD_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
		registerListener(SELECTIONLINE_EVENT);
		registerListener(MOUSEWORLD_EVENT);
	}

	void render(Render2D* renderer)
	{
		for (auto p : Polys) p->render(renderer);
		renderer->DrawNum<int>(Polys.size(), 2, 2, FG_WHITE);
	}

};

class LivePolyEditor : public GameState
{
private:
	SelectionSquare* selectionSquare;
	Transform3D* pCameraTransform;
	bool bSelectionSquareHotkey;
	PolyList Polys;

	void onSelectionLineEvent(SelectionLineEvent* pEvent)
	{
		if (pEvent->isReleased())
		{
			delete selectionSquare;
			selectionSquare = NULL;
		}
	}

	void onSelectionSquareEvent(SelectionSquareEvent* pEvent)
	{
		if (pEvent->isReleased())
		{
			delete selectionSquare;
			selectionSquare = NULL;
		}
	}

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		Vector2 pos = pEvent->getWorldPosition();
		if (pEvent->getState().bLeftButtonDown && bSelectionSquareHotkey)
		{
			if (!selectionSquare)
				selectionSquare = new SelectionSquare(pos, false, true);
		}

		if (pEvent->getState().bLeftButtonDown)
		{
			if (!selectionSquare)
				selectionSquare = new SelectionSquare(pos, true, false);
		}
	}
	
	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		switch (pEvent->getKey())
		{
			case 'p': bSelectionSquareHotkey = pEvent->isKeyDown('p');
				break;
		}
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case SELECTIONLINE_EVENT: onSelectionLineEvent(pEvent->get<SelectionLineEvent>());
				break;
			case SELECTIONSQUARE_EVENT: onSelectionSquareEvent(pEvent->get<SelectionSquareEvent>());
				break;
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
		}
	}

public:
	LivePolyEditor() :
		bSelectionSquareHotkey(false),
		selectionSquare(NULL),
		Polys()
	{
		registerListener(KEYBOARD_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
		registerListener(SELECTIONLINE_EVENT);
		registerListener(MOUSEWORLD_EVENT);
	};


	void start(BaseNode * pData, BaseNode * pSystems, BaseNode * pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		pCameraTransform = pCamera->getChild<Transform3D>();
		pData->add(pCamera);
		pGUI->addAtEnd(pCameraWindow);



		pData->add(new BuildingMap(200, 100));
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());

		if (selectionSquare) selectionSquare->render(&renderer);

		Polys.render(&renderer);
	}
};