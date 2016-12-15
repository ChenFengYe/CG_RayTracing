#ifndef FATHERMESH_H
#define FATHERMESH_H
 
#include <vector>
#include "ray.h"
#include "KDTree.h"
#include "image.h"

using namespace lw;
using namespace std;
class KDNode;
class Plane;

enum MeshType
{
	SphereMesh = 1,
	PlaneMesh = 2,
	TriangleMesh = 3,
};
enum PlaneType
{
	LeftType = 1,
	RightType = 2,
	FrontType = 3,
	UpType = 4,
	DownType =5,
	BackType = 6,
};
//-------------------------------------------------------------//
// Father Mesh
//-------------------------------------------------------------//
class Light;
class FatherMesh
{

public:
	FatherMesh();
	~FatherMesh();
	Ray									GetReflectRay(Ray* I, Vec3d insec, Vec3d ntemp)
	{
		Vec3d n = ntemp;
		n.normalize();

		Ray R;
		R.m_dire = I->m_dire - 2.0 *(I->m_dire.dot(n))*n;
		R.m_origin = insec + eposlion * R.m_dire;

		return R;
	}
	virtual	Vec4d						GiveHdrPixelPosition(Vec2i hdrRecCenter)
	{
		cout << "error: it shuold not enter virtual function!" << endl;
		return Vec4d(0, 0, 0, 0);
	};
	virtual Vec3d						GetInsecColor(Vec4d insec, double ShortScale)
	{
		return m_color;
	};
	virtual Vec4d						GetIntersection(Ray* ray);
	virtual Vec3d						GetInsecNormal(Vec4d insec, Ray *ray);

	Vec3d								GetPointValue(Ray* ray, vector<Light*> lights, 
														vector<FatherMesh*> meshs, Vec4d Insec, Vec3d EyeDire);
	Vec3d								GetSpecularValue(Vec4d insec, Ray *I, vector<FatherMesh*> meshs)
	{
		// Get Normal
		Vec3d normal = GetInsecNormal(insec, I);
		
		// Get Reflect
		Ray reflect = GetReflectRay(I, toVec3d(insec), normal);

		// Get LightInsec
		Vec4d reInsec = Vec4d(0, 0, 0, DBL_MAX);
		Vec3d Specular = Vec3d(0, 0, 0);

		int Index_mesh = -1;
		double	RayInsecDistanceMin = DBL_MAX;									// Min Distance for Insec
		for (int i = 0; i < meshs.size(); i++)
		{
			Vec4d reInsecTemp = meshs[i]->GetIntersection(&reflect);
			// hit nothing
			if (DBL_MAX - reInsecTemp[3] < eposlion)		continue;
			double RayInsecDistance;

			// Update RayInsecDistance
			if (meshs[i]->m_meshType == TriangleMesh && reInsecTemp[3] != DBL_MAX)	// Get TriMesh Insection distance
			{
				RayInsecDistance =
					(reflect.m_origin - toVec3d(reInsecTemp)).length();
			}
			else
			{
				RayInsecDistance = reInsecTemp[3];
			}

			if (RayInsecDistance < RayInsecDistanceMin)
			{
				reInsec = reInsecTemp;
				Index_mesh = i;
				Specular = meshs[i]->GetInsecColor(reInsec, 30000.0);

				// Update RayInsecDistanceMin
				if (meshs[Index_mesh]->m_meshType == TriangleMesh)
				{
					RayInsecDistanceMin =
						(reflect.m_origin - toVec3d(reInsec)).length();
				}
				else
				{
					RayInsecDistanceMin = reInsec[3];
				}
			}
		}

		if (Index_mesh > 0						// must hit something
			&& !meshs[Index_mesh]->IsLightPlane)	// hit other mesh
			return Vec3d(0, 0, 0);

		return Specular;
	}
	Vec3d								GetDiffuseValue(Vec4d insec, Ray *cameraRay, Light* light, 
														vector<FatherMesh*> meshs, Vec3d EyeDire);

