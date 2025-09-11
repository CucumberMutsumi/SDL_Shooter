#include"ScencTitle.h"
#include"Game.h"
#include"ScencMain.h"
#include<string>
void ScencTitle::Init()
{
	bgm = Mix_LoadMUS("music/06_Battle_in_Space_Intro.ogg");
	if (bgm == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Init bgm fail Error: %s\n ", SDL_GetError());
	}
	Mix_PlayMusic(bgm, -1);
}

void ScencTitle::HandleEvent(SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.scancode == SDL_SCANCODE_J)
		{
			auto Scencmain = new ScencMain();
			game.ChangeScenc(Scencmain);
		}
	}
}

void ScencTitle::Updata(float deltatime)
{
	timer += deltatime;
	if (timer > 1.0f)
	{
		timer -= 1.0f;
	}
}

void ScencTitle::Render()
{
	string title = ("Space War");
	game.RenderCenterText(title, 0.4, true);

	if (timer > 0.5f)
	{
		string instrution = "Puse J to start game";
		game.RenderCenterText(instrution, 0.8, false);
	}
	
}

void ScencTitle::Clean()
{

}
