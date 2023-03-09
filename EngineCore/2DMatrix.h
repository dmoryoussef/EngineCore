struct mat3x3
{
	float m[3][3] = { 0 };

	mat3x3 Identity()
	{
		mat3x3 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		return matrix;
	}

	mat3x3 RotateZ(float fAngleRad)
	{
		mat3x3 matRotate;
		matRotate.m[0][0] = cosf(fAngleRad);
		matRotate.m[0][1] = -sinf(fAngleRad);
		matRotate.m[0][2] = 0;
		matRotate.m[1][0] = sinf(fAngleRad);
		matRotate.m[1][1] = cosf(fAngleRad);
		matRotate.m[1][2] = 0;
		matRotate.m[2][0] = 0;
		matRotate.m[2][1] = 0;
		matRotate.m[2][2] = 1.0f;

		return matRotate;
	}

	mat3x3 RotateX(float fAngleRad)
	{
		mat3x3 matRotate;
		matRotate.m[0][0] = 1;
		matRotate.m[1][1] = cosf(fAngleRad);
		matRotate.m[1][2] = sinf(fAngleRad);
		matRotate.m[2][1] = -sinf(fAngleRad);
		matRotate.m[2][2] = cosf(fAngleRad);

		return matRotate;
	}

	mat3x3 RotateY(float fAngleRad)
	{
		mat3x3 matRotate;
		matRotate.m[0][0] = cosf(fAngleRad);
		matRotate.m[0][2] = sinf(fAngleRad);
		matRotate.m[2][0] = -sinf(fAngleRad);
		matRotate.m[1][1] = 1.0f;
		matRotate.m[2][2] = cosf(fAngleRad);
		return matRotate;
	}

	mat3x3 Scale(Vector2 scale)
	{
		mat3x3 matScale;
		matScale.m[0][0] = scale.X;
		matScale.m[1][1] = scale.Y;
		matScale.m[2][2] = 1;
		return matScale;
	}

	mat3x3 Translate(Vector2 transl)
	{
		mat3x3 matTranslate;
		matTranslate.m[0][0] = 1.0;
		matTranslate.m[1][1] = 1.0;
		matTranslate.m[2][2] = 1.0;
		matTranslate.m[2][0] = transl.X;
		matTranslate.m[2][1] = transl.Y;
		return matTranslate;
	}

};

Vector2 operator*(Vector2 &vec, mat3x3 &mat)
{
	Vector2 out;
	float z = 1;
	out.X = vec.X * mat.m[0][0] + vec.Y * mat.m[1][0] + z * mat.m[2][0];
	out.Y = vec.X * mat.m[0][1] + vec.Y * mat.m[1][1] + z * mat.m[2][1];

	return out;
}


Triangle2D operator*(Triangle2D& tri, mat3x3& mat)
{
	Triangle2D out(tri.A * mat, tri.B * mat, tri.C * mat);
	return out;
}

mat3x3 operator*(mat3x3& m1, mat3x3& m2)
{
	mat3x3 matrix;

	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
			matrix.m[x][y] = m1.m[x][0] * m2.m[0][y] +
							 m1.m[x][1] * m2.m[1][y] +
							 m1.m[x][2] * m2.m[2][y];

	return matrix;
}