	Vec3d								toVec3d(Vec4d v);

public:
	MeshType							m_meshType;
	Vec3d								m_color = Vec3d(1.0, 1.0, 1.0);
	bool								IsLightPlane;

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
	Plane(double A, double B, double C, double D, Vec3d normal, Vec3d color)
	{
		m_meshType = PlaneMesh;
		IsLightPlane = false;
		m_origin = Vec3d(DBL_MAX, DBL_MAX, DBL_MAX);
		m_end = Vec3d(DBL_MAX, DBL_MAX, DBL_MAX);

		m_normal = normal;
		m_color = color;

		A_ = A;
		B_ = B;
		C_ = C;
		D_ = D;
	};
	Plane(Vec3d PlaneOrigin, Vec3d PlaneEnd, Vec3d normal, 
		Vec2i hdrOrigin, double hdrPieceSide, double hdrOffset, Image* hdr)
	{
		m_meshType		= PlaneMesh;
		IsLightPlane	= true;
		m_hdr			= hdr;
		m_origin		= PlaneOrigin;
		m_normal		= normal;
		m_hdrPixelOrgin = hdrOrigin;
		m_hdrPixelEnd	= hdrOrigin + Vec2i(hdrPieceSide - 1, hdrPieceSide - 1);
		m_hdrOffset		= hdrOffset;
		m_end			= PlaneEnd;
		
		A_ = normal.x();
		B_ = normal.y();
		C_ = normal.z();
		D_ = -normal.dot(m_origin);

		UpdatePlaneUVDirec();
	}
	~Plane();

