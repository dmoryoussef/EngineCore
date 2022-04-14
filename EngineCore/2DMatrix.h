struct mat3x3
{
	float m[3][3] = { 0 };

	mat3x3 Rotate(float fAngleRad)
	{
		mat3x3 matRotate;
		matRotate.m[0][0] = cosf(fAngleRad);
		matRotate.m[0][1] = sinf(fAngleRad);
		matRotate.m[1][0] = -sinf(fAngleRad);
		matRotate.m[1][1] = cosf(fAngleRad);
		return matRotate;
	}

	mat3x3 Scale(Vector2 scale)
	{
		mat3x3 matScale;
		matScale.m[0][0] = scale.X;
		matScale.m[1][1] = scale.Y;
		return matScale;
	}

	mat3x3 Translate(Vector2 transl)
	{
		mat3x3 matTranslate;
		matTranslate.m[0][0] = 1;
		matTranslate.m[1][1] = 1;
		matTranslate.m[2][2] = 1;
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

	for (int c = 0; c < 3; c++)
		for (int r = 0; r < 3; r++)
			matrix.m[r][c] = 
				m1.m[r][0] * m2.m[0][c] +
				m1.m[r][1] * m2.m[1][c] +
				m1.m[r][2] * m2.m[2][c];

	return matrix;
}