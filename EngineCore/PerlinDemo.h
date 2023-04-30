class PerlinDemo : public GameState, Perlin
{
private:
	Transform3D* pCamera;

	DefaultTileMap* pMap;
	int nOutputHeight = 128;
	int nOutputWidth = 128;
	int nOutputSize = 125;
	int nOctaveCount = 1;
	float fScaleBias = 1.0;
	
	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		/*if (!pEvent->isKeyDown())
		switch (pEvent->getKey())
		{
			case 32:
			{
				nOctaveCount++;
				if (nOctaveCount > 8)
					nOctaveCount = 1;

				PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScaleBias, fPerlinNoise2D);
				pMap->initialize(fPerlinNoise2D, nOutputWidth, nOutputHeight);
			} break;
			

			case 'r':
			{
				newSeed2D(nOutputWidth, nOutputHeight, nOutputSize);
				PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScaleBias, fPerlinNoise2D);
				pMap->initialize(fPerlinNoise2D, nOutputWidth, nOutputHeight);
			} break;

			case ',' :
			{
				fScaleBias -= 0.2f;
				if (fScaleBias < 0.2f) fScaleBias = 0.2f;
				PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScaleBias, fPerlinNoise2D);
				pMap->initialize(fPerlinNoise2D, nOutputWidth, nOutputHeight);
			} break;

			case '.':
			{
				fScaleBias += 0.2f;
				PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScaleBias, fPerlinNoise2D);
				pMap->initialize(fPerlinNoise2D, nOutputWidth, nOutputHeight);
			} break;
			
		}*/
	}

	void onMouseEvent(MouseEvent* pEvent)
	{
		/*if (pEvent->getState().bWheeledDown || pEvent->getState().bWheeledUp)
		{
			nOctaveCount = pCamera->getPosition().Z / 4;
			PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScaleBias, fPerlinNoise2D);
		}*/

	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
			case CONSOLE_MOUSE_EVENT: onMouseEvent(pEvent->get<MouseEvent>());
				break;
		}
	}

public:
	PerlinDemo()
	{
		registerListener(KEYBOARD_EVENT);
		registerListener(CONSOLE_MOUSE_EVENT);
	}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		CameraViewWindow* pCameraWindow = new CameraViewWindow(200, 150, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pCamera = pCameraWindow->getCamera()->getChild<Transform3D>();
		pGUI->addChild(pCameraWindow);
		
		
		fNoiseSeed1D = new float[nOutputSize];
		fPerlinNoise1D = new float[nOutputSize];

		float* fNoiseSeed2D = new float[nOutputHeight * nOutputWidth];
		float* fPerlinNoise2D = new float[nOutputHeight * nOutputWidth];

		//	newSeed1D();
		//	PerlinNoise1D(nOutputSize, fNoiseSeed1D, nOctaveCount, fScaleBias, fPerlinNoise1D);

		newSeed2D(nOutputWidth, nOutputHeight, nOutputSize);
		PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScaleBias);

		pMap = new DefaultTileMap(nOutputWidth, nOutputHeight);
		pMap->initialize(fPerlinNoise2D, nOutputWidth, nOutputHeight);

		pData->add(pMap);
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D r(pEngineBuffer, pCamera->getPosition());

		//	1D noise
		/*for (int x = 0; x < nOutputSize; x++)
		{
			float fNoise = fPerlinNoise1D[x] * pCamera->getPosition().Z * 10;
			render.FillQuad((float)x, 20 - fNoise, (float)x + 1, 20, { PIXEL_SOLID, FG_WHITE });
		}*/

		// 2D noise
		/*Vector3 cam = pCamera->getPosition();
		for (int y = 0; y < nOutputHeight; y++)
			for (int x = 0; x < nOutputWidth; x++)
			{
				float noise = fPerlinNoise2D[y * nOutputWidth + x];
				Pixel p = r.getGreyscaleColor(noise);
				r.FillQuad(x, y, x + 1, y + 1, p);
			}		
		r.DrawString("Octave: " + thingToString<int>(nOctaveCount), 2, 2);
		r.DrawString("Scale: " + thingToString<float>(fScaleBias), 2, 3);*/
	}

};