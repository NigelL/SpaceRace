#ifndef MATERIAL
#define MATERIAL
struct Component
{
	float r, g, b;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f)
	{
		Set(r, g, b);
	}
	void Set(float r, float g, float b)
	{
		this->r = r; this->g = g; this->b = b;
	}
};
struct Material
{
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess;

	Material() {

	}
	Material(Component _kAmbient, Component _kDiffuse, Component _kSpecular, float _kShininess) {
		kAmbient = _kAmbient;
		kDiffuse = _kDiffuse;
		kSpecular = _kSpecular;
		kShininess = _kShininess;
	}

};

#endif