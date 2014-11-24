#include "Advanced2D.h"

namespace Advanced2D {

	float Math::toDegrees(float radians)
	{
		return radians * PI_under_180;
	}

	float Math::toRadians(float degrees)
	{
		return degrees * PI_over_180;
	}

	float Math::wrapAngleDegs(float degs)
	{
		float result = fmod(degs, 360.0f);
		if (result < 0) result += 360.0f;
		return result;
	}

	/*float Math::wrapAngleRads(float rads)
	{
		float result = fmod(rads, 2*PI);//no 2*
		if (result > PI) result = 2*PI - result;
		else result=-result;
		//if (result < 0) result += PI;
		return result;
	}*/

	float Math::LinearVelocityX(float angle)
	{
		//angle -= 90;
		if (angle < 0) angle = 360 + angle;    
		return cos( angle * PI_over_180);
	}

	float Math::LinearVelocityY(float angle)
	{
		//angle -= 90;
		if (angle < 0) angle = 360 + angle;    
		return sin( angle * PI_over_180);
	}

	Vector3 Math::LinearVelocity(float angle)
	{
		float vx = LinearVelocityX(angle);
		float vy = LinearVelocityY(angle);
		return Vector3(vx,vy,0.0f);
	}

	float Math::AngleToTarget(float x1,float y1,float x2,float y2)
	{
		float deltaX = (x2-x1);
		float deltaY = (y2-y1);
		return atan2(deltaY,deltaX);
	}

	float Math::AngleToTarget(Vector3& source,Vector3& target)
	{
		return AngleToTarget(source.getX(),source.getY(),target.getX(),target.getY());
	}

	float Math::Distance( float x1,float y1,float x2,float y2 )
	{
		float deltaX = (x2-x1);
		float deltaY = (y2-y1);
		return sqrt(deltaX*deltaX + deltaY*deltaY);
	}

	float Math::Distance( Vector3& vec1, Vector3& vec2 ) 
	{
		return Distance(vec1.getX(),vec1.getY(),vec2.getX(),vec2.getY());
	}

	float Math::Length(float x,float y,float z)
	{
		return sqrt(x*x + y*y + z*z);
	}

	float Math::Length(Vector3& vec)
	{
		return Length(vec.getX(),vec.getY(),vec.getZ());
	}

	float Math::DotProduct(float x1,float y1,float z1,float x2,float y2,float z2)
	{
		return (x1*x2 + y1*y2 + z1*z2);
	}

	float Math::DotProduct( Vector3& vec1, Vector3& vec2 )
	{
		return DotProduct(vec1.getX(),vec1.getY(),vec1.getZ(),vec2.getX(),vec2.getY(),vec2.getZ());
	}

	Vector3 Math::CrossProduct( float x1,float y1,float z1,float x2,float y2,float z2)
	{
		float nx = (y1*z2)-(z1*y2);
		float ny = (z1*y2)-(x1*z2);
		float nz = (x1*y2)-(y1*x2);
		return Vector3(nx,ny,nz);
	}

	Vector3 Math::CrossProduct( Vector3& vec1, Vector3& vec2 )
	{
		return CrossProduct(vec1.getX(),vec1.getY(),vec1.getZ(),vec2.getX(),vec2.getY(),vec2.getZ());
	}

	Vector3    Math::Normal(float x,float y,float z)
	{
		float length = Length(x,y,z);
		if (length != 0) length = 1 / length;
		float nx = x*length;
		float ny = y*length;
		float nz = z*length;
		return Vector3(nx,ny,nz);
	}

	Vector3 Math::Normal(Vector3& vec)
	{
		return Normal(vec.getX(),vec.getY(),vec.getZ());
	}

};
