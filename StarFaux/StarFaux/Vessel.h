#ifndef _VESSEL_H_
#define _VESSEL_H_

#include "Shape.h"
#include "MathExtension.h"
#include "ExternalModel.h"

class Vessel : public ExternalModel {
public:
	Vessel(GLuint program, vec4 color, Camera* camera);
	void setAccelerationX(float acc);
	void setAccelerationY(float acc);
	void setAccelerationZ(float acc);
	void updateMovement();
	vec3 getAcceleration();
	vec3 getVelocity();
	void draw(DrawType type, Camera& camera, Light& light);

private:
	static const float MAX_VELOCITY;
	static const float VELOCITY_DECAY;
	void updateVelocity(float dTime);

	vec3 m_velocity;
	vec3 m_acceleration;

	float m_lastUpdateTime;

	Camera* m_camera;
};

#endif