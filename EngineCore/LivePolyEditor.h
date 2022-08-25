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

	vector<Side*> Sides;
	vector<Vertice*> Verts;

	Side* ActiveSide;
	Vertice* ActiveVert;
	Vector2 vPrevMouse;

	Side* top;
	Side* left;
	Side* bottom;
	Side* right;

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

		if (bButton)
		{
			if (ActiveSide)
			{
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
			}
		}

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
				Vector2 mouseDiff = pos - vPrevMouse;
				for (auto v : Verts)
				{
					v->position = (v->position + mouseDiff);
				}
			}
		}
	}

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		Vector2 mouse = pEvent->getWorldPosition();

		onMouseOverVerts(mouse, pEvent->getState().bLeftButtonDown);
		onMouseOverSides(mouse, pEvent->getState().bLeftButtonDown);
		onMouseOverPoly(mouse, pEvent->getState().bRightButtonDown);

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

	void render(Render2D* renderer)
	{
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

class EditablePoly2DGroup : public EventListener
{
private:
	vector<EditablePoly2D*> Polys;
	EditablePoly2D* ActivePoly;

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{

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
	EditablePoly2DGroup()
	{
		registerListener(MOUSEWORLD_EVENT);
	};
};

class LivePolyEditor : public GameState
{
private:
	SelectionSquare* selectionSquare;
	Transform3D* pCameraTransform;
	bool bSelectionSquareHotkey;

	vector<EditablePoly2D*> Polys;

	void onSelectionSquareEvent(SelectionSquareEvent* pEvent)
	{
		if (!pEvent->isHovering())
			Polys.push_back(new EditablePoly2D(pEvent->getMin(), pEvent->getMax()));

		if (!pEvent->isHovering())
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
				selectionSquare = new SelectionSquare(pos);
		}
	}
	
	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		bSelectionSquareHotkey = pEvent->isKeyDown('p');
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
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
		selectionSquare(NULL)
	{
		registerListener(KEYBOARD_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
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

		pData->add(new BuildingMap(20, 20));
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());

		for (auto p : Polys) p->render(&renderer);

		if (selectionSquare) selectionSquare->render(&renderer);
	}
};