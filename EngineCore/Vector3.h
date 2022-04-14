#ifndef VECTOR3_H
#define VECTOR3_H

struct Vector3
{
	float X;
	float Y;
	float Z;
	float W;;

	Vector3() :
		X(0.0),
		Y(0.0),
		Z(0.0),
		W(1.0) {};

	Vector3(float x, float y, float z) :
		X(x),
		Y(y),
		Z(z),
		W(1.0) {};

	float magnitude()
	{
		return sqrt((Y * Y) + (X * X) + (Z * Z));
	}

	Vector3 trunc()
	{
		int nY = int(Y);
		int nX = int(X);
		int nZ = int(Z);
		return Vector3(nX, nY, nZ);
	}

	Vector3 toInt()
	{
		int nY = int(Y);
		int nX = int(X);
		int nZ = int(Z);
		Vector3 Int(nX, nY, nZ);
		return Int;
	}

	Vector3 normalized()
	{
		float l = magnitude();
		return Vector3(X /= l, Y /= l, Z /= l);
	}

	Vector2 toVec2()
	{
		return { X, Y };
	}

	string toString()
	{
		return "[" + thingToString<float>(X) + ", " + thingToString<float>(Y) + ", " + thingToString<float>(Z) + "]";
	}
};

Vector3 operator/(Vector3& A, float fScaler)
{
	return Vector3(A.X / fScaler, A.Y / fScaler, A.Z / fScaler);
}

Vector3 operator*(Vector3 A, float fScaler)
{
	Vector3 operation(A.X * fScaler, A.Y * fScaler, A.Z * fScaler);
	return operation;
}

Vector3 operator+(Vector3 A, Vector3 B)
{
	Vector3 operation(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
	return operation;
}

Vector3 operator-(Vector3 A)
{
	return Vector3(-A.X, -A.Y, -A.Z);
}

Vector3 operator-(Vector3 A, Vector3 B)
{
	return Vector3(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

bool operator>(Vector3 A, Vector3 B)
{
	return A.X > B.X && A.Y > B.Y && A.Z > B.Z;
}

bool operator<(Vector3 A, Vector3 B)
{
	return A.X < B.X && A.Y < B.Y && A.Z < B.Z;
}

bool operator<=(Vector3 A, Vector3 B)
{
	return A.X <= B.X&& A.Y <= B.Y&& A.Z <= B.Z;
}

bool operator>=(Vector3 A, Vector3 B)
{
	return A.X >= B.X && A.Y >= B.Y && A.Z >= B.Z;
}

Vector3 crossProduct(Vector3 A, Vector3 B)
{
	float fX = A.Y * B.Z - A.Z * B.Y;
	float fY = A.Z * B.X - A.X * B.Z;
	float fZ = A.X * B.Y - A.Y * B.X;
	return Vector3(fX, fY, fZ);
}

float dotProduct(Vector3 A, Vector3 B)
{
	return (A.X * B.X) + (A.Y * B.Y) + (A.Z * B.Z);
}


#endif