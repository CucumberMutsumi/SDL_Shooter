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
	//渲染远星
	for (int posY = static_cast<int>(farStar.offset); posY <= GetWindowHeigth(); posY += farStar.height)
	{
		for (int posX = 0; posX <= GetWindowWidth(); posX += farStar.width)
		{
			SDL_Rect desrect = { posX,posY,farStar.width,farStar.height };
			SDL_RenderCopy(renderer, farStar.texture, NULL, &desrect);
		}
	}

	//渲染近星
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
	//1.初始化帧数
	frametime = 1000 / FPS;
	//2.初始化SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Init Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	//3.创建窗口
	window = SDL_CreateWindow("SDL SHOOTER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeigth, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL window Init Error: %s\n", SDL_GetError());
		IsGameRunning = false;
	}
	//4.创建渲染器
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
	//7.背景
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
	//6.创建并初始化主窗口

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
		auto frameStart = SDL_GetTicks();  // 记录当前帧开始的时间（毫秒）
		SDL_Event event;
		HandleEvent(&event);  // 处理用户输入（如键盘、鼠标事件）

		Updata(deltatime);  // 更新游戏逻辑（角色移动、碰撞检测等）

		Renderer();  // 渲染画面（将更新后的游戏状态绘制到屏幕）

		auto frameEnd = SDL_GetTicks();  // 记录当前帧结束的时间（毫秒）
		auto diff = frameEnd - frameStart;  // 计算当前帧耗时（毫秒）

		// 控制帧率：如果当前帧耗时小于目标帧耗时，就延迟等待
		if (diff < frametime)
		{
			SDL_Delay(frametime - diff);  // 延迟剩余时间，保证每帧至少耗时frametime
			deltatime = frametime / 1000.0f;  // 计算每帧的时间间隔（秒），用于时间相关的更新
		}
		else
		{
			deltatime = diff / 1000.0f;  // 如果帧耗时超过目标，直接用实际耗时计算deltatime
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
	//放入一个空的event，SDL_PollEvent函数会从事件队列中取出一个事件
	// 并存储到event指针指向的结构体中
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
	//用表面来创建纹理
	//颜色
	SDL_Color color = { 255,255,255,255 };
	//创建表面
	SDL_Surface* surface;
	if (IsCenter)
	{
		surface = TTF_RenderUTF8_Solid(titleFont, title.c_str(), color);
	}
	else
	{
		surface = TTF_RenderUTF8_Solid(textFont, title.c_str(), color);
	}
	//创建纹理
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GetRenderer(), surface);
	//指定渲染位置
	//按比例指定渲染的位置
	int y = (static_cast<int>(GetWindowHeigth() - surface->h) * posY);
	SDL_Rect rect = { GetWindowWidth() / 2 - surface->w / 2,
					y,surface->w,surface->h };
	SDL_RenderCopy(GetRenderer(), texture, NULL, &rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
