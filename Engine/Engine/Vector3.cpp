#include "Advanced2D.h"

namespace Advanced2D {

	Vector3::Vector3()
	{
		x = y = z = 0;
	}

	Vector3::Vector3( const Vector3& v ) 
	{ 
		*this = v; 
	}

	Vector3::Vector3( float x, float y, float z ) 
	{ 
		Set( x, y, z ); 
	}

	Vector3::Vector3( int x, int y, int z)
	{ 
		Set((double)x,(double)y,(double)z); 
	}

	void Vector3::Set( float x1,float y1,float z1 ) 
	{ 
		x=x1; y=y1; z=z1; 
	}

	void Vector3::Set( const Vector3& v) 
	{ 
		x=v.x; y=v.y; z=v.z; 
	}

	void Vector3::Move( float mx,float my,float mz) 
	{ 
		x+=mx; y+=my; z+=mz; 
	}

	void Vector3::operator+=(const Vector3& v) 
	{ 
		x+=v.x; y+=v.y; z+=v.z; 
	}

	void Vector3::operator-=(const Vector3& v) 
	{ 
		x-=v.x; y-=v.y; z-=v.z; 
	}

	void Vector3::operator*=(const Vector3& v) 
	{ 
		x*=v.x; y*=v.y; z*=v.z; 
	}

	void Vector3::operator/=(const Vector3& v) 
	{ 
		x/=v.x; y/=v.y; z/=v.z; 
	}

	//equality operator comparison includes float rounding
	bool Vector3::operator==( const Vector3& v ) const
	{ 
		return (
			(((v.x - 0.0001f) < x) && (x < (v.x + 0.0001f))) &&
			(((v.y - 0.0001f) < y) && (y < (v.y + 0.0001f))) &&
			(((v.z - 0.0001f) < z) && (z < (v.z + 0.0001f))) );
	}

	//inequality operator
	bool Vector3::operator!=( const Vector3& p ) const 
	{
		return (!(*this == p));
	}

	//assign operator
	Vector3& Vector3::operator=( const Vector3& v) 
	{
		Set(v);
		return *this;
	}

	//distance only coded for 2D
	float Vector3::Distance( const Vector3& v ) 
	{
		return sqrt((v.x-x)*(v.x-x) + (v.y-y)*(v.y-y));
	}

	//Vector3 length is distance from the origin
	float Vector3::Length()
	{
		return sqrt(x*x + y*y + z*z);
	}

	//dot/scalar product: difference between two directions
	float Vector3::DotProduct( const Vector3& v )
	{
		return (x*v.x + y*v.y + z*v.z);
	}

	//cross/Vector product is used to calculate the normal
	Vector3 Vector3::CrossProduct( const Vector3& v )
	{
		float nx = (y*v.z)-(z*v.y);
		float ny = (z*v.y)-(x*v.z);
		float nz = (x*v.y)-(y*v.x);
		return Vector3(nx,ny,nz);
	}

	//calculate normal angle of the Vector
	Vector3 Vector3::Normal()
	{
		float length;
		if (Length() == 0)
			length = 0;
		else
			length = 1 / Length();
		float nx = x*length;
		float ny = y*length;
		float nz = z*length;
		return Vector3(nx,ny,nz);
	}


}
