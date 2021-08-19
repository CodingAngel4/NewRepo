#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
using namespace std;

class Enemy
{

public:

	bool active;
	SDL_Texture* texture;
	SDL_Rect posRect;
	float xDir, yDir;
	float speed;
	float pos_x, pos_y;

	Enemy(SDL_Renderer* renderer);

	void Update(float deltaTime);

	void Draw(SDL_Renderer* renderer);

	void Reset();

	~Enemy();

};

