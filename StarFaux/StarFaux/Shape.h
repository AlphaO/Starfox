#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Angel.h"
#include "Quaternion.h"
#include "Camera.h"
#include "Light.h"
#include "Types.h"
#include "Tga.h"
#include <string>
#include "BoundingBox.h"

class Shape {
public:
	Shape();
	~Shape();

	// Call this after setting up lighting and textures!
	void initDraw();

	void draw(DrawType type, Camera& camera, Light* light, lightEffects effects);
	void setupTexture(TextureUseType useType, TextureSamplingType samplingType, 
					  TextureWrappingType wrappingType, std::string textureName);
	void setupLighting(float shininess, vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular);
	void rotate(Quaternion q);
	void resetRotation();
	void scale(float amount);
	void scale(vec3 amount);
	void translate(float x, float y, float z);
	void setPosition(vec3 position);
	void resetTranslation();
	void update();
	mat4 m_objectToWorld;
	Quaternion m_qRotation;

	BoundingShape* m_shape;
	vec3 m_position;

protected:
	enum ShapeType {
		LINE, SPHERE, CUBE, EXTERNAL_MODEL, VESSEL
	};

	ShapeType m_shapeType;

	// Transformations
	
	vec3 m_scale;
	
	bool m_modified;

	// Lighting
	ShadingType m_shading;
	float m_shininess;
	vec4 m_materialAmbient;
	vec4 m_materialDiffuse;
	vec4 m_materialSpecular;

	// Shape
	int m_numVertices;
	int m_vertexIndex;
	vec4* m_vertices;
	vec3* m_normals;
	vec2* m_textureCoords;
	
	// Texture
	int m_useTexture;
	std::string m_textureName;
	TgaImage m_textureImage;
	TextureSamplingType m_samplingType;
	TextureWrappingType m_wrappingType;

	// OpenGL 
	GLuint m_program;
	GLuint m_vertexArrayObject;                      
    GLuint m_vertexBuffer;
	GLuint m_textureObject;
	GLuint m_textureBuffer;
};

#endif