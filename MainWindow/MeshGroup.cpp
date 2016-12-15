#include "LightSource.h"
#include "MeshGroup.h"

using namespace std;
//-------------------------------------------------------------//
// Father Mesh
//-------------------------------------------------------------//
FatherMesh::FatherMesh()
{
	IsLightPlane = false;

}

FatherMesh::~FatherMesh()
{

}

Vec3d FatherMesh::GetPointValue(
	Ray* ray, 
	vector<Light*> lights,
	vector<FatherMesh*> meshs,
	Vec4d insec, 
	Vec3d EyeDire)
{
	Vec3d pointColor = Vec3d(0, 0, 0);
	Vec3d SpecularPointColor = Vec3d(0, 0, 0);
	Vec3d DiffusePointColor = Vec3d(0, 0, 0);

	for (int i_l = 0; i_l < lights.size(); i_l++)
	{
		//-----------------------------------------------//
		// Specular
		if (lights[i_l]->m_LightType == CubeType && !IsLightPlane)	// text light no specular
		{
			SpecularPointColor += GetSpecularValue(insec, ray, meshs);
		}

		//-----------------------------------------------//
		// Diffuse
		if (lights[i_l]->m_LightType != CubeType)
		{
			DiffusePointColor += GetDiffuseValue(insec, ray, lights[i_l], meshs, EyeDire);
		}
	}
		
	double KDeffuse = 2.5;
	double KSpecualr = 1;
	pointColor = KSpecualr * SpecularPointColor + 1.0 / lights.size() *KDeffuse * DiffusePointColor;

	return Vec3d(pointColor.x(), pointColor.y(), pointColor.z());
}

Vec3d FatherMesh::GetDiffuseValue(Vec4d insec, Ray *cameraRay, Light* light, vector<FatherMesh*> meshs, Vec3d EyeDire)
{
	Vec3d pointColor = Vec3d(0, 0, 0);
	Vec3d insecP = toVec3d(insec);

	// Check Block
	bool isBlocked = false;
	Ray BlockRay;
	BlockRay = light->GetLightSourceRay(BlockRay, insecP);	// Point Light get reverse light

	// Check blocked
	for (int i = 0; i < meshs.size(); i++)
	{
		Vec4d lightPoint = meshs[i]->GetIntersection(&BlockRay);
		double OldInsecDist = BlockRay.m_d;
		double NewInsecDist;

		// Get TriMesh Insection distance
		if (meshs[i]->m_meshType == TriangleMesh
			&& lightPoint[3] != DBL_MAX)
		{
			NewInsecDist =
				(BlockRay.m_origin - toVec3d(lightPoint)).length();
		}
		// Get Other Mesh Insection distance
		else
		{
			NewInsecDist = lightPoint[3];
		}

		// 遮挡的是 Light Plane 且  自己不是LightPlane
		// 不会被遮挡
		if (meshs[i]->IsLightPlane)
		{
			NewInsecDist = DBL_MAX;
		}
		if (NewInsecDist - OldInsecDist < eposlion)
		{
			isBlocked = true;
			break;
		}
	}

	if (isBlocked)	return Vec3d(0, 0, 0);
	//double ambition = 0.5;
	//if (isBlocked)
	//{
	//	m_color*ambition;
	//}

	// Get Color Value
	Vec3d normal = GetInsecNormal(insec, cameraRay);
	Vec3d color = GetInsecColor(insec, 200.0);

	// Envernment Texture
	if (m_meshType == PlaneMesh && IsLightPlane)
	{
		pointColor = color;
	}

	// Object Mesh Light
	else
	{
		pointColor = light->GetPointColor(insecP, color, normal, EyeDire);
	}
	return pointColor;
};

Vec3d FatherMesh::toVec3d(Vec4d v)	
{ return Vec3d(v.x(), v.y(), v.z()); }


Vec4d FatherMesh::GetIntersection(Ray* ray)
{
	std::cout << "virtual father mesh function!" << std::endl;
	return NULL;
}

Vec3d FatherMesh::GetInsecNormal(Vec4d Insec, Ray *y)
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

