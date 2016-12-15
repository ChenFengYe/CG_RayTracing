#include "Scene.h"
#include "QImage"

Scene::Scene()
{
	hdrImage = hdrImage.fromFile("..\\MainWindow\\HDR\\grace_cross.hdr");
	//hdrImage = hdrImage.fromFile("..\\MainWindow\\HDR\\1.png");
}

Scene::~Scene()
{
	if (camera_!=NULL)
		delete camera_;
	if (meshs_.size() > 0)
	{
		for (int i = 0; i < meshs_.size(); i++)
			delete meshs_[i];
	}

	if (lights_.size() >0)
	{
		for (int i = 0; i < lights_.size(); i++)
			delete lights_[i];
	}
}

void Scene::BuildScene()
{
	BuildCamera();
	BuildLightSource();
	BuildMeshGroup();
}
void Scene::BuildCamera()
{
	camera_ = new Camera;
}

void Scene::BuildMeshGroup()
{
	Sphere *shpere1 = new Sphere(Vec3d(0.0, 0.0, 3.0), 1.0, Vec3d(1.0, 1.0, 1.0));
	Sphere *shpere2 = new Sphere(Vec3d(0.3, -0.6, 2.0), 0.3, Vec3d(0.0, 1.0, 0.0));
	Sphere *shpere3 = new Sphere(Vec3d(-1.0, 0.0, 2.0), 0.5, Vec3d(0.0, 0.0, 1.0));

	Plane *plane1 = new Plane(0.0, 1.0, 0.0, 1.0, Vec3d(0.0, 1.0, 0.0), Vec3d(0.5, 0.5, 0.5));
	Plane *plane2 = new Plane(1.0, 0.0, 0.0, -3.0, Vec3d(1.0, 0.0, 0.0), Vec3d(0.5, 0.5, 0.5));
	Plane *plane3 = new Plane(1.0, 0.0, 0.0, 3.0, Vec3d(-1.0, 0.0, 0.0), Vec3d(0.5, 0.5, 0.5));
	//Plane *plane4 = new Plane(0.0, 0.0, 1.0, -5.0, Vec3d(0.0, 0.0, 1.0), Vec3d(0.2, 0.3, 0.5));
	TriMesh *Bunny= new TriMesh("D:\\Desktop\\MainWindow\\MainWindow\\OBJ\\bunny.obj");
	//TriMesh *Dragon = new TriMesh("D:\\Desktop\\MainWindow\\MainWindow\\OBJ\\dragon2.obj");
	//TriMesh *TestTrian = new TriMesh("D:\\Desktop\\MainWindow\\MainWindow\\OBJ\\testTrian.obj");
	//TriMesh *ball = new TriMesh("D:\\Desktop\\MainWindow\\MainWindow\\OBJ\\ball.obj");
	//TriMesh *untitled = new TriMesh("D:\\Desktop\\MainWindow\\MainWindow\\OBJ\\untitled.obj");
	//TriMesh *Bunny = new TriMesh("D:\\Desktop\\MainWindow\\MainWindow\\OBJ\\bunnyEars.obj");

	// Build for CubeLight
	meshs_.push_back(cubeLight->left_plane_);
	meshs_.push_back(cubeLight->right_plane_);
	meshs_.push_back(cubeLight->front_plane_);
	meshs_.push_back(cubeLight->up_plane_);
	meshs_.push_back(cubeLight->down_plane_);
	meshs_.push_back(cubeLight->back_plane_);

	//meshs_.push_back(shpere1);
	//meshs_.push_back(shpere2);
	//meshs_.push_back(shpere3);
	//meshs_.push_back(plane1);
	//meshs_.push_back(plane2);
	//meshs_.push_back(plane3);
	//meshs_.push_back(plane4);
	meshs_.push_back(Bunny);
	//meshs_.push_back(Dragon);
	//meshs_.push_back(untitled);
	//meshs_.push_back(TestTrian);
	//meshs_.push_back(ball);
}

void Scene::BuildLightSource()
{
	//PointLight *pointLight = new PointLight;
	//lights_.push_back(pointLight);
	

	double CubeSide = 10;
	cubeLight = new CubeLight(&hdrImage, CubeSide);
	lights_.push_back(cubeLight);
	lights_.insert(lights_.end(), 
		cubeLight->m_SampleLights.begin(), 
		cubeLight->m_SampleLights.end());
}

void Scene::TakePicture()
{
	camera_->SetCameraPosition(camera_position);
	camera_->SetCameraLookAt(camera_LookAt);
	camera_->SetCameraSample(sample);
	picture_ = camera_->CreateImage(meshs_, lights_);
}

void Scene::GetImage(QImage*& picture)
{
	picture = picture_;
}

void Scene::SampleCubeLight()
{

}
