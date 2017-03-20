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

void draw();
void interpolate(float a, float b, vector<float>& result);
void interpolate(vec3 a, vec3 b, vector<vec3>& result);

int main(int argc, char* argv[]) {
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

	screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
	while (NoQuitMessageSDL()) {
		draw();
	}
	SDL_SaveBMP(screen, "screenshot.bmp");
	return 0;
}

void draw() {

	SDL_LockSurface(screen);

	for (int y = 0; y < SCREEN_HEIGHT; ++y) {

		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			vec3 color(1, 0.6, 0.6);
			PutPixelSDL(screen, x, y, color);
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