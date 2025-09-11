#pragma once
#include"Scenc.h"
#include<SDL.h>
#include<SDL_mixer.h>
#include<iostream>
class Game;

class ScencTitle : public Scenc
{
public:
	void Init() override;
	void HandleEvent(SDL_Event* event) override;
	void Updata(float deltatime) override;
	void Render() override;
	void Clean() override;
private:
	Mix_Music* bgm=nullptr;
	float timer = 0.0f;
};