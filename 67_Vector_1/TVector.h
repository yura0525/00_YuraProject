#pragma once
#include "xStd.h"

#define TBASIS_EPSILON 0.001
#define TBASIS_PI 3.141592654
#define DtoR(degree)	((degree) * (TBASIS_PI/180.0))
#define RtoD(radian)	((radian) * (180.0 / TBASIS_PI))

class TVector2
{
public:
	float x, y;
public:
	float operator | (TVector2 const &v0);		//벡터의 내적
	TVector2 operator ^ (TVector2 const &v0);	//벡터의 외적
public:
	TVector2();
	TVector2(const TVector2& v);
	TVector2(float fx, float fy);
	~TVector2();
};

class TVector3
{
public:
	float x, y, z;
public:
	bool operator == (TVector3 const &v0);
	float operator | (TVector3 const &v0);		//벡터의 내적
	TVector3 operator ^ (TVector3 const &v0);	//벡터의 외적

	TVector3 operator - ();
	TVector3 operator + (TVector3 const &v0);
	TVector3 operator - (TVector3 const &v0);	//-TVector3 마이너스 단항 연산자
	TVector3 operator * (float const& fScala);
	TVector3 Normalize()
	{
		float invert = 1.0f / Length();
		return TVector3(x*invert, y*invert, z*invert);
	}
	TVector3 Normal()
	{
		float invert = 1.0f / Length();
		x *= invert;
		y *= invert;
		z *= invert;

		return *this;
	}
	float Length()
	{
		return sqrt(x*x + y * y + z * z);
	}
public:
	TVector3();
	TVector3(const TVector3& v);
	TVector3(float fx, float fy, float fz);
	~TVector3();
};


class TVector4
{
public:
	float x, y, z, w;
public:
	TVector4();
	TVector4(const TVector4& v);
	TVector4(float fx, float fy, float fz, float fw);
	~TVector4();
};


