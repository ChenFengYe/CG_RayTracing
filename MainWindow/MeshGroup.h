#ifndef FATHERMESH_H
#define FATHERMESH_H
 
#include "Vec3d.h"
#include "ray.h"
#include "vector"
using namespace std;
enum MeshType
{
	SphereMesh = 1,
	PlaneMesh = 2,
};
class PointLight;

//-------------------------------------------------------------//
// Father Mesh
//-------------------------------------------------------------//
class FatherMesh
{

public:
	FatherMesh();
	~FatherMesh();
	Vec3d			GetPointValue(Ray* ray, PointLight* light, vector<FatherMesh*> meshs, Vec4d Insec, Vec3d EyeDire);
	virtual Vec4d	GetIntersection(Ray* ray);
	virtual Vec3d	GetInsecNormal(Vec3d Insec);


public:
	MeshType							m_meshType;
	Vec3d								m_color = Vec3d(1.0, 1.0, 1.0);

	
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
	MeshType							m_meshType = PlaneMesh;
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
	MeshType							m_meshType = SphereMesh;
	Vec3d								m_center;
	double								m_r;

};
#endif