Vec4d Plane::GetIntersection(Ray* ray)
{
	// A(Ox + k*Dx) + B(Oy + k*Dy) + C(Oz +k*Dz) + D = 0
	// k = -（A*Ox + B*Oy + C*Oz + D) / (A*Dx + B*Dy + C*Dz)
	double t = -(ray->m_origin.x()*A_ + ray->m_origin.y()*B_ + ray->m_origin.z()*C_+ D_) 
		/ (A_*ray->m_dire.x() + B_*ray->m_dire.y()+ C_*ray->m_dire.z());

	Vec3d Insec = ray->m_origin + t*ray->m_dire;
	if (t < eposlion)
		return Vec4d(0.0, 0.0, 0.0, DBL_MAX);
	
	if (m_origin != Vec3d(DBL_MAX, DBL_MAX, DBL_MAX) &&
		m_end != Vec3d(DBL_MAX, DBL_MAX, DBL_MAX))
	{
		// axis x = 0   axis y = 1  axis z = 2
		// vertical x axis
		if (abs(m_normal.x()) > eposlion&&abs(m_normal.y()) < eposlion&& abs(m_normal.z()) < eposlion)
		{
			// y z   1 2
			if (CheckEdgeRange(Insec, 1, 2) == false)
				return Vec4d(0.0, 0.0, 0.0, DBL_MAX);
		}
		// vertical y axis
		else if (abs(m_normal.x()) < eposlion&&abs(m_normal.y()) > eposlion&&abs(m_normal.z()) < eposlion)
		{
			// x z   0 2
			if (CheckEdgeRange(Insec, 0, 2) == false)
			return Vec4d(0.0, 0.0, 0.0, DBL_MAX);
		}
		// vertical z axis
		else if (abs(m_normal.x()) < eposlion&&abs(m_normal.y()) < eposlion&&abs(m_normal.z()) > eposlion)
		{
			// x y   0 1
			if (CheckEdgeRange(Insec, 0, 1) == false)
			return Vec4d(0.0, 0.0, 0.0, DBL_MAX);
		}
	}
	return Vec4d(Insec.x(), Insec.y(),Insec.z(), (Insec-ray->m_origin).length());
}

Vec3d Plane::GetInsecNormal(Vec4d Insec, Ray *ray)
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
	m_meshType = SphereMesh;
	IsLightPlane = false;
}

Vec4d Sphere::GetIntersection(Ray* ray)
{
	Vec3d	orig = ray->m_origin;
	Vec3d	dire = ray->m_dire;
	Vec3d	l = m_center - orig;
	double	dist = l.length();

	// Sphere is on the opposite direction of ray
	double	s = l.dot(dire);
	if (s < 0 && dist > m_r)	return Vec4d(0.0, 0.0, 0.0, DBL_MAX);

	// Distance between line and sphere is too far
	double m = sqrt(l*l - s*s);
	if (m > m_r)				return Vec4d(0.0, 0.0, 0.0, DBL_MAX);

	// Get the length between origin and intersection
	double t;
	double q = sqrt(m_r*m_r - m*m);
	if (dist > m_r)				t = s - q;
	else						t = s + q;

	Vec3d inters = t * dire + orig;
	return Vec4d(inters.x(), inters.y(), inters.z(), t);
}

Vec3d Sphere::GetInsecNormal(Vec4d Insec, Ray * ray)
{
	Vec3d normal;
	normal = toVec3d(Insec) - m_center;
	normal.normalize();
	return Vec3d(normal.x(), normal.y(), normal.z());
}

//-------------------------------------------------------------//
// TriMesh Mesh
//-------------------------------------------------------------//
TriMesh::TriMesh()
{

}

TriMesh::~TriMesh() 
{

}

TriMesh::TriMesh(char* file)
{
	m_meshType = TriangleMesh;
	LoadFromFile(file);
	KDRoot_.tris_ = *mesh.get_faces_list();
	KDRoot_.CreatKDTree(0);
	IsLightPlane = false;
}

Vec4d TriMesh::GetIntersection(Ray* ray) 
{
	return KDRoot_.HitKDTree(ray);
	

}

Vec3d TriMesh::GetInsecNormal(Vec4d Insec, Ray *ray)
{
	Vec3d Norm;
	HE_face* InsecFace = mesh.get_faces_list()->at((int)Insec[3]);

	KDNode::HitTriangle(ray, InsecFace, Norm);

	return Norm;
}

bool TriMesh::LoadFromFile(char* file)
{
	return mesh.LoadFromOBJFile(file);
}