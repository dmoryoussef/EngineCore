class EditableRoom2D : public EditablePoly2D
{
private:
	int* Tiles;
	int height;
	int width;
	int PosX;
	int PosY;


public:
	EditableRoom2D(Vector2 min, Vector2 max) :
		EditablePoly2D(min, max)
	{
		updatePosition({ 0, 0 });
	};

	Vector2 getOutsideMin()
	{
		return Vector2(PosX, PosY);
	}

	Vector2 getOutsideMax()
	{
		return Vector2(PosX + width, PosY + height);
	}

	void updatePosition(Vector2 mouseDiff)
	{
		EditablePoly2D::updatePosition(mouseDiff);

		PosX = getMin().X;
		PosY = getMin().Y;
		int maxX = (int)getMax().X + 1;
		int maxY = (int)getMax().Y + 1;
		height = maxY - PosY;
		width = maxX - PosX;
	}

	void render(Render2D* r)
	{
		//	outline
		Vector2 start = getOutsideMin();
		Vector2 end = getOutsideMax();
		r->DrawLine(start, { end.X, start.Y }, { PIXEL_SOLID, FG_LIGHTGREY });
		r->DrawLine(start, { start.X, end.Y }, { PIXEL_SOLID, FG_LIGHTGREY });
		r->DrawLine({ end.X, start.Y }, end, { PIXEL_SOLID, FG_LIGHTGREY });
		r->DrawLine({ start.X, end.Y }, end, { PIXEL_SOLID, FG_LIGHTGREY });

		EditablePoly2D::render(r);

		for (auto e : Edges)
		{
			Pixel color = { PIXEL_HALF, FG_LIGHTRED };
			EditableRoom2D* s = dynamic_cast<EditableRoom2D*>(e);
			Vector2 aMin = getOutsideMin();
			Vector2 aMax = getOutsideMax();
			Vector2 bMin = s->getOutsideMin();
			Vector2 bMax = s->getOutsideMax();

			if (isQuadvQuad(aMin, aMax, bMin, bMax) || isQuadvQuad(bMin, bMax, aMin, aMax))
				color = { PIXEL_HALF, FG_LIGHTGREEN };
			r->DrawLine(getMin() + (getSize() / 2), e->getMin() + (e->getSize() / 2), color);
		}

		vector<string> data;
		data.push_back("Editable Room");
		data.push_back(thingToString<int>(width) + "x" + thingToString<int>(height));
		r->DrawString(data, getMin().X + 1, getMin().Y + 1);
	}
};