#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector2
{
	float Y;
	float X;

	Vector2() :
		Y(0.0),
		X(0.0) {};

	Vector2(float x, float y) :
		Y(y),
		X(x) {};

	Vector2(float mag) :
		Y(sin(mag)),
		X(cos(mag)) {};


	float magnitude()
	{
		return sqrt((Y * Y) + (X * X));
	}

	Vector2 normalize()
	{
		float fMagnitude = magnitude();
		return Vector2((X / fMagnitude), (Y / fMagnitude));
	}

	Vector2 trunc()
	{
		int nY = int(Y);
		int nX = int(X);
		return Vector2(nX, nY);
	}

	float getAngle()
	{
		return atan2(X, Y);
	}

	template <typename T>
	string toString()
	{
		return "[" + thingToString<T>(X) + ", " + thingToString<T>(Y) + "]";
	}

	string toString()
	{
		//	default int, if you want float, call the above funtion and specify float
		return "[" + thingToString<int>(X) + ", " + thingToString<int>(Y) + "]";
	}
};

//Vector2 Min(Vector2 A, Vector2 B)
//{
//	if (A < B)
//		return A;
//	else
//		return B;
//}
//
//Vector2 Max(Vector2 A, Vector2 B)
//{
//	if (A > B)
//		return A;
//	else
//		return B;
//}

bool operator!=(Vector2 A, Vector2 B)
{
	if (A.X != B.X)
		return true;

	if (A.Y != B.Y)
		return true;

	return false;
}

bool operator==(Vector2 A, Vector2 B)
{
	if (A.X == B.X && A.Y == B.Y)
		return true;
	else
		return false;
}

Vector2 operator+(Vector2 A, Vector2 B)
{
	Vector2 Add(A.X + B.X, B.Y + A.Y);
	return Add;
}

Vector2 operator-(Vector2 A, Vector2 B)
{
	Vector2 Subtract(A.X - B.X, A.Y - B.Y);
	return Subtract;
}

Vector2 operator*(Vector2 A, float fMultiplier)
{
	return Vector2(A.X * fMultiplier, A.Y * fMultiplier);
}

Vector2 operator/(Vector2 A, float fMultiplier)
{
	return Vector2(A.X / fMultiplier, A.Y / fMultiplier);
}

bool operator>(Vector2 A, Vector2 B)
{
	if (A.Y > B.Y &&
		A.X > B.X)
		return true;
	else
		return false;
}

bool operator<(Vector2 A, Vector2 B)
{
	if (A.Y < B.Y &&
		A.X < B.X)
		return true;
	else
		return false;
}

bool operator<=(Vector2 A, Vector2 B)
{
	return A.X <= B.X && A.Y <= B.Y;
}

bool operator>=(Vector2 A, Vector2 B)
{
	return A.X >= B.X && A.Y >= B.Y;
}

Vector2 getMax(Vector2 A, Vector2 B)
{
	if (A.Y > B.Y)
	{
		if (A.X > B.X)
			return A;
		else
			return B;
	}
	else
	{
		if (A.X > B.X)
			return A;
		else
			return B;
	}

}

Vector2 getMin(Vector2 A, Vector2 B)
{
	if (A.Y < B.Y)
	{
		if (A.X < B.X)
			return A;
		else
			return B;
	}
	else
	{
		if (A.X < B.X)
			return A;
		else
			return B;
	}

}

Vector2 getNormal(Vector2 &A, Vector2& B)
{
	return Vector2(- (B.Y - A.Y), B.X - A.X );
}

float dotProduct(Vector2 A, Vector2 B)
{
	return (A.Y * B.Y) + (A.X * B.X);
}

float hypotenuse(Vector2& A, Vector2& B)
{
	float hypo = sqrt((abs(A.X - B.X) * abs(A.X - B.X)) + (abs(A.Y - B.Y) * abs(A.Y - B.Y)));
	return hypo;
}

#endif