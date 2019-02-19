#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName,Color color, float length);
	static Mesh* GenerateCube(const std::string &meshName,Color color, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string
		&file_path);
	static Mesh* MeshBuilder::GenerateCircle(const std::string &meshName,Color color, float
		lengthX, float lengthY, float lengthZ);
	static Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, float
		radius);
	static Mesh* MeshBuilder::GenerateHemiSphere(const std::string &meshName, Color color, float
		radius);
	static Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height);
	static Mesh* MeshBuilder::GenerateTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);
	static Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow,
		unsigned numCol);

};

#endif