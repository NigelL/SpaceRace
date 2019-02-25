#include "MeshBuilder.h"
#include <iostream>
#include "LoadOBJ.h"
#include <GL\glew.h>
#include "Vertex.h"
#include <vector>

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/


Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const
	std::string &file_path) {
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices,
		uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data,
		vertex_buffer_data);

	//Create the mesh and call glBindBuffer, glBufferData
	//Use triangle list and remember to set index size

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0],
		GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	
	return mesh;
}


Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0],
		GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;
	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow,
	unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;


	float width = 1.0f / numCol;
	float height = 1.0f / numRow;
	for (unsigned j = 0; j < numRow; ++j)
	{
		for (unsigned i = 0; i < numCol; ++i)
		{
			v.pos.Set(0, 1, 0);
			v.texCoord.Set(width * i, (height * j) + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(1, 1, 0);
			v.texCoord.Set((width * i) + width, (height * j) + height);
			vertex_buffer_data.push_back(v);
			std::cout << (width * i) + width << std::endl;

			v.pos.Set(0, 0, 0);
			v.texCoord.Set(width * i, (height * j));
			vertex_buffer_data.push_back(v);

			v.pos.Set(1, 0, 0);
			v.texCoord.Set((width * i) + width, (height * j));
			vertex_buffer_data.push_back(v);

			
										
			index_buffer_data.push_back((j * numCol * 4) + (i * 4) + 1);
			index_buffer_data.push_back((j * numCol * 4) + (i * 4));
			index_buffer_data.push_back((j * numCol * 4) + (i * 4) + 2);

			index_buffer_data.push_back((j * numCol * 4) + (i * 4) + 1);
			index_buffer_data.push_back((j * numCol * 4) + (i * 4) + 2);
			index_buffer_data.push_back((j * numCol * 4) + (i * 4) + 3);

			

			//Task: Add 4 vertices into vertex_buffer_data
			//Task: Add 6 indices into index_buffer_data
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof
	(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof
	(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}
Mesh* MeshBuilder::GenerateRealQuad(const std::string &meshName, Color color, float lengthX, float lengthY)
{

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.color.Set(color.r, color.g, color.b);
	v.pos.Set(lengthX, lengthY, 0);
	v.normal.Set(0, 1, 3.0);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX, lengthY, 0);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX, -lengthY, 0);
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX, -lengthY, 0);
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v);



	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),
		&index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}


float DegreeToRadian(float value)
{
	return value * 3.142f / 180.0f;
}



