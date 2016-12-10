#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <QImage>
#include "LightSource.h"
#define PI 3.1415926

using namespace std;

// class used
class Ray;
class FatherMesh;

class Camera
{

public:
	Camera();
	Camera(vector<FatherMesh>* MeshGroup);
	~Camera();
	void				SetPixelSize(double u, double v);
	void				SetCameraPosition(Vec3d posi){ m_center = posi; }
	void				SetCameraLookAt(Vec3d lookAt){ m_lookAt = lookAt; m_lookAt.normalize(); }
	void				SetCameraSample(double sample){ m_sampleNum = sample; }
	QImage*				CreateImage(vector<FatherMesh*> meshs, vector<Light*> lights);

private:
	void				CreateRayParam(vector<Ray>& rays, int w, int h, int s);
	void				CreateCameraParam();

private:
	Vec2d				m_imageSize;
	int					m_rows, m_cols;
	// Camera Param
	Vec3d				m_center;
	Vec3d				m_lookAt;
	Vec3d				m_up;
	double				m_theta;
	double				m_L;
	Vec3d				m_origin;
	Vec3d				m_u;
	Vec3d				m_v;

	// Image Param
	QImage							*m_picture;
	double							m_pixelSize;
	int								m_sampleNum;
	//vector<vector<vector<Ray>>>		m_rayGroup;
};
#endif