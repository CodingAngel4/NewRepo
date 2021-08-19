#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

//boolean to keep loop
bool quit = false;

//delta time for frame rate indepencence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

//set player speed
float playerSpeed = 500.0f;

//movement in x axis - left to roght
float xDir;


//variables to control integer creep
float pos_x, pos_y;

//enemy stuff
#include "enemy.h";

#include <vector>

//enemy list 
vector<Enemy> enemyList;

//bullet stuff
#include "bullet.h"
vector<Bullet> bulletList;

//create rect for position
SDL_Rect playerPos;

//declare sounds
//small sounds
Mix_Chunk* laser;
Mix_Chunk* explode;
//longer sound
Mix_Music* bgm;

void CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			//play laser sound
			Mix_PlayChannel(-1, laser, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = (pos_x + (playerPos.w / 2));

			bulletList[i].posRect.x = (bulletList[i].posRect.x -
				(bulletList[i].posRect.w / 2));

			bulletList[i].posRect.y = playerPos.y;

			bulletList[i].pos_x = pos_x;

			bulletList[i].pos_y = playerPos.y;

			break;
		}
	}


}


//variables for score and lives
int playerScore, oldScore, playerLives, oldLives;

//font
TTF_Font* font;

//font color
SDL_Color colorP1 = { 0, 255, 0 , 255 };

//surface
SDL_Surface* scoreSurface, * liveSurface;

//textures
SDL_Texture* scoreTexture, * livesTexture;

SDL_Rect scorePos, livesPos;

string tempScore, tempLives;

void UpdateScore(SDL_Renderer* renderer) {

	//creat text
	tempScore = "Player Score: " + std::to_string(playerScore);

	//create render text
	scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP1);

	//texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);

	SDL_FreeSurface(scoreSurface);

	oldScore = playerScore;



}


void UpdateLives(SDL_Renderer* renderer) {

	//creat text
	tempLives = "Player Lives: " + std::to_string(playerLives);

	//create render text
	liveSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP1);

	//texture
	livesTexture = SDL_CreateTextureFromSurface(renderer, liveSurface);

	SDL_QueryTexture(livesTexture, NULL, NULL, &livesPos.w, &livesPos.h);

	SDL_FreeSurface(liveSurface);

	oldLives = playerLives;
}

//setup pickup
#include "pickup.h"
vector<Pickup> pickupList;


