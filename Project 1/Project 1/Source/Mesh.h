#ifndef __MESH_H__
#define __MESH_H__

#include "glm/vec3.hpp"

#include <string>

#define MAX_TEXTURES 2

class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES,
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};

	struct Position
	{
		float x, y, z;
		Position(float x = 0, float y = 0, float z = 0) { Set(x, y, z); }
		void Set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	};

	struct Color
	{
		float r, g, b;
		Color(float r = 1, float g = 1, float b = 1) { Set(r, g, b); }
		void Set(float r, float g, float b) { this->r = r; this->g = g; this->b = b; }
	};

	struct TexCoord
	{
		float u, v;
		TexCoord(float u = 0, float v = 0) { Set(u, v); }
		void Set(float u, float v) { this->u = u; this->v = v; }
	};

	struct Material_Component
	{
		float r, g, b;
		Material_Component(float r = 0.1f, float g = 0.1f, float b = 0.1f) { Set(r, g, b); }
		void Set(float r, float g, float b) { this->r = r; this->g = g; this->b = b; }
	};

	struct Material
	{
		Material_Component kAmbient;
		Material_Component kDiffuse;
		Material_Component kSpecular;
		float kShininess;

		Material()
		{
			//some default values
			kAmbient.Set(0.1f, 0.1f, 0.1f);
			kDiffuse.Set(0.6f, 0.6f, 0.6f);
			kSpecular.Set(0.3f, 0.3f, 0.3f);
			kShininess = 5.f;
		}
	};

	struct Vertex
	{
		Position pos;
		Color color;
		//glm::vec3 normal;
		TexCoord texCoord;
		Vertex() {}
	};
public:

	Mesh(std::string meshName);
	virtual ~Mesh();
	virtual void Render();
	void Render(unsigned offset, unsigned count);

	DRAW_MODE mode;
	unsigned int vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	Material material;
	unsigned textureID;
	unsigned textureArray[MAX_TEXTURES];

	float alpha;
private:
	std::string m_sMeshName;
};

#endif
