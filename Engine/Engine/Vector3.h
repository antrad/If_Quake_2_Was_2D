#include "Advanced2D.h"

#pragma once

namespace Advanced2D {

	class Vector3 {
	private:
		float x, y, z;

	public:
		Vector3();
		Vector3(const Vector3& v);
		Vector3(float x, float y, float z);
		Vector3(int x, int y, int z);
		void Set(float x1,float y1,float z1);
		void Set(const Vector3& v);
		float getX() { return x; }
		void setX(float v) { x = v; }
		float getY() { return y; }
		void setY(float v) { y = v; }
		float getZ() { return z; }
		void setZ(float v) { z = v; }
		void Move( float mx,float my,float mz);
		void operator+=(const Vector3& v);
		void operator-=(const Vector3& v);
		void operator*=(const Vector3& v);
		void operator/=(const Vector3& v);
		bool operator==( const Vector3& v ) const;
		bool operator!=( const Vector3& p ) const;
		Vector3& operator=( const Vector3& v);
		float Distance( const Vector3& v );
		float Length();
		float DotProduct( const Vector3& v );
		Vector3 CrossProduct( const Vector3& v );
		Vector3 Normal();
	}; //class
};


