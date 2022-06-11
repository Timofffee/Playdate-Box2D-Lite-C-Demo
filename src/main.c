//
//  main.c
//  bach.mid
//
//  Created by Dave Hayden on 5/10/21.
//  Copyright (c) 2021 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <float.h>

#include "pd_api.h"
#include "b2d_lite.h"

PlaydateAPI *pd = NULL;
const struct playdate_sound *snd = NULL;
const struct playdate_graphics *gfx = NULL;

#define CRATE_COUNT 60
int width = 256 * 3;
int height = 192 * 3;
float zoom = 10.0f;
float pan_y = 8.0f;

struct Body ground, crate, crates[CRATE_COUNT];

struct Vec2 initVec2(float x, float y)
{
	struct Vec2 res;
	res.x = x;
	res.y = y;
	return res;
}

int update(void *ud);
void Init(void);

#ifdef _WINDLL
__declspec(dllexport)
#endif
	int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg;

	if (event == kEventInit)
	{
		pd = playdate;
		snd = playdate->sound;
		gfx = playdate->graphics;

		Init();

		pd->system->setUpdateCallback(update, NULL);
	}

	return 0;
}

static inline int isButtonDown(PDButtons button)
{
	PDButtons down;
	pd->system->getButtonState(NULL, &down, NULL);
	return down & button;
}

static inline int getRand(int min, int max)
{
	return min + (rand() % (max - min));
}

static inline void drawLineV(struct Vec2 a, struct Vec2 b, LCDColor color)
{
	pd->graphics->drawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y, 1, color);
}

void DrawBody(struct Body *body)
{
	struct Mat22 R;
	initMat22(&R, body->rotation);

	struct Vec2 x = body->position;
	struct Vec2 h = scaledVec2(0.5f, body->width);

	struct Vec2 v1 = sumVec2(x, matmul(R, initVec2(-h.x, -h.y)));
	struct Vec2 v2 = sumVec2(x, matmul(R, initVec2(h.x, -h.y)));
	struct Vec2 v3 = sumVec2(x, matmul(R, initVec2(h.x, h.y)));
	struct Vec2 v4 = sumVec2(x, matmul(R, initVec2(-h.x, h.y)));

	drawLineV(v1, v2, kColorBlack);
	drawLineV(v2, v3, kColorBlack);
	drawLineV(v3, v4, kColorBlack);
	drawLineV(v4, v1, kColorBlack);
}

void Init(void)
{
	iterations = 20;
	gravity.x = 0.0f;
	gravity.y = 50.0f;

	struct Vec2 size;
	size.x = 300.f;
	size.y = 40.0f;

	struct Vec2 pos;
	pos.x = 200;
	pos.y = 240 + -0.5f * size.y;

	initBody(&ground, size, FLT_MAX);
	ground.position = pos;

	addBody(&ground);

	size.x = 20.0f;
	size.y = 20.0f;

	for (int i = 0; i < CRATE_COUNT; ++i)
	{
		initBody(&crates[i], size, 1.0f);
		crates[i].friction = 0.2f;

		float x = RandomRange(-100.0f, 100.0f);
		pos.x = 200.0f + x;
		pos.y = 100.0f - 20.05f * i;
		crates[i].position = pos;

		addBody(&crates[i]);
	}

	return;
}

int update(void *ud)
{
	(void)ud; // unused

	// Update
	//----------------------------------------------------------------------------------
	// Physics body creation inputs
	// if (isButtonDown(kButtonA))
	// 	CreatePhysicsBodyPolygon((Vector2){getRand(180, 220), 20}, getRand(10, 40), getRand(3, 8), 1);
	// else if (isButtonDown(kButtonB))
	// 	CreatePhysicsBodyCircle((Vector2){getRand(180, 220), 20}, getRand(5, 25), 1);

	// Destroy falling physics bodies

	pd->graphics->clear(kColorWhite);

	// Draw created physics bodies
	float dt = 1.0f/30.f;
	Step(dt);
	
	DrawBody(&ground);        
	
	for (int i=0;i<CRATE_COUNT;i++)
		DrawBody(&crates[i]);

	pd->system->drawFPS(0, 0);
	return 1;
}
