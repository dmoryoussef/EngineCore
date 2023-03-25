class PerlinDemo : public GameState
{
private:
	Transform3D* pCamera;

	float* fNoiseSeed1D = NULL;
	float* fPerlinNoise1D = NULL;
	int nOutputSize = 125;
	int nOctaveCount = 1;
	float fScaleBias = 1.0;

	int nOutputHeight = 128;
	int nOutputWidth = 128;
	float* fNoiseSeed2D = NULL;
	float* fPerlinNoise2D = NULL;

	DefaultTileMap* pMap;

	void PerlinNoise1D(int nSize, float* fSeed, int nOctaves, float fBias, float* fOutput)
	{
		for (int x = 0; x < nSize; x++)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAccum = 0.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				int nPitch = nSize >> o;
				if (nPitch == 0) nPitch = 1;

				int nSampleA = (x / nPitch) * nPitch;
				int nSampleB = (nSampleA + nPitch) % nSize;

				float fBlend = (float)(x - nSampleA) / (float)nPitch;
				float fSample = lerp(fSeed[nSampleA], fSeed[nSampleB], fBlend);
				
				fScaleAccum += fScale;
				fNoise += fSample * fScale;
				fScale = fScale / fBias;
			}
				

			fOutput[x] = fNoise / fScaleAccum;
		}
	}

	void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput)
	{
		for (int x = 0; x < nWidth; x++)
			for (int y = 0; y < nHeight; y++)
			{
				float fNoise = 0.0f;
				float fScaleAcc = 0.0f;
				float fScale = 1.0f;

				for (int o = 0; o < nOctaves; o++)
				{
					int nPitch = nWidth >> o;
					if (nPitch == 0) nPitch = 1;

					int nSampleX1 = (x / nPitch) * nPitch;
					int nSampleY1 = (y / nPitch) * nPitch;

					int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
					int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

					float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
					float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

					float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
					float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

					fScaleAcc += fScale;
					fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
					fScale = fScale / fBias;
				}

				// Scale to seed range
				fOutput[y * nWidth + x] = fNoise / fScaleAcc;
			}
	}

	void PerlinNoise2D(int worldX, int worldY, int width, int height, float* seed, int octaves, float bias, float* output)
	{
		for (int x = worldX; x < width; x++)
			for (int y = worldY; y < height; y++)
			{
				float fNoise = 0.0f;
				float fScaleAcc = 0.0f;
				float fScale = 1.0f;

				for (int o = 0; o < octaves; o++)
				{
					int nPitch = width >> o;
					if (nPitch == 0) nPitch = 1;

					int nSampleX1 = (x / nPitch) * nPitch;
					int nSampleY1 = (y / nPitch) * nPitch;

					int nSampleX2 = (nSampleX1 + nPitch) % width;
					int nSampleY2 = (nSampleY1 + nPitch) % width;

					float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
					float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

					float fSampleT = (1.0f - fBlendX) * seed[nSampleY1 * width + nSampleX1] + fBlendX * seed[nSampleY1 * width + nSampleX2];
					float fSampleB = (1.0f - fBlendX) * seed[nSampleY2 * width + nSampleX1] + fBlendX * seed[nSampleY2 * width + nSampleX2];

					fScaleAcc += fScale;
					fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
					fScale = fScale / bias;
				}

				// Scale to seed range
				output[y * width + x] = fNoise / fScaleAcc;
			}
	}

	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		if (!pEvent->isKeyDown())
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
				newSeed2D();
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
			
		}
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

		fNoiseSeed2D = new float[nOutputHeight * nOutputWidth];
		fPerlinNoise2D = new float[nOutputHeight * nOutputWidth];

		//	newSeed1D();
		//	PerlinNoise1D(nOutputSize, fNoiseSeed1D, nOctaveCount, fScaleBias, fPerlinNoise1D);

		newSeed2D();
		PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScaleBias, fPerlinNoise2D);

		pMap = new DefaultTileMap(nOutputWidth, nOutputHeight);
		pMap->initialize(fPerlinNoise2D, nOutputWidth, nOutputHeight);

		pData->add(pMap);
	}

	void newSeed2D()
	{
		for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)random(float(0), float(nOutputSize)) / nOutputSize;
	}

	void newSeed1D()
	{
		for (int i = 0; i < nOutputSize; i++) fNoiseSeed1D[i] = (float)random(float(0), float(nOutputSize)) / nOutputSize;
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