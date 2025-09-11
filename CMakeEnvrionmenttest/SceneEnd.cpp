#include"SceneEnd.h"
#include<string>
#include"Game.h"

void SceneEnd::Init()
{
	//若没有打开文字输入模式，则打开
	if (!SDL_IsTextInputActive())
	{
		SDL_StartTextInput();
	}
	if (!SDL_IsTextInputActive())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TextInput fail Error:%s\n", SDL_GetError());
	}
}

void SceneEnd::HandleEvent(SDL_Event* event)
{
	if (IsTyping)
	{
		if (event->type == SDL_TEXTINPUT)
		{
			//将text事件里的文字加到name
			name += event->text.text;
		}
		if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				IsTyping = false;
				SDL_StopTextInput();
			}
			if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
			{
				name.pop_back();
			}
		}
	}
	else
	{

	}
}

void SceneEnd::Updata(float deltatime)
{
}

void SceneEnd::Render()
{
	if (IsTyping)
	{
		RenderPhase1();
	}
	else
	{
		RenderPhase2();
	}

}

void SceneEnd::Clean()
{
}

void SceneEnd::RenderPhase1()
{
	auto score = game.GetFinalScore();
	std::string Scoretext = "Your Score:"+std::to_string(score);
	std::string Gameover = "Game Over";
	std::string Instruction = "Please input your name:";
	game.RenderCenterText(Scoretext, 0.1, false);
	game.RenderCenterText(Gameover,0.4,true);
	game.RenderCenterText(Instruction,0.6,false);

	if (name != "")
	{
		game.RenderCenterText(name, 0.8, false);
	}
}

void SceneEnd::RenderPhase2()
{
}
