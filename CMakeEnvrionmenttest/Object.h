#pragma once
#include<SDL.h>

enum class ItemType
{
	Health,
	Shield,
	Time
};

struct Player
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint postion = { 0,0 };
	int width = 0;
	int heigth = 0;
	int Speed = 200;
	int currentHealth = 3;
	int MaxHealth = 3;
	Uint32 CoolDown = 300;
	Uint32 LastShootTime = 0;
};

struct PlayerBullte
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint postion = { 0,0 };
	int width = 0;
	int heigth = 0;
	int Speed = 600;
	int damage = 1;
};

struct Enemy
{
	SDL_Texture* texture=nullptr;
	SDL_FPoint postion = { 0,0 };
	int width = 0;
	int heigth = 0;
	int Speed = 200;
	int currenthealth = 2;
	Uint32 CoolDown = 2000;
	Uint32 LastShootTime = 0;
};

struct EnemyBullet
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint postion = { 0,0 };
	SDL_FPoint direction = { 0,0 };
	int width = 0;
	int heigth = 0;
	int Speed = 100;
	int damage = 1;
};

struct Explode
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	int width = 0;
	int heigth = 0;
	int CurrentFrame = 0;
	int TotalFrame = 0;
	Uint32 StratTime = 0;
	Uint32 FPS = 10;
};

struct Item
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	SDL_FPoint direction = { 0,0 };
	int width=0;
	int height=0;
	int Speed = 200;
	int bounceCount = 3;
	ItemType type = ItemType::Health;
};

struct BackGround
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	float offset = 0;
	int width = 0;
	int height = 0;
	int Speed = 30;
};