#pragma once
#include"Scenc.h"
#include"Object.h"
#include<list>
#include<random>
#include<SDL.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<map>
#include<string>
using namespace std;

class ScencMain : public Scenc
{
public:
	~ScencMain();
	void Init()override;
	void HandleEvent(SDL_Event* event)override;
	void Updata(float deltatime)override;
	void Render()override;
	void Clean()override;
	void KeyboardControl(float deltatime);

	void PlayerShootBullte();
	void UpdataPleyerBullte(float deltatime);
	void UpdataPlayer();
	void RenderPlayerBullte();
	void SpawnEnemy();
	void UpdataEnemy(float deltatime);
	void UpdataEnemyBullet(float deltatime);
	void RenderEnemy();
	void RenderEnemyBullet();
	void EnemyShoot(Enemy* enemy);
	SDL_FPoint GetDirection(Enemy* enemy);
	void EnemyExplode(Enemy* enemy);
	void UpdataExplosion(float deltatime);
	void RenderExxplosion();
	void DropItem(Enemy* enemy);
	void UpdateItem(float deltatime);
	void PlayerGetItem(Item* item);
	void RenderItem();
	void RenderUIhealth();
	void ChengeSceneDelay(float deltatime, float delay);
private:
	
	//初始化模板
	SDL_Texture* UIHealth = nullptr;
	Mix_Music* bgm;
	TTF_Font* ScoreFont;
	Player player;
	bool IsPlayerDead = false;
	PlayerBullte PlayerBullteTemple;
	Enemy EnemyTemple;
	EnemyBullet EnemyBulletTemple;
	Explode ExplodeTemple;
	Item HealthTemple;
	int Score = 0;
	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;
	float timerEnd = 0.0f;
	//子弹容器
	list<PlayerBullte*> PlayerBulltes;
	list<Enemy*> Enemies;
	list<Explode*> Explosions;
	list<EnemyBullet*> EnemyBullets;
	list<Item*> Healths;
	map<string, Mix_Chunk*> Sound;
};