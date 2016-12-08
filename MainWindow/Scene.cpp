#include "Scene.h"
#include "MeshGroup.h"
#include "camera.h"
#include "LightSource.h"
#include "ray.h"
#include "QImage"
#include "Vec3d.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	if (camera_!=NULL)
		delete camera_;

	if (meshs_.size() > 0)
	{
		for (int i = 0; i < meshs_.size();i++)
			delete meshs_[i];
	}
}

void Scene::BuildScene()
{
	BuildCamera();
	BuildMeshGroup();
	BuildLightSource();
}
void Scene::BuildCamera()
{
	camera_ = new Camera;

}

void Scene::BuildMeshGroup()
{
	Sphere *shpere1 = new Sphere(Vec3d(0.0, 0.0, 3.0), 1.0, Vec3d(1.0, 0.0, 0.0));
	Sphere *shpere2 = new Sphere(Vec3d(0.3, -0.6, 2.0), 0.3, Vec3d(0.0, 1.0, 0.0));
	Sphere *shpere3 = new Sphere(Vec3d(-1.0, 0.0, 2.0), 0.5, Vec3d(0.0, 0.0, 1.0));
	Plane *plane1 = new Plane(0.0, 1.0, 0.0, 1.0, Vec3d(0.0, 1.0, 0.0), Vec3d(0.5, 0.5, 0.5));
	Plane *plane2 = new Plane(1.0, 0.0, 0.0, -3.0, Vec3d(1.0, 0.0, 0.0), Vec3d(0.5, 0.5, 0.5));
	Plane *plane3 = new Plane(1.0, 0.0, 0.0, 3.0, Vec3d(-1.0, 0.0, 0.0), Vec3d(0.5, 0.5, 0.5));
	Plane *plane4 = new Plane(0.0, 0.0, 1.0, -5.0, Vec3d(0.0, 0.0, 1.0), Vec3d(0.2, 0.3, 0.5));
	//Plane *plane5 = new Plane(1.0, 0.0, 0.0, 5.0, Vec3d(0.0, 1.0, 0.0), Vec3d(0.0, 0.0, 1.0));
	meshs_.push_back(shpere1);
	meshs_.push_back(shpere2);
	meshs_.push_back(shpere3);
	meshs_.push_back(plane1);
	meshs_.push_back(plane2);
	meshs_.push_back(plane3);
	meshs_.push_back(plane4);
}

void Scene::BuildLightSource()
{
	light_ = new PointLight;
}

void Scene::TakePicture()
{
	camera_->SetCameraPosition(camera_position);
	camera_->SetCameraLookAt(camera_LookAt);
	camera_->SetCameraSample(sample);
	picture_ = camera_->CreateImage(meshs_, light_);
}

void Scene::GetImage(QImage*& picture)
{
	picture = picture_;
}