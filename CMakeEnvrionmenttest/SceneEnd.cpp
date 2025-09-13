#include"SceneEnd.h"
#include<string>
#include"Game.h"
#include"ScencMain.h"

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
	if (IsTyping)//判断是否处于输入玩家名字
	{
		if (event->type == SDL_TEXTINPUT)
		{
			//当玩家输入字符时（如键盘输入字母、数字等），会触发SDL_TEXTINPUT事件
			//event->text.text包含了输入的 UTF - 8 编码字符
			name += event->text.text;
		}
		if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				//处理玩家按下回车键的情况，结束输入流程
				IsTyping = false;
				SDL_StopTextInput();// 停止文本输入功能
				if (name == "")
				{
					name = "noname";
				}
				game.Insertleaderboard(game.GetFinalScore(), name);
			}
			if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
			{
				//专门处理 UTF-8 字符的删除函数，因为 UTF-8 字符可能由多个字节组成，不能简单地用name.pop_back()
				RemoveLastUTF8Char(name);
			}
		}
	}
	else
	{
		//这里是按完回车键后，IsTyping=false,切换到排行榜界面时的按键控制
		if (event->type == SDL_KEYDOWN)
		{
			//按J重新开始游戏
			if (event->key.keysym.scancode == SDL_SCANCODE_J)
			{
				auto scenemain = new ScencMain();
				game.ChangeScenc(scenemain);
			}
		}
	}
}

void SceneEnd::Updata(float deltatime)
{
	timer += deltatime;
	if (timer > 1.0f)
	{
		timer -= 1.0f;
	}
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
		SDL_Point p=game.RenderCenterText(name, 0.8, false);
		if (timer < 0.5f)
		{
			game.RenderTextPos("_", p.x, p.y);
		}
		
	}
	else
	{
		if(timer<0.5f)
		game.RenderCenterText("_", 0.8, false);
	}
}

void SceneEnd::RenderPhase2()
{
	game.RenderCenterText("得分榜", 0.1, true);
	auto i = 1;
	auto posY = 0.2 * game.GetWindowHeigth();
	for (auto& user : game.GetBoard())
	{
		std::string name = to_string(i) + "." + user.second;
		auto score = to_string(user.first);
		game.RenderTextPos(name, 100, posY);
		game.RenderTextPos(score, 100, posY, false);
		posY += 50;
		i++;
	}
	if (timer < 0.5f)
	{
		game.RenderCenterText("Pause J to start game", 0.85, false);
	}
}

void SceneEnd::RemoveLastUTF8Char(std::string& str)
{
	
	if (str.empty())return;
	auto lastchar = str.back();
	if ((lastchar & 0b10000000) == 0b10000000)//中文字符的后续字节格式为10XXXXXX
	{
		str.pop_back();
		while ((str.back() & 0b11000000) != 0b11000000)//判断是否是中文的最后一个字节
		{
			str.pop_back();
		}
		//UTF8下的中文字符占3个字节
	}
	str.pop_back();
}
