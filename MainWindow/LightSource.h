#include "MeshGroup.h"
#include "image.h"
#include "QuadTree.h"

#define eposlion 0.0000001
using namespace lw;
enum LightType
{
	PointType = 1,
	CubeType = 2,
};
//-------------------------------------------------------------//
// Light
//-------------------------------------------------------------//
class Light
{
public:
	Light()					{};
	~Light()				{};
	virtual Ray				GetLightSourceRay(Ray CameraRay, Vec3d insec){ return CameraRay; };
	virtual Vec3d			GetPointColor(Vec3d p, Vec3d color, Vec3d normal, Vec3d EyeDire){ cout << "Light errer!"; return Vec3d(1, 1, 1); };
	LightType				m_LightType;
	Vec3d					m_center;
	Vec3d					m_color;
	double					m_lightStrength;

	// Cube light
	vector<FatherMesh*>		planeList_;
	Plane					*left_plane_;
	Plane					*right_plane_;
	Plane					*front_plane_;
	Plane					*back_plane_;
	Plane					*up_plane_;
	Plane					*down_plane_;

};
//-------------------------------------------------------------//
// Point Light
//-------------------------------------------------------------//
class PointLight : public Light
{
public:
	PointLight()
	{
		m_center = Vec3d(0.0, 10.0, -20.0);
		m_color = Vec3d(1.0, 1.0, 1.0);
		m_lightStrength = 10;
		m_LightType = PointType;
	}
	
	PointLight(Vec3d center, Vec3d color)
	{
		m_center = center;
		m_color = color;
		m_lightStrength = 10;
		m_LightType = PointType;
	}
	~PointLight(){}

	Ray		GetLightSourceRay(Ray CameraRay, Vec3d insec)
	{
		Vec3d	direc = m_center - insec;		direc.normalize();
		Ray		LightRay(insec + eposlion*direc, m_center);
		// Store distance between light point and center for block detection
		LightRay.m_d = (insec - m_center).length();

		return LightRay;
	}

	Vec3d GetPointColor(Vec3d p, Vec3d color, Vec3d normal, Vec3d EyeDire)
	{
		//-------------------------------------------
		// Phony Lighting Model
		//-------------------------------------------
		//double ambient = 0.3;
		//double shininess = 70.0f;

		//// Light Direction
		//Vec3d light_direction = m_center - p; // ?? p - center
		//light_direction.normalize();
		//
		//// Normal
		//normal.normalize();

		//// Half vector
		//Vec3d half_vector = light_direction + EyeDire;
		//half_vector.normalize();	

		//// diffuse
		//double diffuse = 0.6*max(0.0, normal.dot(light_direction));
		//// specular
		//double specular = 0.4*pow(max(0.0, normal.dot(half_vector)), shininess);

		////color = diffuse * color + specular * color;
		//color = (ambient + diffuse)  * color + m_color * specular;

		//--------------------------------------------
		// Direction Diffuse
		//--------------------------------------------

		// Light Direction
		Vec3d light_direction = m_center - p; // ?? p - center
		light_direction.normalize();

		// Normal
		normal.normalize();
		double diffuseParam = max(0.0, normal.dot(light_direction));
		Vec3d diffuseValue = 
			 diffuseParam *
			 Vec3d(m_color.x() * color.x(), m_color.y() * color.y(), m_color.z() * color.z());

		return Vec3d(diffuseValue.x(), diffuseValue.y(), diffuseValue.z());
	}

};

class CubeLight : public Light
{
public: 
	CubeLight() 
	{
		m_LightType = CubeType;
	};
	
	CubeLight(Image *hdr, double sideLength)
	{
		m_LightType = CubeType;
		hdrShortScale = 500;
		hdrThreshold = 60000;		// for build QuadTree and scale final Sample PointLight
		hdrChannalControlParm = 5;  // for scale final Sample PointLight consider the hdrThreshold is for the sum of r,g,b
		M_hdr = hdr ;

		if (hdr->width() != hdr->height() * 3 / 4)
		{
			cout << "error: this hdr is wrong!" << endl;
			return;
		}

		CreatCubePlane(sideLength*0.5, hdr->height() / 4, hdr);
		// Creat sum image
		quadTree.BuildSumImage(hdr, hdrShortScale);
		// Using sum image to build quadTree
		quadTree.BuildQuadTree(quadTree.rootNode_, 0, hdrThreshold);
		// Sample light
		CreatSamplePointLight(quadTree.rootNode_);
	};
	
	~CubeLight() 
	{
	};

	Ray		GetLightSourceRay(Ray CameraRay, Vec3d insec)
	{ 
		return CameraRay;
	};

