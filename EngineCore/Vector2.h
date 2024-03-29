#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector2
{
	float X;
	float Y;

	Vector2() :
		Y(0.0),
		X(0.0) {};

	Vector2(float x, float y) :
		Y(y),
		X(x) {};

	Vector2(float rad) :
		Y(sin(rad)),
		X(cos(rad)) {};


	Vector2 mirror(Vector2 p)
	{
		Vector2 v(X - p.X, Y - p.Y);
		Vector2 mirror(-v.X + p.X,  -v.Y + p.Y);
		return mirror;
	}

	Vector2 left()
	{
		return Vector2(-Y, X);
	}

	Vector2 right()
	{
		return Vector2(Y, -X);
	}
	
	float magnitude()
	{
		return sqrt((Y * Y) + (X * X));
	}

	Vector2 normalize()
	{
		float fMagnitude = magnitude();
		if (fMagnitude != 0)
			return Vector2((X / fMagnitude), (Y / fMagnitude));
		else
			return (0, 0);
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
		return "[" + thingToString<float>(X) + ", " + thingToString<float>(Y) + "]";
	}

	Vector2 limit(float max)
	{
		float current = magnitude();
		if (current > max)
		{
			float limit = max / current;
			return Vector2(X * limit, Y * limit) ;
		}
		return Vector2(X, Y);
	}

	void floor(float min)
	{
		if (magnitude() < min)
		{
			X = X * 0;
			Y = Y * 0;
		}
	}

	void clamp(float min, float max)
	{
		if (magnitude() > max)
			limit(max);

		if (magnitude() < min)
			limit(min);
	}

	void add(Vector2 A)
	{
		X = X + A.X;
		Y = Y + A.Y;
	}

	void sub(Vector2 A)
	{
		X = X - A.X;
		Y = Y - A.Y;
	}

	void mult(float f)
	{
		X = X * f;
		Y = Y * f;
	}

	void div(float f)
	{
		X = X / f;
		Y = Y / f;
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

Vector2 operator-(Vector2 A)
{
	Vector2 Subtract(-A.X, -A.Y);
	return Subtract;
}

Vector2 operator*(Vector2 A, Vector2 B)
{
	return Vector2(A.X * B.X, A.Y * B.Y);
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
	return Vector2(-(B.Y - A.Y), B.X - A.X );
}

float dotProduct(Vector2 A, Vector2 B)
{
	return -((A.Y * B.Y) + (A.X * B.X));
}

float hypotenuse(Vector2& A, Vector2& B)
{
	float hypo = sqrt((abs(A.X - B.X) * abs(A.X - B.X)) + (abs(A.Y - B.Y) * abs(A.Y - B.Y)));
	return hypo;
}

float distance(Vector2& A, Vector2& B)
{
	return hypotenuse(A, B);
}

float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

Vector2 lerp(Vector2 a, Vector2 b, float t)
{  
	return Vector2(lerp(a.X, b.X, t), lerp(a.Y, b.Y, t));
}

Vector2 reflect(Vector2 a, Vector2 n)
{
	//	n = n.normalize();
	Vector2 reflect = n * (2 * dotProduct(a, n)) + a;
	return reflect;
}
#endif