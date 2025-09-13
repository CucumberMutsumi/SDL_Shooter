#include"SceneEnd.h"
#include<string>
#include"Game.h"
#include"ScencMain.h"

void SceneEnd::Init()
{
	//��û�д���������ģʽ�����
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
	if (IsTyping)//�ж��Ƿ��������������
	{
		if (event->type == SDL_TEXTINPUT)
		{
			//����������ַ�ʱ�������������ĸ�����ֵȣ����ᴥ��SDL_TEXTINPUT�¼�
			//event->text.text����������� UTF - 8 �����ַ�
			name += event->text.text;
		}
		if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				//������Ұ��»س����������������������
				IsTyping = false;
				SDL_StopTextInput();// ֹͣ�ı����빦��
				if (name == "")
				{
					name = "noname";
				}
				game.Insertleaderboard(game.GetFinalScore(), name);
			}
			if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
			{
				//ר�Ŵ��� UTF-8 �ַ���ɾ����������Ϊ UTF-8 �ַ������ɶ���ֽ���ɣ����ܼ򵥵���name.pop_back()
				RemoveLastUTF8Char(name);
			}
		}
	}
	else
	{
		//�����ǰ���س�����IsTyping=false,�л������а����ʱ�İ�������
		if (event->type == SDL_KEYDOWN)
		{
			//��J���¿�ʼ��Ϸ
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
	game.RenderCenterText("�÷ְ�", 0.1, true);
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
	if ((lastchar & 0b10000000) == 0b10000000)//�����ַ��ĺ����ֽڸ�ʽΪ10XXXXXX
	{
		str.pop_back();
		while ((str.back() & 0b11000000) != 0b11000000)//�ж��Ƿ������ĵ����һ���ֽ�
		{
			str.pop_back();
		}
		//UTF8�µ������ַ�ռ3���ֽ�
	}
	str.pop_back();
}
