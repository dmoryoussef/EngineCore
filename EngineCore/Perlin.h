class Perlin
{
protected:
	float* fNoiseSeed2D = NULL;
	float* fPerlinNoise2D = NULL;

	float* fNoiseSeed1D = NULL;
	float* fPerlinNoise1D = NULL;

public:
	Perlin() {};
	~Perlin()
	{
 		delete fNoiseSeed2D;
		delete fPerlinNoise2D;
	}

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

	float* PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias)
	{
		fPerlinNoise2D = new float[nWidth * nHeight];
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
				fPerlinNoise2D[y * nWidth + x] = fNoise / fScaleAcc;
			}
		return fPerlinNoise2D;
	}

	float* PerlinNoise2D(int minX, int minY, int maxX, int maxY, float* seed, int octaves, float bias)
	{
		int width = maxX - minX;
		int height = maxY - minY;
		fPerlinNoise2D = new float[width * height];
		for (int x = minX; x < maxX; x++)
			for (int y = minY; y < maxY; y++)
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
				fPerlinNoise2D[y * width + x] = fNoise / fScaleAcc;
			}

		return fPerlinNoise2D;
	}
	
	float* newSeed2D(int w, int h, int range)
	{
		fNoiseSeed2D = new float[w * h];
		for (int i = 0; i < w * h; i++) fNoiseSeed2D[i] = (float)random(float(0), float(range)) / range;
		return fNoiseSeed2D;
	}

	void newSeed1D(int range)
	{
		for (int i = 0; i < range; i++) fNoiseSeed1D[i] = (float)random(float(0), float(range)) / range;
	}
};
