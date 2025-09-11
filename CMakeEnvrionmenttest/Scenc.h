#pragma once
#include<SDL.h>

class Game;

class Scenc
{
public:
	Scenc();
	~Scenc() = default;
	virtual void Init() = 0;
	virtual void HandleEvent(SDL_Event* event) = 0;
	virtual void Updata(float deltatime) = 0;
	virtual void Render() = 0;
	virtual void Clean() = 0;
protected:
	Game& game;
};