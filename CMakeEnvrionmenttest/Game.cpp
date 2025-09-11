#include"Game.h"
#include"ScencTitle.h"
void Game::BackGroundUpdate(float deltatime)
{
	nearStar.offset += deltatime * nearStar.Speed;
	if (nearStar.offset >= 0)
	{
		nearStar.offset -= nearStar.height;
	}

	farStar.offset += deltatime * farStar.Speed;
	if (farStar.offset >= 0)
	{
		farStar.offset -= farStar.height;
	}
}

void Game::RenderBackGround()
{
	//��ȾԶ��
	for (int posY = static_cast<int>(farStar.offset); posY <= GetWindowHeigth(); posY += farStar.height)
	{
		for (int posX = 0; posX <= GetWindowWidth(); posX += farStar.width)
		{
			SDL_Rect desrect = { posX,posY,farStar.width,farStar.height };
			SDL_RenderCopy(renderer, farStar.texture, NULL, &desrect);
		}
	}

	//��Ⱦ����
	for (int posY = static_cast<int>(nearStar.offset); posY <= GetWindowHeigth(); posY += nearStar.height)
	{
		for (int posX = 0; posX <= GetWindowWidth(); posX += nearStar.width)
		{
			SDL_Rect nearstarrect = { posX,posY,nearStar.width,nearStar.height };
			SDL_RenderCopy(renderer, nearStar.texture, NULL, &nearstarrect);
		}
	}

}

Game::Game()
{
}

Game& Game::GetInstance()
{
	static Game Instance;
	return  Instance;
}

Game::~Game()
{
	Clean();
}

void Game::Init()
{
	//1.��ʼ��֡��
	frametime = 1000 / FPS;
	//2.��ʼ��SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Init Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	//3.��������
	window = SDL_CreateWindow("SDL SHOOTER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeigth, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL window Init Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	//4.������Ⱦ��
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL renderer Init Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	//5.SDL_image
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL image Inti Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	//6.SDL_Mix
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL mixer Ini Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could no open mixaudio Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	Mix_AllocateChannels(32);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 6);
	Mix_Volume(-1, MIX_MAX_VOLUME / 8);

	if (TTF_Init() == -1)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Init ttf fail Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	//7.����
	nearStar.texture = IMG_LoadTexture(GetRenderer(), "image/Stars-A.png");
	if (nearStar.texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Init nearStar texture fail Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	SDL_QueryTexture(nearStar.texture, NULL, NULL, &nearStar.width, &nearStar.height);
	nearStar.width /= 2;
	nearStar.height /= 2;
	farStar.texture = IMG_LoadTexture(GetRenderer(), "image/Stars-B.png");
	if (farStar.texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Init farStar texture fail Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	SDL_QueryTexture(farStar.texture, NULL, NULL, &farStar.width, &farStar.height);
	farStar.Speed = 20;
	farStar.width /= 2;
	farStar.height /= 2;
	//6.��������ʼ��������

	titleFont = TTF_OpenFont("font/VonwaonBitmap-16px.ttf", 64);
	textFont = TTF_OpenFont("font/VonwaonBitmap-16px.ttf", 32);
	if (titleFont == nullptr || textFont == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "init Font Error: %s\n", SDL_GetError());
	}
	CurrentScenc = new ScencTitle();
	CurrentScenc->Init();
}

void Game::Run()
{
	while (IsGameRunning)
	{
		auto frameStart = SDL_GetTicks();  // ��¼��ǰ֡��ʼ��ʱ�䣨���룩
		SDL_Event event;
		HandleEvent(&event);  // �����û����루����̡�����¼���

		Updata(deltatime);  // ������Ϸ�߼�����ɫ�ƶ�����ײ���ȣ�

		Renderer();  // ��Ⱦ���棨�����º����Ϸ״̬���Ƶ���Ļ��

		auto frameEnd = SDL_GetTicks();  // ��¼��ǰ֡������ʱ�䣨���룩
		auto diff = frameEnd - frameStart;  // ���㵱ǰ֡��ʱ�����룩

		// ����֡�ʣ������ǰ֡��ʱС��Ŀ��֡��ʱ�����ӳٵȴ�
		if (diff < frametime)
		{
			SDL_Delay(frametime - diff);  // �ӳ�ʣ��ʱ�䣬��֤ÿ֡���ٺ�ʱframetime
			deltatime = frametime / 1000.0f;  // ����ÿ֡��ʱ�������룩������ʱ����صĸ���
		}
		else
		{
			deltatime = diff / 1000.0f;  // ���֡��ʱ����Ŀ�ֱ꣬����ʵ�ʺ�ʱ����deltatime
		}
	}
}

void Game::Clean()
{
	if (CurrentScenc != nullptr)
	{
		CurrentScenc->Clean();
		delete CurrentScenc;
	}
	if (nearStar.texture != nullptr)
	{
		SDL_DestroyTexture(nearStar.texture);
	}
	if (farStar.texture != nullptr)
	{
		SDL_DestroyTexture(farStar.texture);
	}
	if (titleFont != nullptr)
	{
		TTF_CloseFont(titleFont);
	}
	if (textFont != nullptr)
	{
		TTF_CloseFont(textFont);
	}
	IMG_Quit();

	Mix_CloseAudio();
	Mix_Quit();

	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::ChangeScenc(Scenc* scenc)
{
	if (CurrentScenc != nullptr)
	{
		CurrentScenc->Clean();
		delete CurrentScenc;
	}
	CurrentScenc = scenc;
	CurrentScenc->Init();
}

void Game::HandleEvent(SDL_Event* event)
{
	//����һ���յ�event��SDL_PollEvent��������¼�������ȡ��һ���¼�
	// ���洢��eventָ��ָ��Ľṹ����
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			IsGameRunning = false;
		}
		CurrentScenc->HandleEvent(event);
	}
}

void Game::Updata(float deltatime)
{
	BackGroundUpdate(deltatime);
	CurrentScenc->Updata(deltatime);
}

void Game::Renderer()
{
	
	//clear renderer
	SDL_RenderClear(renderer);

	RenderBackGround();

	CurrentScenc->Render();
	//Show Updata
	SDL_RenderPresent(renderer);
}

void Game::RenderCenterText(const string title, float posY, bool IsCenter)
{
	//�ñ�������������
	//��ɫ
	SDL_Color color = { 255,255,255,255 };
	//��������
	SDL_Surface* surface;
	if (IsCenter)
	{
		surface = TTF_RenderUTF8_Solid(titleFont, title.c_str(), color);
	}
	else
	{
		surface = TTF_RenderUTF8_Solid(textFont, title.c_str(), color);
	}
	//��������
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GetRenderer(), surface);
	//ָ����Ⱦλ��
	//������ָ����Ⱦ��λ��
	int y = (static_cast<int>(GetWindowHeigth() - surface->h) * posY);
	SDL_Rect rect = { GetWindowWidth() / 2 - surface->w / 2,
					y,surface->w,surface->h };
	SDL_RenderCopy(GetRenderer(), texture, NULL, &rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