	Vec3d								GetInsecColor(Vec4d insec, double ShortScale)
	{
		if (IsLightPlane == true)
		{
			// Check is which axis is vertical
			// vertical axis x
			// left and right plane
			Vec2i hdrpixel;
			if (abs(m_origin.x() - insec.x()) < eposlion)
			{
				hdrpixel = GetHdrPixelIndex(
					m_origin.z() - insec.z(),
					m_origin.y() - insec.y());
			}
			
			// vertical axis y
			// up and down plane
			else if (abs(m_origin.y() - insec.y()) < eposlion)
			{
				hdrpixel = GetHdrPixelIndex(
					m_origin.x() - insec.x(),
					m_origin.z() - insec.z());
			}
			// vertical axis z
			// front and back plane
			else if (abs(m_origin.z() - insec.z()) < eposlion)
			{
				hdrpixel = GetHdrPixelIndex(
					m_origin.x() - insec.x(),
					m_origin.y() - insec.y());
			}
			
			// Get Hdr value
			//cout << m_hdrPixelOrgin.x() + hdrpixel.x() << "    ";
			//cout << m_hdrPixelOrgin.y() + hdrpixel.y() << endl;
			int x_pixel = m_hdrPixelOrgin.x() + hdrpixel.x();
			int y_pixel = m_hdrPixelOrgin.y() + hdrpixel.y();
			x_pixel = min(x_pixel, (int)m_hdr->width() - 1);
			y_pixel = min(y_pixel, (int)m_hdr->height() - 1);
			
			Color hdrColor = m_hdr->pixel(x_pixel,y_pixel);
			return ShortHdrColor(hdrColor, ShortScale);
		}
		else
		{
			return m_color;
		}
	}
	Vec2i								GetHdrPixelIndex(double x_, double y_)
	{
		Vec2i hdrpixel;
		hdrpixel[0] = (int)(abs(x_) / m_hdrOffset);
		hdrpixel[1] = (int)(abs(y_) / m_hdrOffset);
		return hdrpixel;
	};
	Vec3d								ShortHdrColor(Color hdrColor, double ShortScale)
	{
		Vec3d color;
		color[0] = min(hdrColor.red(),		ShortScale);
		color[1] = min(hdrColor.green(),	ShortScale);
		color[2] = min(hdrColor.blue(),		ShortScale);
		color = color / ShortScale * 255.0;
		return color;
	};
	bool								CheckEdgeRange(Vec3d Insec,int axis1, int axis2)
	{
		if ((max(m_origin[axis1], m_end[axis1]) - Insec[axis1] < -eposlion ||
			Insec[axis1] - min(m_origin[axis1], m_end[axis1]) < -eposlion))
			return false;
		if ((max(m_origin[axis2], m_end[axis2]) - Insec[axis2] < -eposlion ||
			Insec[axis2] - min(m_origin[axis2], m_end[axis2]) <-eposlion))
			return false;
		return true;
	};
	Vec4d								GetIntersection(Ray* ray);
	Vec3d								GetInsecNormal(Vec4d Insec, Ray *ray);
	Vec4d								GiveHdrPixelPosition(Vec2i hdrRecCenter)
	{
		if (hdrRecCenter.x() >= m_hdrPixelOrgin.x()
			&& hdrRecCenter.y() >= m_hdrPixelOrgin.y()
			&& hdrRecCenter.x() <= m_hdrPixelEnd.x()
			&& hdrRecCenter.y() <= m_hdrPixelEnd.y())
		{
			Vec3d pixelPosition = 
				m_origin + 
				UDirec * (double)(hdrRecCenter[0] - m_hdrPixelOrgin[0]) +
				VDirec*(double)(hdrRecCenter[1] - m_hdrPixelOrgin[1]);
			return Vec4d(pixelPosition.x(), pixelPosition.y(), pixelPosition.z(), 1);
		}
		return Vec4d(0, 0, 0, 0);
	};
	void								UpdatePlaneUVDirec()
	{
		if (abs(m_normal.x()) > eposlion&&abs(m_normal.y()) < eposlion&& abs(m_normal.z()) < eposlion)
		{
			// left:  u (0, 0, 1)   v (0, -1, 0)
			// right: u (0, 0, -1)  v (0, -1, 0)
			UDirec = Vec3d(0, 0,  mySign((m_end - m_origin).z())) * m_hdrOffset;
			VDirec = Vec3d(0, -1.0, 0) * m_hdrOffset;
		}
		// vertical y axis
		else if (abs(m_normal.x()) < eposlion&&abs(m_normal.y()) > eposlion&&abs(m_normal.z()) < eposlion)
		{
			// up:   u (-1, 0, 0)   v (0, 0, 1)
			// down: u (-1, 0, 0)   v (0, 0, -1)
			UDirec = Vec3d(-1.0, 0, 0) * m_hdrOffset;
			VDirec = Vec3d(0, 0, mySign((m_end - m_origin).z())) * m_hdrOffset;
		}
		// vertical z axis
		else if (abs(m_normal.x()) < eposlion&&abs(m_normal.y()) < eposlion&&abs(m_normal.z()) > eposlion)
		{
			// front:  u(-1, 0, 0)  v(0, -1, 0)
			// back:   u(-1, 0, 0)  v(0,  1, 0)
			UDirec = Vec3d(-1, 0, 0) * m_hdrOffset;
			VDirec = Vec3d(0, mySign((m_end - m_origin).y()), 0) * m_hdrOffset;
		}
	};
	inline double						mySign(double a)
	{
		return a > 0 ? 1.0 : -1.0;
	};

public:
	double								A_;
	double								B_;
	double								C_;
	double								D_;
	Vec3d								m_origin;		// left right point
	Vec3d								m_end;
	Vec3d								m_normal;
	Vec2i								m_hdrPixelOrgin;
	Vec2i								m_hdrPixelEnd;
	double								m_hdrOffset;
	PlaneType							m_planeType;
	Image								*m_hdr;
	Vec3d								UDirec, VDirec;
	
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
	Vec3d								GetInsecNormal(Vec4d Insec, Ray *ray);
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
	Vec3d								GetInsecNormal(Vec4d Insec, Ray *ray);
	bool								LoadFromFile(char* file);

private:
	KDNode								KDRoot_;
};
#endif