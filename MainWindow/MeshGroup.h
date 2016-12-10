#ifndef FATHERMESH_H
#define FATHERMESH_H
 
#include "ray.h"
#include <vector>
#include "KDTree.h"

using namespace std;
class Light;
class KDNode;

enum MeshType
{
	SphereMesh = 1,
	PlaneMesh = 2,
	TriangleMesh = 3,
};
//-------------------------------------------------------------//
// Father Mesh
//-------------------------------------------------------------//
class FatherMesh
{

public:
	FatherMesh();
	~FatherMesh();
	Vec3d			GetPointValue(Ray* ray, vector<Light*> lights, vector<FatherMesh*> meshs, Vec4d Insec, Vec3d EyeDire);
	virtual Vec4d	GetIntersection(Ray* ray);
	virtual Vec3d	GetInsecNormal(Vec3d Insec);
	Vec3d			toVec3d(Vec4d v)	{return Vec3d(v.x(), v.y(), v.z());}

public:
	MeshType							m_meshType;
	Vec3d								m_color = Vec3d(1.0, 1.0, 1.0);

public:
	Mesh3D								mesh;
};
//-------------------------------------------------------------//
// Plane Mesh
//-------------------------------------------------------------//
class Plane :public FatherMesh
{
public:
	Plane();
	Plane(double A, double B, double C, double D, Vec3d normal, Vec3d color);
	~Plane();

	Vec4d								GetIntersection(Ray* ray);
	Vec3d								GetInsecNormal(Vec3d Insec);
public:
	double								A_;
	double								B_;
	double								C_;
	double								D_;
	Vec3d								m_normal;
};
//-------------------------------------------------------------//
// Sphere Mesh
//-------------------------------------------------------------//
class Sphere:public FatherMesh
{
public:
	Sphere();
	Sphere(Vec3d center, double r,Vec3d color);
	~Sphere();

	Vec4d								GetIntersection(Ray* ray);
	Vec3d								GetInsecNormal(Vec3d Insec);
public:
	Vec3d								m_center;
	double								m_r;
};
//-------------------------------------------------------------//
// Sphere Mesh
//-------------------------------------------------------------//
class TriMesh :public FatherMesh
{
public:
	TriMesh();
	TriMesh(char* file);
	~TriMesh();

	Vec4d								GetIntersection(Ray* ray);
	Vec3d								GetInsecNormal(Vec3d Insec);
	bool								LoadFromFile(char* file);

private:
	KDNode								KDRoot_;
};
#endif