int main(int argc, char* argv[]) {

	SDL_Window* window; //pointer

	//render variable
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);

	//create app window with settings
	window = SDL_CreateWindow(
		"Space Game",					//window title
		SDL_WINDOWPOS_UNDEFINED,		//x position
		SDL_WINDOWPOS_UNDEFINED,		//y position
		1024,							//width, in pixels
		768,							//height, in pixels
		SDL_WINDOW_OPENGL				// flags
	);

	//check window was created successfully
	if (window == NULL) {
		//case if not made
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//background image -- CREATE

	//create sdl Surface
	SDL_Surface* surface = IMG_Load("./Assets/background.png");

	//create bkgd texture
	SDL_Texture* bkgd;

	//place surface into texture
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface
	SDL_FreeSurface(surface);

	//create rect for menu graphics
	SDL_Rect bkgdPos;

	//set pos
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 1024;
	bkgdPos.h = 768;

	//background -- CREATE END

	//player image -- CREATE

	//create surface
	surface = IMG_Load("./Assets/player.png");

	//create bkgd texture
	SDL_Texture* player;

	//place surface into texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface
	SDL_FreeSurface(surface);


	//set pos
	playerPos.x = 500;
	playerPos.y = 700;
	playerPos.w = 165;
	playerPos.h = 64;
	

	//player -- CREATE END

	//sdl event to handle input
	SDL_Event event;

	//fill bullets list
	for (int i = 0; i < 10; i++) {
		Bullet tempBullet(renderer, i + 5, i + 5);

		bulletList.push_back(tempBullet);
	}

	//clear enemy list
	enemyList.clear();

	//fill enemy list
	for (int i = 0; i < 6; i++) {
		Enemy tempEnemy(renderer);

		enemyList.push_back(tempEnemy);
	}

	//audio playback
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//load laser, explosion and bkg sound
	laser = Mix_LoadWAV("./Assets/laser.wav");

	explode = Mix_LoadWAV("./Assets/explode.wav");

	bgm = Mix_LoadMUS("./Assets/spaceloop.mp3");

	//if music not playing set to loop
	if (!Mix_PlayingMusic()) {
		Mix_PlayMusic(bgm, -1);
	}

	//init score, lives, font
	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 3;

	TTF_Init();
	font = TTF_OpenFont("./Assets/spacenoid.ttf", 20);

	// set up rects
	scorePos.x = scorePos.y = 10;
	livesPos.x = 10;
	livesPos.y = 40;

	UpdateScore(renderer);
	UpdateLives(renderer);

	//set up enum for game states
	enum GameState { MENU, INSTRUCTIONS, LEVEL2, GAME, WIN, LOSE};

	//variable to track where we are
	GameState gameState = MENU;

	//bool for movement
	bool menu, instructions, game, win, lose, level2;

	//clear and create pickup
	pickupList.clear();

	Pickup tempPickup(renderer);

	pickupList.push_back(tempPickup);


	//basic program loop
	while (!quit)
	{
		switch (gameState)
		{
		case MENU:

			menu = true;

			std::cout << "The Game State is MENU" << endl;
			std::cout << "Press the I key for Instructions" << endl;
			std::cout << "Press the S key to Start The Game" << endl;
			std::cout << "Press the Q key to Quit Game" << endl;
			std::cout << endl;

			//load menu surface
			surface = IMG_Load("./Assets/mainmenu.png");

			//place surface into texture
			bkgd = SDL_CreateTextureFromSurface(renderer, surface);

			//free surface
			SDL_FreeSurface(surface);

			//background CREATE end

			while (menu) {

				//create delta time
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input
				if (SDL_PollEvent(&event))
				{
					//close window by X button
					if (event.type == SDL_QUIT) {
						quit = true;
						menu = false;
						break;
					}

					switch (event.type) {

						//look for keypress
					case SDL_KEYUP:

						//check SDLkey
						switch (event.key.keysym.sym)
						{
						case SDLK_i:
							menu = false;
							gameState = INSTRUCTIONS;
							break;
						case SDLK_s:
							menu = false;
							gameState = GAME;
							break;
						case SDLK_q:
							menu = false;
							quit = true;
							break;
						default:
							break;
						}
					}
				} //input end

				//DRAW ***************************

				//clear old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkgd1
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				//draw new info to screen
				SDL_RenderPresent(renderer);

				//DRAW END ***********************


			}//ends menu screen
			break;

		case INSTRUCTIONS:

			instructions = true;

			std::cout << "The Game State is INSTRUCTIONS" << endl;
			std::cout << "Press the S key to Start The Game" << endl;
			std::cout << "Press the M key for Main Menu" << endl;
			std::cout << "Press the Q key to Quit Game" << endl;
			std::cout << endl;

			//load instruction surface
			surface = IMG_Load("./Assets/instructions2.png");

			//place surface into texture
			bkgd = SDL_CreateTextureFromSurface(renderer, surface);

			//free surface
			SDL_FreeSurface(surface);

			//instruction CREATE end

			while (instructions) {

				//create delta time
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input
				if (SDL_PollEvent(&event))
				{
					//close window by X button
					if (event.type == SDL_QUIT) {
						quit = true;
						instructions = false;
						break;
					}

					switch (event.type) {

						//look for keypress
					case SDL_KEYUP:

						//check SDLkey
						switch (event.key.keysym.sym)
						{
						case SDLK_s:
							instructions = false;
							gameState = GAME;
							break;
						case SDLK_m:
							instructions = false;
							gameState = MENU;
							break;
						case SDLK_q:
							instructions = false;
							quit = true;
							break;
						default:
							break;
						}
					}
				} //input end

				//DRAW ***************************

				//clear old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkgd1
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				//draw new info to screen
				SDL_RenderPresent(renderer);

				//DRAW END ***********************


			}//ends instructions screen
			break;

		case GAME:
		{
			game = true;

			std::cout << "The Game State is GAME" << endl;
			std::cout << endl;

			//background image -- CREATE

			//create sdl Surface
			surface = IMG_Load("./Assets/background.png");

			//place surface into texture
			bkgd = SDL_CreateTextureFromSurface(renderer, surface);

			//free the surface
			SDL_FreeSurface(surface);

			//start fresh
			enemyList.clear();

			//fill enemy list
			for (int i = 0; i < 6; i++) {
				Enemy tempEnemy(renderer);

				enemyList.push_back(tempEnemy);
			}

			//clear and create pickup
			pickupList.clear();

			Pickup tempPickup(renderer);

			pickupList.push_back(tempPickup);

			//reset score and lives
			playerScore = 0;
			playerLives = 3;


			//background -- CREATE END

			while (game) {

				//create delta time
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input
				if (SDL_PollEvent(&event))
				{
					//close window by X button
					if (event.type == SDL_QUIT) {
						quit = true;
						game = false;
						break;
					}

					switch (event.type) {

						//look for keypress
					case SDL_KEYUP:

						//check spacebar
						switch (event.key.keysym.sym)
						{
						case SDLK_SPACE:
							CreateBullet();
							pickupList[0].Reset();
						default:
							break;
						}
					}
				} //input end

				//player movement
				//get keyboard state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

				//check arrows pressed
				if (currentKeyStates[SDL_SCANCODE_LEFT]) {
					xDir = -1;
				}
				else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
					xDir = 1;
				}
				else {
					xDir = 0;
				}

				//UPDATE ***************************************

				//get new position using direction and deltatime
				pos_x += (playerSpeed * xDir) * deltaTime;

				//move player
				playerPos.x = (int)(pos_x + 0.5f);

				//keep player on screen
				//moving left
				if (playerPos.x < 0) {
					playerPos.x = 0;
					pos_x = 0;
				}
				//moving right
				if (playerPos.x > 1024 - playerPos.w)
				{
					playerPos.x = 1024 - playerPos.w;
					pos_x = 1024 - playerPos.w;
				}

				//update bullet list
				for (int i = 0; i < bulletList.size(); i++) {
					if (bulletList[i].active == true) {
						bulletList[i].Update(deltaTime);
					}
				}

				//update pickup
				pickupList[0].Update(deltaTime);

				//update enemy list
				for (int i = 0; i < 6; i++) {
					enemyList[i].Update(deltaTime);
				}

				//bullet with enemy
				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						for (int j = 0; j < enemyList.size(); j++)
						{
							if (SDL_HasIntersection(&bulletList[i].posRect, &enemyList[j].posRect))
							{
								//play explosion
								Mix_PlayChannel(-1, explode, 0);

								enemyList[j].Reset();

								bulletList[i].Reset();

								//give player points
								playerScore += 50;

								//check for winning score
								//level2
								if (playerScore >= 500) {
									game = false;
									gameState = LEVEL2;
								}
							}
						}
					}
				}

				//check for collision
				//enemy against player
				for (int i = 0; i < enemyList.size(); i++)
				{
					if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
					{
						//play explosion
						Mix_PlayChannel(-1, explode, 0);

						enemyList[i].Reset();

						playerLives -= 1;

						//check for lose
						if (playerLives <= 0) {
							game = false;
							gameState = LOSE;
						}
					}
				}

				//check for collision
				//pickup against player
				for (int i = 0; i < pickupList.size(); i++)
				{
					if (SDL_HasIntersection(&playerPos, &pickupList[i].posRect))
					{
						if (pickupList[i].pickupType == "lives")
						{
							//deactivate enemy
							pickupList[i].DeActivate();

							//play explosion
							Mix_PlayChannel(-1, explode, 0);

							playerLives -= 1;

							//check if lost
							if (playerLives <= 0)
							{
								game = false;
								gameState = LOSE;
							}

						}
						else if (pickupList[i].pickupType == "bomb")
						{
							//deactivate
							pickupList[i].DeActivate();

							//playlaser
							Mix_PlayChannel(-1, laser, 0);

							playerScore += 1;

							//cycle through enemies
							for (int i = 0; i < enemyList.size(); i++)
							{
								enemyList[i].Reset();

								playerScore += 10;
							}

							//check for lvl2
							if (playerLives >= 500)
							{
								game = false;
								gameState = LEVEL2;
							}

						}
					}
				}


				//update score and lives
				if (playerScore != oldScore)
				{
					UpdateScore(renderer);
				}

				if (playerLives != oldLives)
				{
					UpdateLives(renderer);
				}
				//UPDATE ***************************************

				///DRAW ****************************************

				//clear old buffer
				SDL_RenderClear(renderer);

				//prepare to draw bkgd1
				SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

				//prepare to draw player
				SDL_RenderCopy(renderer, player, NULL, &playerPos);

				//draw enemy list
				for (int i = 0; i < 6; i++) {
					enemyList[i].Draw(renderer);
				}

				//draw pickup
				pickupList[0].Draw(renderer);

				//draw bullet list
				for (int i = 0; i < bulletList.size(); i++) {
					if (bulletList[i].active == true) {
						bulletList[i].Draw(renderer);
					}
				}

				//draw score texture
				SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

				//draw live texture
				SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);


				//draw new info to screen
				SDL_RenderPresent(renderer);

				// END DRAW ************************

			}//ends game screen

			break;
		}
			case LEVEL2:

				level2 = true;

				std::cout << "The Game State is LEVEL2" << endl;
				std::cout << endl;

				//start fresh
				enemyList.clear();

				//fill enemy list
				for (int i = 0; i < 6; i++) {
					Enemy tempEnemy(renderer);

					enemyList.push_back(tempEnemy);
				}


				//background -- CREATE END

				while (level2) {

					//create delta time
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime) / 1000;
					lastTime = thisTime;

					//check for input
					if (SDL_PollEvent(&event))
					{
						//close window by X button
						if (event.type == SDL_QUIT) {
							quit = true;
							level2 = false;
							break;
						}

						switch (event.type) {

							//look for keypress
						case SDL_KEYUP:

							//check spacebar
							switch (event.key.keysym.sym)
							{
							case SDLK_SPACE:
								CreateBullet();
							default:
								break;
							}
						}
					} //input end

					//player movement
					//get keyboard state
					const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

					//check arrows pressed
					if (currentKeyStates[SDL_SCANCODE_LEFT]) {
						xDir = -1;
					}
					else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
						xDir = 1;
					}
					else {
						xDir = 0;
					}

					//UPDATE ***************************************

					//get new position using direction and deltatime
					pos_x += (playerSpeed * xDir) * deltaTime;

					//move player
					playerPos.x = (int)(pos_x + 0.5f);

					//keep player on screen
					//moving left
					if (playerPos.x < 0) {
						playerPos.x = 0;
						pos_x = 0;
					}
					//moving right
					if (playerPos.x > 1024 - playerPos.w)
					{
						playerPos.x = 1024 - playerPos.w;
						pos_x = 1024 - playerPos.w;
					}

					//update bullet list
					for (int i = 0; i < bulletList.size(); i++) {
						if (bulletList[i].active == true) {
							bulletList[i].Update(deltaTime);
						}
					}

					//update enemy list
					for (int i = 0; i < 6; i++) {
						enemyList[i].Update(deltaTime);
					}

					//bullet with enemy
					for (int i = 0; i < bulletList.size(); i++)
					{
						if (bulletList[i].active == true)
						{
							for (int j = 0; j < enemyList.size(); j++)
							{
								if (SDL_HasIntersection(&bulletList[i].posRect, &enemyList[j].posRect))
								{
									//play explosion
									Mix_PlayChannel(-1, explode, 0);

									enemyList[j].Reset();

									bulletList[i].Reset();

									//give player points
									playerScore += 50;

									//check for winning score
									if (playerScore >= 1000) {
										level2 = false;
										gameState = WIN;
									}
								}
							}
						}
					}

					//check for collision
					//enemy against player
					for (int i = 0; i < enemyList.size(); i++)
					{
						if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
						{
							//play explosion
							Mix_PlayChannel(-1, explode, 0);

							enemyList[i].Reset();

							playerLives -= 1;

							//check for lose
							if (playerLives <= 0) {
								level2 = false;
								gameState = LOSE;
							}
						}
					}

					//update score and lives
					if (playerScore != oldScore)
					{
						UpdateScore(renderer);
					}

					if (playerLives != oldLives)
					{
						UpdateLives(renderer);
					}
					//UPDATE ***************************************

					///DRAW ****************************************

					//clear old buffer
					SDL_RenderClear(renderer);

					//prepare to draw bkgd1
					SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

					//prepare to draw player
					SDL_RenderCopy(renderer, player, NULL, &playerPos);

					//draw enemy list
					for (int i = 0; i < 6; i++) {
						enemyList[i].Draw(renderer);
					}

					//draw bullet list
					for (int i = 0; i < bulletList.size(); i++) {
						if (bulletList[i].active == true) {
							bulletList[i].Draw(renderer);
						}
					}

					//draw score texture
					SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

					//draw live texture
					SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);


					//draw new info to screen
					SDL_RenderPresent(renderer);

					// END DRAW ************************

				}//ends game screen
				break;

			case WIN:

				win = true;

				std::cout << "The Game State is WIN" << endl;
				std::cout << "Press the S key to Restart The Game" << endl;
				std::cout << "Press the M key for Main Menu" << endl;
				std::cout << "Press the Q key to Quit Game" << endl;
				std::cout << endl;

				//load win surface
				surface = IMG_Load("./Assets/winscreen.png");

				//place surface into texture
				bkgd = SDL_CreateTextureFromSurface(renderer, surface);

				//free surface
				SDL_FreeSurface(surface);

				//win CREATE end

				while (win) {

					//create delta time
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime) / 1000;
					lastTime = thisTime;

					//check for input
					if (SDL_PollEvent(&event))
					{
						//close window by X button
						if (event.type == SDL_QUIT) {
							quit = true;
							win = false;
							break;
						}

						switch (event.type) {

							//look for keypress
						case SDL_KEYUP:

							//check SDLkey
							switch (event.key.keysym.sym)
							{
							case SDLK_s:
								win = false;
								gameState = GAME;
								break;
							case SDLK_m:
								win = false;
								gameState = MENU;
								break;
							case SDLK_q:
								win = false;
								quit = true;
								break;
							default:
								break;
							}
						}
					} //input end

					//DRAW ***************************

					//clear old buffer
					SDL_RenderClear(renderer);

					//prepare to draw bkgd1
					SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

					//draw new info to screen
					SDL_RenderPresent(renderer);

					//DRAW END ***********************


				}//ends win screen
				break;

			case LOSE:

				lose = true;

				std::cout << "The Game State is LOSE" << endl;
				std::cout << "Press the S key to Restart The Game" << endl;
				std::cout << "Press the M key for Main Menu" << endl;
				std::cout << "Press the Q key to Quit Game" << endl;
				std::cout << endl;

				//load lose surface
				surface = IMG_Load("./Assets/losescreen.png");

				//place surface into texture
				bkgd = SDL_CreateTextureFromSurface(renderer, surface);

				//free surface
				SDL_FreeSurface(surface);

				//lose CREATE end

				while (lose) {

					//create delta time
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime) / 1000;
					lastTime = thisTime;

					//check for input
					if (SDL_PollEvent(&event))
					{
						//close window by X button
						if (event.type == SDL_QUIT) {
							quit = true;
							lose = false;
							break;
						}

						switch (event.type) {

							//look for keypress
						case SDL_KEYUP:

							//check SDLkey
							switch (event.key.keysym.sym)
							{
							case SDLK_s:
								lose = false;
								gameState = GAME;
								break;
							case SDLK_m:
								lose = false;
								gameState = MENU;
								break;
							case SDLK_q:
								lose = false;
								quit = true;
								break;
							default:
								break;
							}
						}
					} //input end

					//DRAW ***************************

					//clear old buffer
					SDL_RenderClear(renderer);

					//prepare to draw bkgd1
					SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

					//draw new info to screen
					SDL_RenderPresent(renderer);

					//DRAW END ***********************


				}//ends lose screen
				break;
		}//ends switch
	} //end game loop

	//close and destroy window
	SDL_DestroyWindow(window);

	//clean up
	SDL_Quit();

	return 0;


}