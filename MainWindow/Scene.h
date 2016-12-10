#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include "Vec3d.h"

using namespace std;

class Camera;
class FatherMesh;
class Light;
class QImage;

class Scene
{
public:
	Scene();
	~Scene();

	void							BuildScene();
	void							TakePicture();
	void							GetImage(QImage*& picture);
	Vec3d							camera_position= Vec3d(0.0, 0.0, -2.0);
	Vec3d							camera_LookAt = Vec3d(0.0, 0.0, 1.0);
	double							sample = 1;

private:
	void							BuildCamera();
	void							BuildMeshGroup();
	void							BuildLightSource();
	void							SampleAreaLight();

	Camera							*camera_;
	QImage							*picture_;
	vector<FatherMesh*>				meshs_;
	vector<Light*>					lights_;
	//vector<light>					lights_;
	//QImage*
};
#endif