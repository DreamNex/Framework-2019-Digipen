#ifndef __MESH_BUILDER_H__
#define __MESH_BUILDER_H__

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
	static Mesh* GenerateAxis(const std::string &meshName, float lengthX=1000.0f, float lengthY=1000.0f, float lengthZ=1000.0f);
	static Mesh* GenerateCrossHair(const std::string &meshName, float colour_r=1.0f, float colour_g=1.0f, float colour_b=0.0f, float length=1.0f);
	static Mesh* GenerateQuad(const std::string &meshName, Mesh::Color color, float length = 1.f);
	static Mesh* GenerateCube(const std::string &meshName, Mesh::Color color, float length = 1.f);
	static Mesh* GenerateRing(const std::string &meshName, Mesh::Color color, unsigned numSlice, float outerR = 1.f, float innerR = 0.f);
	static Mesh* GenerateSphere(const std::string &meshName, Mesh::Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateCone(const std::string &meshName, Mesh::Color color, unsigned numSlice, float radius, float height);
};

#endif