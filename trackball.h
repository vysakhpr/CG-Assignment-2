#ifndef TRACKBALL_H
#define TRACKBALL_H


#include "quaternion.h"
#include "math_utils.h"

class TrackBall
{
	Vector3f Target;
	Vector3f Up;
	float PanHorizontalAngle;
	float PanVerticalAngle;
	bool OnUpperEdge;
    bool OnLowerEdge;
    bool OnLeftEdge;
    bool OnRightEdge;

	
public:

	void ResetTrackBall()
	{
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


	TrackBall()
	{
		ResetTrackBall();
	}
	
	
	void RotateTrackBall()
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
		Matrix4f TrackBallTransH,TrackBallTransV;
		CheckEdges();
		TrackBallTransH.InitAxisRotateTransform(Up,PanHorizontalAngle);
		TrackBallTransV.InitAxisRotateTransform(Up.Cross(Target),PanVerticalAngle);
		//TrackBallTrans.InitCameraTransform(Target,Up);
		return TrackBallTransH*TrackBallTransV;
	}

	void CheckEdges(){
		bool ShouldRotate = false;
	    if (OnLeftEdge) 
	    {
        	PanHorizontalAngle += 0.01f;
        	ShouldRotate = true;
    	}
    	else if (OnRightEdge) 
    	{
        	PanHorizontalAngle -= 0.01f;
        	ShouldRotate = true;
    	}
    	if (OnUpperEdge) 
    	{
            	PanVerticalAngle += 0.01f;
            	ShouldRotate = true;
    	}
    	else if (OnLowerEdge) 
    	{
            	PanVerticalAngle -= 0.01f;
            	ShouldRotate = true;
    	}

    	if (ShouldRotate) 
    	{
        	RotateTrackBall();
    	}
	}

	void SetAngles(int dx,int dy,int x,int y)
	{	
		int MARGIN=50;
		PanHorizontalAngle+=(float)+2*dx/WINDOW_WIDTH;
		PanVerticalAngle+=(float)+2*dy/WINDOW_HEIGHT;
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
		RotateTrackBall();
	}

};

#endif