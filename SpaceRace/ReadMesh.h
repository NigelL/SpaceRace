
#include <iostream> 
#include <string>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <vector>
#include "Vector3.h"



class MeshInfo {
public:
	std::string meshName;
	std::vector<double> pos;
	std::vector<double>	rot;
	std::vector<double> scale;
	std::vector<double> amt;
	std::vector<double> ambient;
	std::vector<double>	diffuse;
	std::vector<double> specular;
	std::vector<double> shininess;
	

	
};

MeshInfo ReadFromMesh(std::string path);