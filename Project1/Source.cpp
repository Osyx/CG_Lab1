#include "SDL.h"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "SDLauxiliary.h"
#include <ctime>
#include <math.h>

using namespace std;
using glm::vec3;
using glm::vec2;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int F = SCREEN_HEIGHT / 2;
SDL_Surface* screen;
vector<vec3> stars(1000);
int t;
float V = 0.03;

void update();
void initStars();
void initRainbow();
void rainbowDraw(vector<vec3> left, vector<vec3> right);
void interpolate(float a, float b, vector<float>& result);
void interpolate(vec3 a, vec3 b, vector<vec3>& result);
void testInterpolate();
void starsDraw();
void movement(int s, float dt);
vec2 conv3Dto2D(vec3 point);

int main(int argc, char* argv[]) {
	screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
	testInterpolate();
	initRainbow();
	initStars();
	
	return 0;
}

void initRainbow() {
	vec3 topLeft(1, 0, 0);
	vec3 topRight(0, 0, 1);
	vec3 bottomLeft(0, 1, 0);
	vec3 bottomRight(1, 1, 0);
	vector<vec3> leftSide(SCREEN_HEIGHT);
	vector<vec3> rightSide(SCREEN_HEIGHT);
	interpolate(topLeft, bottomLeft, leftSide);
	interpolate(topRight, bottomRight, rightSide);

	while (NoQuitMessageSDL()) {
		rainbowDraw(leftSide, rightSide);
	}
	SDL_SaveBMP(screen, "rainbow.bmp");
}

void rainbowDraw(vector<vec3> left, vector<vec3> right) {

	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

	for (size_t y = 0; y < SCREEN_HEIGHT; ++y) {
		vector<vec3> row(SCREEN_WIDTH);
		interpolate(left[y], right[y], row);

		for (size_t x = 0; x < SCREEN_WIDTH; ++x) {
			PutPixelSDL(screen, x, y, row[x]);
		}
	}

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void interpolate(float a, float b, vector<float>& result) {
	if (result.size() == 1)
		0;
	float stepSize = (b - a) / (result.size() - 1);
	float step = 0;
	for (size_t i = 0; i < result.size(); ++i) {
		result[i] = a + step;
		step += stepSize;
	}
}

void interpolate(vec3 a, vec3 b, vector<vec3>& result) {
	if (result.size() == 1)
		0;
	float stepSizeX = (b.x - a.x) / (result.size() - 1);
	float stepSizeY = (b.y - a.y) / (result.size() - 1);
	float stepSizeZ = (b.z - a.z) / (result.size() - 1);
	float stepX = 0, stepY = 0, stepZ = 0;

	for (size_t i = 0; i < result.size(); ++i) {
		result[i].x = a.x + stepX;
		result[i].y = a.y + stepY;
		result[i].z = a.z + stepZ;
		stepX += stepSizeX;
		stepY += stepSizeY;
		stepZ += stepSizeZ;
	}
}

void testInterpolate() {
	cout << "Using float values:" << endl;
	vector<float> resultFloat(10);
	interpolate(5, 14, resultFloat);
	for (size_t i = 0; i < resultFloat.size(); ++i)
		cout << resultFloat[i] << " ";
	cout << "\n\nUsing glm::vec3:" << endl;;
	vector<vec3> result(4);
	vec3 a(1, 4, 9.2);
	vec3 b(4, 1, 9.8);
	interpolate(a, b, result);
	for (size_t i = 0; i<result.size(); ++i) {
		cout << "( "
			<< result[i].x << ", "
			<< result[i].y << ", "
			<< result[i].z << " ) ";
	}
}

void initStars() {
	srand((unsigned int) time(nullptr));
	for (size_t i = 0; i < stars.size(); ++i) {
		switch (rand() % 2) {
			case 0:
				stars[i].x = float(rand()) / float(RAND_MAX) - 1.0f;				
				break;
			default:
				stars[i].x = float(rand()) / float(RAND_MAX);
				break;
		}
		switch (rand() % 2) {
		case 0:
			stars[i].y = float(rand()) / float(RAND_MAX) - 1.0f;
			break;
		default:
			stars[i].y = float(rand()) / float(RAND_MAX);
			break;
		}
		stars[i].z = float(rand()) / float(RAND_MAX);
	}
	t = SDL_GetTicks();
	while (NoQuitMessageSDL()) {
		update();
		starsDraw();
	}
	SDL_SaveBMP(screen, "stars.bmp");
}

vec2 conv3Dto2D(vec3 point) {
	vec2 flatPoint(F * point.x / point.z + SCREEN_WIDTH / 2, F * point.y / point.z + SCREEN_HEIGHT / 2);
	return flatPoint;
}

void starsDraw() {
	SDL_FillRect(screen, 0, 0);
	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);
	vec3 color(1, 1, 1);
	for (size_t s = 0; s < stars.size(); ++s) {
		vec2 star = conv3Dto2D(stars[s]);
		PutPixelSDL(screen, (int) star.x, (int) star.y, color);
	}
	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void update() {
	int t2 = SDL_GetTicks();
	float dt = float(t2 - t);
	t = t2;
	for (int s = 0; s < stars.size(); ++s) {
		movement(s, dt);
		if (stars[s].z <= 0)
			stars[s].z += 1;
		if (stars[s].z > 1)
			stars[s].z -= 1;
	}
}

void movement(int s, float dt) {
	//stars[s].x = stars[s].x;
	//stars[s].y = stars[s].y;
	stars[s].z = stars[s].z - V * dt;
	cout << dt;
}