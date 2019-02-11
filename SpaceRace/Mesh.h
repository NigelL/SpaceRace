#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "Material.h"

#include "Vertex.h"

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_FAN,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	Mesh();

	~Mesh();
	void Render();
	void Render(unsigned offset, unsigned count);
	void UpdateVertices();
	

	std::vector<Vertex> allVertices;
	Mesh& operator=(const Mesh& curMesh);

	std::string name;
	DRAW_MODE mode;
	
	Material material;
	unsigned vertexBuffer;
	unsigned colorBuffer;
	unsigned indexBuffer;
	unsigned indexSize;
	unsigned textureID;
};

#endif