	void	CreatCubePlane(double halfSide, int hdrPieceSide, Image* hdr)
	{
		double HdrOffset = 2.0 * halfSide * 1.0/ hdrPieceSide;
		// new Plane (Origin, Normal , HdrOrigin, HdrOffset)
		//--------------------------------------------------------
		// left
		left_plane_ = new Plane(
			Vec3d(halfSide, halfSide, -halfSide),
			Vec3d(halfSide, -halfSide, halfSide),
			Vec3d(-1, 0, 0),
			Vec2i(0, hdrPieceSide),
			hdrPieceSide,
			HdrOffset,
			hdr
			);
		left_plane_->m_planeType = LeftType;
		planeList_.push_back(left_plane_);

		//--------------------------------------------------------
		// right
		right_plane_ = new Plane(
			Vec3d(-halfSide, halfSide, halfSide),
			Vec3d(-halfSide, -halfSide, -halfSide),
			Vec3d(1, 0, 0), 
			Vec2i(2*hdrPieceSide, hdrPieceSide),
			hdrPieceSide,
			HdrOffset,
			hdr
			);
		right_plane_->m_planeType = RightType;
		planeList_.push_back(right_plane_);

		//--------------------------------------------------------
		// up
		up_plane_ = new Plane(
			Vec3d(halfSide, halfSide, -halfSide),
			Vec3d(-halfSide, halfSide, halfSide),
			Vec3d(0, -1, 0),
			Vec2i(hdrPieceSide, 0),
			hdrPieceSide,
			HdrOffset,
			hdr
			);
		up_plane_->m_planeType = UpType;
		planeList_.push_back(up_plane_);

		//--------------------------------------------------------
		// down
		down_plane_ = new Plane(
			Vec3d(halfSide, -halfSide, halfSide),
			Vec3d(-halfSide, -halfSide, -halfSide),
			Vec3d(0, 1, 0),
			Vec2i(hdrPieceSide, 2 * hdrPieceSide),
			hdrPieceSide,
			HdrOffset,
			hdr
			);
		down_plane_->m_planeType = DownType;
		planeList_.push_back(down_plane_);

		//--------------------------------------------------------
		//  front
		front_plane_ = new Plane(
			Vec3d(halfSide, halfSide, halfSide),
			Vec3d(-halfSide, -halfSide, halfSide),
			Vec3d(0, 0, -1),
			Vec2i(hdrPieceSide, hdrPieceSide),
			hdrPieceSide,
			HdrOffset,
			hdr
			);
		front_plane_->m_planeType = FrontType;
		planeList_.push_back(front_plane_);

		//--------------------------------------------------------
		// back
		back_plane_ = new Plane(
			Vec3d(halfSide, -halfSide, -halfSide),
			Vec3d(-halfSide, halfSide, -halfSide),
			Vec3d(0, 0, 1),
			Vec2i(hdrPieceSide, 3*hdrPieceSide),
			hdrPieceSide,
			HdrOffset,
			hdr
			);
		back_plane_->m_planeType = BackType;
		planeList_.push_back(back_plane_);
	}

	void	CreatSamplePointLight(QuadTree::QuadNode *n)
	{
		//
		if (n->childEmpty)
		{
			Vec4d lightCenter = GetSampleLightCenter(n->originPixel, n->endPixel);
			if (lightCenter[3]) // center is on cube
			{
				PointLight* SampleLight = 
					new PointLight(
					Vec3d(lightCenter.x(), lightCenter.y(), lightCenter.z()),
					n->rgbSum / (double)hdrThreshold *2.0);
				m_SampleLights.push_back(SampleLight);
			}
		}
		else
		{
			CreatSamplePointLight(n->c0);
			CreatSamplePointLight(n->c1);
			CreatSamplePointLight(n->c2);
			CreatSamplePointLight(n->c3);
		}
	}

	Vec4d	GetSampleLightCenter(Vec2i hdrOrigin, Vec2i hdrEnd)
	{
		//bool IsRight = false;
		Vec4d LightCenter;
		for (int i = 0; i < planeList_.size(); i++)
		{
			LightCenter = planeList_[i]->GiveHdrPixelPosition((hdrOrigin + hdrEnd)/2);
			if (LightCenter[3])
			{
				//IsRight = true;
				break;
			}
		}
		//if (!IsRight)
		//	cout << "error: cant get this pixel position!" << endl;
		return LightCenter;
	}


	vector<PointLight*>					m_SampleLights;

private:

	Image								*M_hdr;
	Vec3d								m_origin;
	Vec3d								m_normal;
	QuadTree							quadTree;
	double								hdrShortScale;
	double								hdrThreshold;
	double								hdrChannalControlParm;
};