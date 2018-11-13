#include "TVector.h"


///////////////////////////////////////////////////////////////////////////////
float TVector2::operator | (TVector2 const &v0)	//벡터의 내적
{
	return x * v0.x + y * v0.y;
}
TVector2 TVector2::operator ^ (TVector2 const &v0)	//벡터의 외적
{
	return TVector2(0, (x * v0.y - y * v0.x));
}

TVector2::TVector2()
{
}

TVector2::TVector2(const TVector2& v)
{
	x = v.x;
	y = v.y;
}
TVector2::TVector2(float fx, float fy)
{
	x = fx;
	y = fy;
}

TVector2::~TVector2()
{
}
///////////////////////////////////////////////////////////////////////////////
bool TVector3::operator == (TVector3 const &v0)
{
	if (fabs(x - v0.x) < TBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < TBASIS_EPSILON)
		{
			if (fabs(z - v0.z) < TBASIS_EPSILON)
			{
				return true;
			}
		}
	}
	return false;
}
float TVector3::operator | (TVector3 const &v0)	//벡터의 내적
{
	return x * v0.x + y * v0.y + z * v0.z;
}

TVector3 TVector3::operator ^ (TVector3 const &v0)	//벡터의 외적
{
	return TVector3(
		(y * v0.z - z * v0.y),
		(z * v0.x - z * v0.x),
		(x * v0.y - y * v0.x));
}
//-TVector3 마이너스 단항 연산자
TVector3 TVector3::operator - ()
{
	TVector3 temp = *this;
	temp = temp * -1.0f;
	return temp;
}

TVector3 TVector3::operator + (TVector3 const &v0)
{
	return TVector3(v0.x + x, v0.y + y, v0.z + z);
}
TVector3 TVector3::operator - (TVector3 const &v0)
{
	return TVector3(x - v0.x, y - v0.y, z - v0.z);
}

TVector3 TVector3::operator * (float const& fScala)
{
	return TVector3(x * fScala, y * fScala, z * fScala);
}

TVector3::TVector3()
{
}

TVector3::TVector3(const TVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
TVector3::TVector3(float fx, float fy, float fz)
{
	x = fx;
	y = fy;
	z = fz;
}

TVector3::~TVector3()
{
}


TVector4::TVector4()
{
}
///////////////////////////////////////////////////////////////////////////////
TVector4::TVector4(const TVector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
TVector4::TVector4(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

TVector4::~TVector4()
{
}
