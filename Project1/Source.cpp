#include "SDL.h"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "SDLauxiliary.h"

using namespace std;
using glm::vec3;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Surface* screen;

void startRainbow();
void rainbowDraw(vector<vec3> left, vector<vec3> right);
void interpolate(float a, float b, vector<float>& result);
void interpolate(vec3 a, vec3 b, vector<vec3>& result);
void testInterpolate();

int main(int argc, char* argv[]) {
	testInterpolate();
	startRainbow();
	
	return 0;
}

void startRainbow() {
	vec3 topLeft(1, 0, 0);
	vec3 topRight(0, 0, 1);
	vec3 bottomLeft(0, 1, 0);
	vec3 bottomRight(1, 1, 0);
	vector<vec3> leftSide(SCREEN_HEIGHT);
	vector<vec3> rightSide(SCREEN_HEIGHT);
	interpolate(topLeft, bottomLeft, leftSide);
	interpolate(topRight, bottomRight, rightSide);

	screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
	while (NoQuitMessageSDL()) {
		rainbowDraw(leftSide, rightSide);
	}
	SDL_SaveBMP(screen, "screenshot.bmp");
}

void rainbowDraw(vector<vec3> left, vector<vec3> right) {

	SDL_LockSurface(screen);

	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		vector<vec3> row(SCREEN_WIDTH);
		interpolate(left[y], right[y], row);

		for (int x = 0; x < SCREEN_WIDTH; ++x) {
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
	for (int i = 0; i < resultFloat.size(); ++i)
		cout << resultFloat[i] << " ";
	cout << "\n\nUsing glm::vec3:" << endl;;
	vector<vec3> result(4);
	vec3 a(1, 4, 9.2);
	vec3 b(4, 1, 9.8);
	interpolate(a, b, result);
	for (int i = 0; i<result.size(); ++i) {
		cout << "( "
			<< result[i].x << ", "
			<< result[i].y << ", "
			<< result[i].z << " ) ";
	}
}