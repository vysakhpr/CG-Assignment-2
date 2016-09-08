#ifndef QUATERNION_H
#define QUATERNION_H

#include "math_utils.h"
class Quaternion
{
public:
	float x,y,z,w;	
	Quaternion(float i,float j, float k,float l)
	{
		x=i;
		y=j;
		z=k;
		w=l;
	}

	void Normalize()
	{
    	float Length = sqrt(x * x + y * y + z * z + w * w);
    	x /= Length;
    	y /= Length;
    	z /= Length;
    	w /= Length;
	}

	Quaternion Conjugate()
	{
		return Quaternion(-x,-y,-z,w);
	}

	Vector3f GetVector()
	{
		return Vector3f(x,y,z);
	}

	Quaternion operator*(const Quaternion& r)
	{
    const float q_w = (w * r.w) - (x * r.x) - (y * r.y) - (z * r.z);
    const float q_x = (x * r.w) + (w * r.x) + (y * r.z) - (z * r.y);
    const float q_y = (y * r.w) + (w * r.y) + (z * r.x) - (x * r.z);
    const float q_z = (z * r.w) + (w * r.z) + (x * r.y) - (y * r.x);

    return Quaternion(q_x, q_y, q_z, q_w);
	}


};


Quaternion RotateQuaternion(Quaternion q, Vector3f P)
{
	Matrix4f Lrow_Q=Matrix4f(	q.w,	q.z,	-q.y,	-q.x,
								-q.z,	q.w,	q.x,	-q.y,
								q.y,	-q.x,	q.w,	-q.z,
								q.x,	q.y,	q.z,	q.w);	

	Matrix4f Rrow_Qi=Matrix4f(	q.w,	q.z,	-q.y,	q.x,
								-q.z,	q.w,	q.x,	q.y,
								q.y,	-q.x,	q.w,	q.z,
								-q.x,	-q.y,	-q.z,	q.w);

	Matrix4f Qrow= Rrow_Qi * Lrow_Q;
	//P_rotated=P'*Qrow=(Qrow'*P)'
	Vector4f RotatedVector= Qrow.Transpose()*Vector4f(P.x,P.y,P.z,0);
	return Quaternion(RotatedVector.x,RotatedVector.y,RotatedVector.z,RotatedVector.w);
}

Quaternion CreateRotationQuaternion(float Angle, Vector3f Axe)
{
	float SinHalfAngle = sinf(ToRadian(Angle/2));
   	float CosHalfAngle = cosf(ToRadian(Angle/2));
	float Rx = Axe.x * SinHalfAngle;
   	float Ry = Axe.y * SinHalfAngle;
   	float Rz = Axe.z * SinHalfAngle;
   	float Rw = CosHalfAngle;
   	return Quaternion(Rx,Ry,Rz,Rw);
}

#endif