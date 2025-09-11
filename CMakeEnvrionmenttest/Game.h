#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<string>
#include"Object.h"

using namespace std;
class Scenc;

class Game
{
public:
	static Game& GetInstance();
	~Game();
	void Init();
	void Run();
	void Clean();
	void ChangeScenc(Scenc* scenc);
	void HandleEvent(SDL_Event* event);

	void Updata(float deltatime);
	void Renderer();

	void RenderCenterText(const string title, float posY, bool IsCenter);

	SDL_Window* GetWindow() { return window; }
	SDL_Renderer* GetRenderer() { return renderer; }
	int GetWindowWidth() { return WindowWidth;}
	int GetWindowHeigth() { return WindowHeigth; }
	void BackGroundUpdate(float deltatime);
	void RenderBackGround();
private:
	Game();
	//É¾³ý¿½±´º¯Êý
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	
	TTF_Font* titleFont;
	TTF_Font* textFont;

	bool IsGameRunning = true;
	Scenc* CurrentScenc = nullptr;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	int WindowWidth = 600;
	int WindowHeigth = 800;

	int FPS = 60;
	Uint32 frametime;
	float deltatime;

	BackGround nearStar;
	BackGround farStar;
};
/*
	int FPS=60;
	Uint32 frametime;
	float deltatime;
	frametime=1/FPS;
	auto framestart=SDL_GetTicks();
	...
	auto frameend=SDL_GetTicks();
	auto diff=frameend-framestart;
	if(diff<frametime)
	{
		SDL_Delay(frametime-diff);
		daltatime=frametime;
	}
	else
	{
		deltatime=diff;
	}
*/
