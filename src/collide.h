
#ifndef COLLIDE_H
#define COLLIDE_H

#include "mathutils.h"

union FeaturePair
{
	struct Edges
	{
		char inEdge1;
		char outEdge1;
		char inEdge2;
		char outEdge2;
	} e;
	int value;
};

struct Contact
{
	struct Vec2 position;
	struct Vec2 normal;
	struct Vec2 r1, r2;
	float separation;
	float Pn;	// accumulated normal impulse
	float Pt;	// accumulated tangent impulse
	float Pnb;	// accumulated normal impulse for position bias
	float massNormal, massTangent;
	float bias;
	union FeaturePair feature;
};

struct Body
{
//	Body();
//	void Set(const Vec2& w, float m);

//	void AddForce(const Vec2& f)
//	{
	//	force += f;
//	}

	struct Vec2 position;
	float rotation;

	struct Vec2 velocity;
	float angularVelocity;

	struct Vec2 force;
	float torque;

	struct Vec2 width;

	float friction;
	float mass, invMass;
	float I, invI;
};

int Collide(struct Contact* contacts, struct Body* bodyA, struct Body* bodyB);

#endif
