#include "pickup.h"

Pickup::Pickup(SDL_Renderer* renderer) {

	active = false;

	myRenderer = renderer;

	speed = rand() % (5) + 1;
	speed *= 100;

	int temp = rand() % (2) + 1;

	SDL_Surface* surface;

	if (temp == 1)
	{
		pickupType = "bomb";
		surface = IMG_Load("./Assets/bomb.png");
	}
	else
	{
		pickupType = "lives";
		surface = IMG_Load("./Assets/lives.png");
	}

	texture = SDL_CreateTextureFromSurface(myRenderer, surface);

	SDL_FreeSurface(surface);

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	posRect.x = rand() % (1024 - posRect.w) + 1;
	posRect.y = 0 - posRect.h;

	pos_x = posRect.x;
	pos_y = posRect.y;

	xDir = 0;
	yDir = 1;

}

void Pickup::DeActivate()
{
	active = false;

	posRect.x = -2000;
	posRect.y = -2000;

	pos_x = posRect.x;
	pos_y = posRect.y;

}


void Pickup::Reset()
{
	if (!active) {

		active = true;

		posRect.x = rand() % (1024 - posRect.w) + 1;
		posRect.y = 0 - posRect.h;

		pos_x = posRect.x;
		pos_y = posRect.y;

		speed = rand() % (5) + 1;
		speed *= 100;

		int temp = rand() % (2) + 1;

		SDL_Surface* surface;

		if (temp == 1)
		{
			pickupType = "bomb";
			surface = IMG_Load("./Assets/bomb.png");
		}
		else
		{
			pickupType = "lives";
			surface = IMG_Load("./Assets/lives.png");
		}

		texture = SDL_CreateTextureFromSurface(myRenderer, surface);

		SDL_FreeSurface(surface);

	}

}

void Pickup::Update(float deltatTime)
{
	if (active) {
		pos_y += (speed * yDir) * deltatTime;

		posRect.y = (int)(pos_y + 0.5f);

		if (posRect.y > 768)
		{
			DeActivate();
		}
	}

}

void Pickup::Draw(SDL_Renderer* renderer) {

	SDL_RenderCopy(renderer, texture, NULL, &posRect);

}

Pickup::~Pickup()
{
	//SDL_DestroyTexture(texture);
}