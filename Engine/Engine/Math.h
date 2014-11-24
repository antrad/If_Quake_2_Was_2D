#include "Advanced2D.h"

#pragma once

namespace Advanced2D {

	const float PI = 3.1415926535f;
	const float PI_over_180 = PI / 180.0f;
	const float PI_under_180 = 180.0f / PI;

	class Math {
	public:
		float toDegrees(float radian);
		float toRadians(float degree);
		float wrapAngleDegs(float degs);
		//float wrapAngleRads(float rads);
		float LinearVelocityX(float angle);
		float LinearVelocityY(float angle);
		Vector3 LinearVelocity(float angle);
		float AngleToTarget(float x1,float y1,float x2,float y2);
		float AngleToTarget(Vector3& source,Vector3& target);
		float Distance( float x1,float y1,float x2,float y2 );
		float Distance( Vector3& v, Vector3& vec2  );
		float Length(Vector3& vec);
		float Length(float x,float y,float z);
		float DotProduct(float x1,float y1,float z1,float x2,float y2,float z2);
		float DotProduct( Vector3& vec1, Vector3& vec2 );
		Vector3 CrossProduct( float x1,float y1,float z1,float x2,float y2,float z2);
		Vector3 CrossProduct( Vector3& vec1, Vector3& vec2 );
		Vector3 Normal(float x,float y,float z);
		Vector3 Normal(Vector3& vec);



	};

};