Mesh* MeshBuilder::GenerateCircle(const std::string &meshName,Color color, float
	lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	for (int theta = 0; theta <= 360; theta += 10)
	{
		float x = 1 * cosf(DegreeToRadian((float)theta));
		float y = 1 * sinf(DegreeToRadian((float)theta));
		v.pos.Set(x, 0, y);
		v.color.Set(1, 0, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, 0, 0);
		v.color.Set(1, 0, 0);
		vertex_buffer_data.push_back(v);
	}
	std::vector<GLuint> index_buffer_data;
	for (int i = 0; i < (int)vertex_buffer_data.size(); ++i)
	{
		index_buffer_data.push_back(i);
	}
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof
	(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof
	(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;

}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
/*
Vector3* NormalisePoints(std::vector<Vertex>& vertData) {
	for (int i = 0; i < vertData.size() - 1; i++) {
		Vector3 aOper = Vector3(vertData[i].pos.x - vertData[i + 1].pos.x,vertData[i].pos.y - vertData[i + 1].pos.y,vertData[i].pos.z - vertData[i +1].pos.z);
		
	}
	

}
*/

Mesh* MeshBuilder::GenerateQuad(const std::string &meshName,Color color, float lengthX, float lengthY)
{

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	
	v.normal.Set(0, 1, 3.0);
	for (int i = 0; i < lengthY + 1; i++) {
		for (int j = 0; j < lengthX + 1; j++) {
			v.pos.Set(j / (lengthX + 1), 0, i / (lengthY + 1));
			v.texCoord.Set((j / (lengthX + 1)), (i / (lengthY + 1)));
			v.color.Set(color.r, color.g, color.b);			
			vertex_buffer_data.push_back(v);

		}
	}

	std::vector<GLuint> index_buffer_data;


	for (int i = 0; i < lengthY; i++) {
		for (int j = 0; j < lengthX; j++) {
			index_buffer_data.push_back(i * (lengthX + 1) + j);
			index_buffer_data.push_back(i * (lengthX + 1) + j + lengthX + 1);
			index_buffer_data.push_back(i * (lengthX + 1) + j + lengthX + 2);

			index_buffer_data.push_back(i * (lengthX + 1) + j);
			index_buffer_data.push_back(i * (lengthX + 1) + j + lengthX + 2);
			index_buffer_data.push_back(i * (lengthX + 1) + j + 1);

		}
	}
	



	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
	&vertex_buffer_data[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),
	&index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->allVertices = vertex_buffer_data;
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;
}

Mesh* MeshBuilder::GenerateMenu(const std::string &meshName, Color color, float length)
{
	Vertex v;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(-0.5 * length, -0.5f * length, 0);
	v.color = color;
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5 * length, -0.5f * length, 0);
	v.color = color;
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5 * length, 0.5f * length, 0);
	v.color = color;
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5 * length, 0.5f * length, 0);
	v.color = color;
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}
/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName,Color color, float lengthX, float lengthY, float lengthZ)
{
	/*
	std::vector<Vertex> vertexBuffer;
	Vertex v;
	v.color = color;

	v.pos.Set(lengthX, lengthY, lengthZ);
	v.normal.Set(0, 0, lengthZ);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, lengthY, lengthZ );
	v.normal.Set(0, 0, lengthZ );
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, -lengthY, lengthZ );
	v.normal.Set(0, 0, lengthZ );
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX , -lengthY, lengthZ );
	v.normal.Set(0, 0, lengthZ );
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX, lengthY, -lengthZ);
	v.normal.Set(0, 0, -lengthZ);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, lengthY, -lengthZ);
	v.normal.Set(0, 0, -lengthZ);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, -lengthY, -lengthZ);
	v.normal.Set(0, 0, -lengthZ);
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX, -lengthY, -lengthZ);
	v.normal.Set(0, 0, -lengthZ);
	vertexBuffer.push_back(v);

	v.pos.Set(lengthX , lengthY, lengthZ);
	v.normal.Set(lengthX , 0, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX , lengthY, lengthZ);
	v.normal.Set(-lengthX, 0, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX , -lengthY, lengthZ );
	v.normal.Set(-lengthX, 0, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX , -lengthY, lengthZ );
	v.normal.Set(lengthX, 0, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX, lengthY, -lengthZ);
	v.normal.Set(lengthX, 0, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, lengthY, -lengthZ);
	v.normal.Set(-lengthX, 0, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, -lengthY, -lengthZ);
	v.normal.Set(-lengthX, 0, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX, -lengthY, -lengthZ);
	v.normal.Set(lengthX, 0, 0);
	vertexBuffer.push_back(v);

	v.pos.Set(lengthX , lengthY, lengthZ );
	v.normal.Set(0, lengthY, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, lengthY, lengthZ );
	v.normal.Set(0, lengthY, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX , -lengthY, lengthZ);
	v.normal.Set(0, -lengthY, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX , -lengthY, lengthZ );
	v.normal.Set(0, -lengthY, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX, lengthY, -lengthZ);
	v.normal.Set(0, lengthY, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, lengthY, -lengthZ);
	v.normal.Set(0, lengthY, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(-lengthX, -lengthY, -lengthZ);
	v.normal.Set(0, -lengthY, 0);
	vertexBuffer.push_back(v);
	v.pos.Set(lengthX, -lengthY, -lengthZ);
	v.normal.Set(0, -lengthY, 0);
	vertexBuffer.push_back(v);



	std::vector<GLuint> index_buffer_data;
	int startIndex = 0;
	for (startIndex = 0; startIndex < 3; startIndex++) {
		index_buffer_data.push_back(startIndex * 8);
		index_buffer_data.push_back(startIndex * 8 + 1);
		index_buffer_data.push_back(startIndex * 8 + 2);

		index_buffer_data.push_back(startIndex * 8);
		index_buffer_data.push_back(startIndex * 8 + 2);
		index_buffer_data.push_back(startIndex * 8 + 3);

		index_buffer_data.push_back(startIndex * 8 + 5);
		index_buffer_data.push_back(startIndex * 8 + 4);
		index_buffer_data.push_back(startIndex * 8 + 7);

		index_buffer_data.push_back(startIndex * 8 + 5);
		index_buffer_data.push_back(startIndex * 8 + 7);
		index_buffer_data.push_back(startIndex * 8 + 6);

		index_buffer_data.push_back(startIndex * 8 + 3);
		index_buffer_data.push_back(startIndex * 8 + 4);
		index_buffer_data.push_back(startIndex * 8 + 5);

		index_buffer_data.push_back(startIndex * 8 + 3);
		index_buffer_data.push_back(startIndex * 8 + 5);
		index_buffer_data.push_back(startIndex * 8 + 6);

		index_buffer_data.push_back(startIndex * 8 + 4);
		index_buffer_data.push_back(startIndex * 8);
		index_buffer_data.push_back(startIndex * 8 + 3);

		index_buffer_data.push_back(startIndex * 8 + 4);
		index_buffer_data.push_back(startIndex * 8 + 3);
		index_buffer_data.push_back(startIndex * 8 + 7);

		index_buffer_data.push_back(startIndex * 8 + 1);
		index_buffer_data.push_back(startIndex * 8 + 5);
		index_buffer_data.push_back(startIndex * 8 + 6);

		index_buffer_data.push_back(startIndex * 8 + 1);
		index_buffer_data.push_back(startIndex * 8 + 6);
		index_buffer_data.push_back(startIndex * 8 + 2);

		index_buffer_data.push_back(startIndex * 8 + 5);
		index_buffer_data.push_back(startIndex * 8 + 1);
		index_buffer_data.push_back(startIndex * 8);

		index_buffer_data.push_back(startIndex * 8 + 5);
		index_buffer_data.push_back(startIndex * 8);
		index_buffer_data.push_back(startIndex * 8 + 4);
	}


	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex),
		&vertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),
		&index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;
	*/

	// An array of 3 vectors which represents 3 vertices
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color;
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, -0.5f, -0.5f); v.color = color;
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, 0.5f, -0.5f); v.color = color;
		vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f, 0.5f, -0.5f); v.color = color;
		vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f, -0.5f, 0.5f); v.color = color;
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, -0.5f, 0.5f); v.color = color;
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.5f, 0.5f, 0.5f); v.color = color;
		vertex_buffer_data.push_back(v);
		v.pos.Set(-0.5f, 0.5f, 0.5f); v.color = color;
		vertex_buffer_data.push_back(v);

		std::vector<GLuint> index_buffer_data;
		for (int i = 0; i < 3; i++) {
			index_buffer_data.push_back(i * 8 + 7);
			index_buffer_data.push_back(i * 8 + 4);
			index_buffer_data.push_back(i * 8 + 6);
			index_buffer_data.push_back(i * 8 + 5);
			index_buffer_data.push_back(i * 8 + 6);
			index_buffer_data.push_back(i * 8 + 4);
			index_buffer_data.push_back(i * 8 + 6);
			index_buffer_data.push_back(i * 8 + 5);
			index_buffer_data.push_back(i * 8 + 2);
			index_buffer_data.push_back(i * 8 + 1);
			index_buffer_data.push_back(i * 8 + 2);
			index_buffer_data.push_back(i * 8 + 5);
			index_buffer_data.push_back(i * 8 + 3);
			index_buffer_data.push_back(i * 8 + 7);
			index_buffer_data.push_back(i * 8 + 2);
			index_buffer_data.push_back(i * 8 + 6);
			index_buffer_data.push_back(i * 8 + 2);
			index_buffer_data.push_back(i * 8 + 7);
			index_buffer_data.push_back(i * 8 + 2);
			index_buffer_data.push_back(i * 8 + 1);
			index_buffer_data.push_back(i * 8 + 3);
			index_buffer_data.push_back(i * 8 + 0);
			index_buffer_data.push_back(i * 8 + 3);
			index_buffer_data.push_back(i * 8 + 1);
			index_buffer_data.push_back(i * 8 + 3);
			index_buffer_data.push_back(i * 8 + 0);
			index_buffer_data.push_back(i * 8 + 7);
			index_buffer_data.push_back(i * 8 + 4);
			index_buffer_data.push_back(i * 8 + 7);
			index_buffer_data.push_back(i * 8 + 0);
			index_buffer_data.push_back(i * 8 + 4);
			index_buffer_data.push_back(i * 8 + 0);
			index_buffer_data.push_back(i * 8 + 5);
			index_buffer_data.push_back(i * 8 + 1);
			index_buffer_data.push_back(i * 8 + 5);
			index_buffer_data.push_back(i * 8 + 0);

		}
		Mesh *mesh = new Mesh(meshName);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
		
		mesh->indexSize = index_buffer_data.size();
		mesh->mode = Mesh::DRAW_TRIANGLES;
		return mesh;


}

