#ifndef CAMERA_H
#define CAMERA_H


#include "quaternion.h"
#include "math_utils.h"

class Camera
{
	Vector3f Target;
	Vector3f Up;
	float ZoomFactor;
	float PanHorizontalAngle;
	float PanVerticalAngle;
	bool OnUpperEdge;
    bool OnLowerEdge;
    bool OnLeftEdge;
    bool OnRightEdge;
	
public:
	Camera()
	{
		ZoomFactor=45.0;
		Target=Vector3f(0,0,1);
		Up=Vector3f(0,1,0);
		OnUpperEdge = false;
    	OnLowerEdge = false;
    	OnLeftEdge = false;
    	OnRightEdge = false;
    	Vector3f HTarget=Vector3f(Target.x,0,Target.z);
    	HTarget.Normalize();

    	if(HTarget.z>=0.0f)
    	{
    		if(HTarget.x>=0.0f)
    			PanHorizontalAngle=360.0f-ToDegree(asin(HTarget.z));
    		else
    			PanHorizontalAngle=180.0f+ToDegree(asin(HTarget.z));
    	}
    	else
    	{
    		if(HTarget.x>=0.0f){
    			PanHorizontalAngle=ToDegree(asin(-HTarget.z));
    		}
    		else
    			PanHorizontalAngle=90.0f+ToDegree(asin(-HTarget.z));	
    	}
    	PanVerticalAngle=-ToDegree(asin(Target.y));
	}
	
	void RotateCamera()
	{
		Vector3f Vaxis=Vector3f(0,1,0);
		Vector3f View=Vector3f(1,0,0);
		Quaternion q=CreateRotationQuaternion(PanHorizontalAngle,Vaxis);
		q=RotateQuaternion(q,View);
		View=q.GetVector();
		View.Normalize();

		Vector3f Haxis=Vaxis.Cross(View);
		Haxis.Normalize();
		q=CreateRotationQuaternion(PanVerticalAngle,Haxis);
		q=RotateQuaternion(q,View);
		View=q.GetVector();
		View.Normalize();
		Target=View;
		Target.Normalize();
		Up=Target.Cross(Haxis);
		Up.Normalize();
	}


	
	Matrix4f RenderMatrix()
	{
		Matrix4f CameraTranslateTrans,CameraTrans;
		CheckEdges();
		CameraTranslateTrans.InitTranslationTransform(0,0,boundBox.ZWidth);
		CameraTrans.InitCameraTransform(Target,Up);
		return CameraTrans*CameraTranslateTrans;
	}

	void CheckEdges(){
		bool ShouldRotate = false;
	    if (OnLeftEdge) 
	    {
        	PanHorizontalAngle -= 0.1f;
        	ShouldRotate = true;
    	}
    	else if (OnRightEdge) 
    	{
        	PanHorizontalAngle += 0.1f;
        	ShouldRotate = true;
    	}
    	if (OnUpperEdge) 
    	{
        	if (PanVerticalAngle > -90.0f) 
        	{
            	PanVerticalAngle -= 0.1f;
            	ShouldRotate = true;
        	}
    	}
    	else if (OnLowerEdge) 
    	{
        	if (PanVerticalAngle < 90.0f) 
        	{
            	PanVerticalAngle += 0.1f;
            	ShouldRotate = true;
        	}
    	}
    	if (ShouldRotate) 
    	{
        	RotateCamera();
    	}
	}
	void ZoomIn()
	{
		ZoomFactor+=1;
	}

	void ZoomOut()
	{
		ZoomFactor-=1;
	}

	float FieldOfView()
	{
		return (90-ZoomFactor);
	}

	void SetAngles(int dx,int dy,int x,int y)
	{	
		int MARGIN=100;
		PanHorizontalAngle+=(float)dx/20;
		PanVerticalAngle+=(float)dy/20;
		if(dx == 0)
		{
			if(x<=MARGIN)
				OnLeftEdge=true;
			else if(x>=WINDOW_WIDTH-MARGIN)
				OnRightEdge=true;
		}
		else
		{
			OnLeftEdge=false;
			OnRightEdge=false;
		}
		if(dy==0)
		{
			if(y<=MARGIN)
				OnUpperEdge=true;
			else if(y>=WINDOW_HEIGHT-MARGIN)
				OnLowerEdge=true;	
		}
		else
		{
			OnUpperEdge=false;
			OnLowerEdge=false;
		}
		RotateCamera();
	}

};

#endif