#include <vector>
#include "Vec3d.h"
using namespace std;

class Camera;
class FatherMesh;
class PointLight;
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

	Camera							*camera_;
	vector<FatherMesh*>				meshs_;
	PointLight						*light_;
	QImage							*picture_;
	//vector<light>					lights_;
	//QImage*
};