Mesh* MeshBuilder::GenerateTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 360.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float x1, z1;
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			z1 = outerR * cos(Math::DegreeToRadian(stack * degreePerStack));
			x1 = outerR * sin(Math::DegreeToRadian(stack * degreePerStack));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * cos(Math::DegreeToRadian(stack * degreePerStack));
			y2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice)));
			z2 = innerR * sin(Math::DegreeToRadian(slice * degreePerSlice));

			v.pos.Set(x1, z1, y2);
			v.color = color;

			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}
	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer); // bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateHemiSphere(const std::string &meshName, Color color, float radius) {
	std::vector<Vertex> vertex_buffer_data;
	Vertex v;
	v.color = color;

	std::vector<GLuint>	 index_buffer_data;

	double deg = 0.0, polarDeg = 0.0;

	for (polarDeg = 0.0; polarDeg <= 90; polarDeg += 10.0) {	
		for (deg = 0.0; deg <= 360.0; deg += 10.0) {

			double x = cos(DegreeToRadian((float)deg)) * radius * sin(DegreeToRadian((float)polarDeg));
			double y = radius * sin(DegreeToRadian((float)deg)) * sin(DegreeToRadian((float)polarDeg));
			double z = radius * cos(DegreeToRadian((float)polarDeg));

			v.pos.Set((float)x, (float)y, (float)z);
			v.normal.Set((float)x, (float)y, (float)z);
			vertex_buffer_data.push_back(v);
		}
	}

	for (int i = 0; i < (int)vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
		index_buffer_data.push_back(i + 36);
	}



	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),
		&index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height){
	std::vector<Vertex> vertex_buffer_data;
	Vertex v;
	v.color = color;
	float amountSlice = (float)360.0f/ (float)numSlice;
	float amountStack = (float)180.0f / (float)numStack;
	float  stackHeight = height	 / numStack;
	std::vector<GLuint>	 index_buffer_data;


	for (int curStack = 0; curStack < (int)numStack + 1; curStack++) {
		for (int curSlice = 0; curSlice < (int)numSlice + 1; curSlice++) {			
			v.pos.Set(radius * cos(DegreeToRadian(curSlice * (float)amountSlice)), -height / 2 + curStack * stackHeight, radius*sin(DegreeToRadian(curSlice * (float)amountSlice)));
			vertex_buffer_data.push_back(v);

		}
	}



	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back(stack * (numSlice) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice) + slice);
		}

	}



	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),
		&index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_FAN;
	return mesh;

}
Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, float radius) {

	std::vector<Vertex> vertex_buffer_data;
	Vertex v;
	
	//v.color = color;
	
	std::vector<GLuint>	 index_buffer_data;
	
	float deg = 0.0,polarDeg = 0.0;	
	
	for (polarDeg = 0.0; polarDeg <= 180; polarDeg += 10.0) {
		for (deg = 0.0; deg <= 360.0; deg += 10.0) {

			float x = cos(DegreeToRadian(deg)) * radius * sin(DegreeToRadian(polarDeg));
			float y = radius * cos(DegreeToRadian(polarDeg));
				
			float z = radius * sin(DegreeToRadian(deg)) * sin(DegreeToRadian(polarDeg));

			//std::cout << "X : " << x / radius << " Y " << y / radius  << " Z : " << z / radius << std::endl;
			v.pos.Set(x, z, y);			
			v.normal.Set((float)x, (float)z	 , (float)y );			
			vertex_buffer_data.push_back(v);
		}
	}			

	for (int i = 0; i < (int)vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
		index_buffer_data.push_back(i + 36);
	}

	
	
	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),
		&index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	return mesh;

}