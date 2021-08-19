#include "bullet.h"

Bullet::Bullet(SDL_Renderer* renderer, float x, float y) {

	active = false;

	speed = 800.0;

	SDL_Surface* surface = IMG_Load("./Assets/projectile.png");

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture == NULL)
	{
		printf("Unable to load Image %s! SDL_image Error: %s\n",
			"./Assets/projectile.png", IMG_GetError());
	}

	SDL_FreeSurface(surface);

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	posRect.x = x - (posRect.w / 2);
	posRect.y = y;

	pos_x = x;
	pos_y = y;

	xDir = 0;
	yDir = -1;

}

void Bullet::Reset()
{
	active = false;

	posRect.x = -1000;
	posRect.y = -1000;

	pos_x = posRect.x;
	pos_y = posRect.y;

}

void Bullet::Update(float deltatTime)
{
	if (active) {
		pos_y += (speed * yDir) * deltatTime;

		posRect.y = (int)(pos_y + 0.5f);

		if (posRect.y < (0 - posRect.h))
		{
			posRect.x = -1000;
			pos_x = posRect.x;
			active = false;
		}
	}

}

void Bullet::Draw(SDL_Renderer* renderer) {

	SDL_RenderCopy(renderer, texture, NULL, &posRect);

}

Bullet::~Bullet()
{
	//SDL_DestroyTexture(texture);
}