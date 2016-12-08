#include "Vec3d.h"
#include "MeshGroup.h"
#include "LightSource.h"
#include "ray.h"

using namespace std;
#define eposlion 0.000001
//-------------------------------------------------------------//
// Father Mesh
//-------------------------------------------------------------//
FatherMesh::FatherMesh()
{

}

FatherMesh::~FatherMesh()
{

}

Vec3d FatherMesh::GetPointValue(Ray* ray, PointLight* light, vector<FatherMesh*> meshs, Vec4d Insec, Vec3d EyeDire)
{
	// Is blocked ?
	bool isBlocked = false;
	Vec3d p = Vec3d(Insec.x(), Insec.y(), Insec.z());
	Vec3d dire = light->m_center - p; dire.normalize();
	p += eposlion*dire;
	Ray LightRay(p, light->m_center);												// Get light ray
	double d_light_insec = (p - light->m_center).length();							// Get distance between the light Source and the intersection point
	for (int i = 0; i < meshs.size(); i++)
	{
		Vec4d lightPoint = meshs[i]->GetIntersection(&LightRay);
		if (lightPoint[3] > 0 && lightPoint[3] < d_light_insec)
		{
			isBlocked = true;
			break;
		}
	}
	double ambition = 0.3;
	if (isBlocked)	return m_color*ambition;

	// Get Color Value
	Vec3d normal = GetInsecNormal(p);
	Vec3d pointColor = light->GetPointColor(p, m_color, normal, EyeDire);
	return Vec3d(pointColor.x(), pointColor.y(), pointColor.z());
}

Vec4d FatherMesh::GetIntersection(Ray* ray)
{
	std::cout << "virtual father mesh function!" << std::endl;
	return NULL;
}

Vec3d FatherMesh::GetInsecNormal(Vec3d Insec)
{
	std::cout << "virtual father mesh function!" << std::endl;
	return NULL;
}

//-------------------------------------------------------------//
// Plane Mesh
//-------------------------------------------------------------//
Plane::Plane()
{

}

Plane::~Plane()
{

}

Plane::Plane(double A, double B, double C, double D, Vec3d normal, Vec3d color)
{
	A_ = A;
	B_ = B;
	C_ = C;
	D_ = D;
	m_color = color;
	m_normal = normal;
}

Vec4d Plane::GetIntersection(Ray* ray)
{
	double k = -(ray->m_origin.x()*A_ + ray->m_origin.y()*B_ 
		+ ray->m_origin.z()*C_+ D_) / (A_*ray->m_dire.x() + B_*ray->m_dire.y()+ C_*ray->m_dire.z());

	Vec3d Insec = ray->m_origin + k*ray->m_dire;
	if ((Insec - ray->m_origin).dot(ray->m_dire) < 0)
		return Vec4d(0.0, 0.0, 0.0, -1.0);
	else
		return Vec4d(Insec.x(), Insec.y(),Insec.z(), (Insec-ray->m_origin).length());
}

Vec3d Plane::GetInsecNormal(Vec3d Insec)
{
	return m_normal;
}

//-------------------------------------------------------------//
// Sphere Mesh
//-------------------------------------------------------------//
Sphere::Sphere()
{

}

Sphere::~Sphere()
{

}

Sphere::Sphere(Vec3d center, double r, Vec3d color)
{
	m_center = center;
	m_r = r;
	m_color = color;
}

Vec4d Sphere::GetIntersection(Ray* ray)
{
	Vec3d	orig = ray->m_origin;
	Vec3d	dire = ray->m_dire;
	Vec3d	l = m_center - orig;
	double	dist = l.length();

	// Sphere is on the opposite direction of ray
	double	s = l.dot(dire);
	if (s < 0 && dist > m_r)	return Vec4d(0.0, 0.0, 0.0, -1.0);

	// Distance between line and sphere is too far
	double m = sqrt(l*l - s*s);
	if (m > m_r)				return Vec4d(0.0, 0.0, 0.0, -1.0);

	// Get the length between origin and intersection
	double t;
	double q = sqrt(m_r*m_r - m*m);
	if (dist > m_r)				t = s - q;
	else						t = s + q;

	Vec3d inters = t * dire + orig;
	return Vec4d(inters.x(), inters.y(), inters.z(), t);
}

Vec3d Sphere::GetInsecNormal(Vec3d Insec)
{
	Vec3d normal;
	normal = Insec - m_center;
	normal.normalize();
	return Vec3d(normal.x(), normal.y(), normal.z());
}