#include "MeshGroup.h"
#define eposlion 0.000001

enum LightType
{
	PointType = 1,
	AreaType = 2,
};
//-------------------------------------------------------------//
// Light
//-------------------------------------------------------------//
class Light
{
public:
	Light()				{};
	~Light()			{};
	virtual vector<Ray>	GetLightSourceRay(vector<Ray> LightSourceRays, Vec3d insec){ return LightSourceRays; };
	virtual Vec3d		GetPointColor(Vec3d p, Vec3d color, Vec3d normal, Vec3d EyeDire){ cout << "Light errer!"; return Vec3d(1, 1, 1); };
	LightType							m_LightType;
	Vec3d								m_center;
	Vec3d								m_color;
	double								m_lightStrength;
};
//-------------------------------------------------------------//
// Point Light
//-------------------------------------------------------------//
class PointLight : public Light
{
public:
	PointLight()
	{
		m_center = Vec3d(0.0, 10.0, -8.0);
		m_color = Vec3d(1.0, 1.0, 1.0);
		m_lightStrength = 10;
		m_LightType = PointType;
	}
	~PointLight(){}

	vector<Ray> GetLightSourceRay(vector<Ray> LightSourceRays, Vec3d insec)
	{
		Vec3d	direc = m_center - insec;		direc.normalize();
		insec += eposlion*direc;
		
		Ray		LightRay(insec, m_center);
		// Store distance between light point and center for block detection
		LightRay.m_d = (insec - m_center).length();

		LightSourceRays.push_back(LightRay);
		return LightSourceRays;
	}


	Vec3d GetPointColor(Vec3d p, Vec3d color, Vec3d normal, Vec3d EyeDire)
	{
		//-------------------------------------------
		// Phony Lighting Model
		//-------------------------------------------
		double ambient = 0.3;
		double shininess = 70.0f;

		// Light Direction
		Vec3d light_direction = m_center - p; // ?? p - center
		light_direction.normalize();
		
		// Normal
		normal.normalize();

		// Half vector
		Vec3d half_vector = light_direction + EyeDire;
		half_vector.normalize();

		// diffuse
		double diffuse = 0.6*max(0.0, normal.dot(light_direction));
		// specular
		double specular = 0.4*pow(max(0.0, normal.dot(half_vector)), shininess);

		//color = diffuse * color + specular * color;
		color = (ambient + diffuse)  * color + m_color * specular;

		//--------------------------------------------
		// Clamp
		//--------------------------------------------

		return Vec3d(color.x(), color.y(), color.z());
	}

};

class AreaLight : public Light
{
public: 
	AreaLight() 
	{
		m_LightType = AreaType;
	};
	~AreaLight() {};

private:
	double								A_, B_, C_, D_;	Vec3d								m_normal;
};