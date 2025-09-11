#include"ScencMain.h"
#include"SDL.h"
#include"SDL_image.h"
#include"Game.h"
#include"SceneEnd.h"
#include"ScencTitle.h"
#include<random>

ScencMain::~ScencMain()
{
}

void ScencMain::Init()
{
	bgm = Mix_LoadMUS("music/andata.mp3");
	if (bgm == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "could not Init bgm Error: %s\n", SDL_GetError());
	}
	Mix_PlayMusic(bgm, -1);

	UIHealth = IMG_LoadTexture(game.GetRenderer(), "image/Health UI Black.png");
	if (UIHealth == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "init UIHealth fail Error: %s\n", SDL_GetError());
	}

	ScoreFont = TTF_OpenFont("font/VonwaonBitmap-12px.ttf",24);


	Sound["player_shoot"] = Mix_LoadWAV("sound/laser_shoot4.wav");
	Sound["enemy_shoot"] = Mix_LoadWAV("sound/xs_laser.wav");
	Sound["player_explode"] = Mix_LoadWAV("sound/explosion1.wav");
	Sound["enemy_explode"]=Mix_LoadWAV("sound/explosion3.wav");
	Sound["hit"] = Mix_LoadWAV("sound/eff11.wav");
	Sound["get_item"] = Mix_LoadWAV("sound/eff5.wav");

	random_device rd;
	gen = mt19937(rd());
	dis = uniform_real_distribution<float>(0.0f, 1.0f);
	
	player.texture = IMG_LoadTexture(game.GetRenderer(), "image/SpaceShip.png");
	//��ѯ����Ŀ�Ⱥ͸߶�
	if (player.texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Init player texture Error: %s\n", SDL_GetError());
	}
	//��ȡtexture�ĸߺͿ�����player.width,player.heigth
	SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.heigth);
	player.width /= 4;
	player.heigth /= 4;
	player.postion.x = game.GetWindowWidth() / 2 - player.width / 2;
	player.postion.y = game.GetWindowHeigth() - player.heigth;

	PlayerBullteTemple.texture = IMG_LoadTexture(game.GetRenderer(), "image/laser-2.png");
	if (PlayerBullteTemple.texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL init PlayerBullteTemple Error: %s\n", SDL_GetError());
	}
	SDL_QueryTexture(PlayerBullteTemple.texture, nullptr, nullptr, &PlayerBullteTemple.width, &PlayerBullteTemple.heigth);
	PlayerBullteTemple.width /= 4;
	PlayerBullteTemple.heigth /= 4;

	EnemyTemple.texture = IMG_LoadTexture(game.GetRenderer(), "image/insect-2.png");
	if (EnemyTemple.texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL init Enemy Texture Error: %s\n", SDL_GetError());
	}
	SDL_QueryTexture(EnemyTemple.texture, nullptr, nullptr,&EnemyTemple.width, &EnemyTemple.heigth);
	EnemyTemple.width /= 4;
	EnemyTemple.heigth /= 4;

	EnemyBulletTemple.texture = IMG_LoadTexture(game.GetRenderer(), "image/bullet-2.png");
	if (EnemyBulletTemple.texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Init EnemyBullet Error: %s\n", SDL_GetError());
	}
	SDL_QueryTexture(EnemyBulletTemple.texture, nullptr, nullptr, &EnemyBulletTemple.width, &EnemyBulletTemple.heigth);
	EnemyBulletTemple.width /= 4;
	EnemyBulletTemple.heigth /= 4;

	ExplodeTemple.texture = IMG_LoadTexture(game.GetRenderer(), "image/explosion.png");
	if (ExplodeTemple.texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Init Explode Error: %s\n", SDL_GetError());
	}
	SDL_QueryTexture(ExplodeTemple.texture, NULL, NULL, &ExplodeTemple.width, &ExplodeTemple.heigth);
	ExplodeTemple.TotalFrame = ExplodeTemple.width / ExplodeTemple.heigth;
	ExplodeTemple.width = ExplodeTemple.heigth;

	HealthTemple.texture = IMG_LoadTexture(game.GetRenderer(), "image/bonus_life.png");
	if (HealthTemple.texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Init SDL Health Error: %s\n", SDL_GetError());
	}
	SDL_QueryTexture(HealthTemple.texture, NULL,NULL, &HealthTemple.width, &HealthTemple.height);
	HealthTemple.width /= 4;
	HealthTemple.height /= 4;

}

void ScencMain::HandleEvent(SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			auto Scenctitle = new ScencTitle();
			game.ChangeScenc(Scenctitle);
		}
	}
}

void ScencMain::Updata(float deltatime)
{
	KeyboardControl(deltatime);
	UpdataPleyerBullte(deltatime);
	SpawnEnemy();
	UpdataEnemy(deltatime);
	UpdataEnemyBullet(deltatime);
	UpdataPlayer();
	UpdataExplosion(deltatime);
	UpdateItem(deltatime);
	if (IsPlayerDead)
	{
		ChengeSceneDelay(deltatime, 2);
	}
}

void ScencMain::Render()
{
	//��Ⱦ�ӵ�
	RenderPlayerBullte();
	RenderEnemyBullet();

	//��Ⱦ���
	if (!IsPlayerDead)
	{
		SDL_Rect rect = { static_cast<int>(player.postion.x),
	static_cast<int>(player.postion.y),
	player.width,
	player.heigth };
		SDL_RenderCopy(game.GetRenderer(), player.texture, nullptr, &rect);
	}
	
	//��Ⱦ����
	RenderEnemy();
	RenderItem();
	//��ÿһ֡�������洢��ը������������ھ���Ⱦ������bool�ж��Ƿ���Ҫ��ը
	RenderExxplosion();
	RenderUIhealth();
}

void ScencMain::Clean()
{
	for (auto& sound : Sound)
	{
		if (sound.second != nullptr)
		{
			Mix_FreeChunk(sound.second);
		}
	}
	Sound.clear();
	for (auto& Bulltes : PlayerBulltes)
	{
		if (Bulltes != nullptr)
		{
			delete Bulltes;
		}
	}
	for (auto& enemy : Enemies)
	{

		if (enemy != nullptr)
		{
			delete enemy;
		}
	}
	for (auto& EnemyBullet : EnemyBullets)
	{
		if (EnemyBullet != nullptr)
		{
			delete EnemyBullet;
		}
	}
	for (auto& explosion : Explosions)
	{
		if (explosion != nullptr)
		{
			delete explosion;
		}
	}
	for (auto& itemshealth : Healths)
	{
		if (itemshealth != nullptr)
		{
			delete itemshealth;
		}
	}
	PlayerBulltes.clear();
	if (PlayerBullteTemple.texture != nullptr)
	{
		SDL_DestroyTexture(PlayerBullteTemple.texture);
	}
	if (player.texture != nullptr)
	{
		SDL_DestroyTexture(player.texture);
	}
	if (EnemyTemple.texture != nullptr)
	{
		SDL_DestroyTexture(EnemyTemple.texture);
	}
	if (EnemyBulletTemple.texture != nullptr)
	{
		SDL_DestroyTexture(EnemyBulletTemple.texture);
	}
	if (ExplodeTemple.texture != nullptr)
	{
		SDL_DestroyTexture(ExplodeTemple.texture);
	}
	if (HealthTemple.texture != nullptr)
	{
		SDL_DestroyTexture(HealthTemple.texture);
	}
	if (UIHealth != nullptr)
	{
		SDL_DestroyTexture(UIHealth);
	}
	if (ScoreFont != nullptr)
	{
		TTF_CloseFont(ScoreFont);
	}
	if (bgm != nullptr)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(bgm);
	}
}

void ScencMain::KeyboardControl(float deltatime)
{
	if (IsPlayerDead)
	{
		return;
	}
	auto keyboardState = SDL_GetKeyboardState(NULL);
	if (keyboardState[SDL_SCANCODE_W])
	{
		player.postion.y -= deltatime*player.Speed;
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		player.postion.y += deltatime * player.Speed;
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		player.postion.x += deltatime * player.Speed;
	}
	if (keyboardState[SDL_SCANCODE_A])
	{
		player.postion.x -= deltatime * player.Speed;
	}

	if (player.postion.x < 0)
	{
		player.postion.x = 0;
	}
	if (player.postion.x > game.GetWindowWidth() - player.width)
	{
		player.postion.x = game.GetWindowWidth() - player.width;
	}
	if (player.postion.y < 0)
	{
		player.postion.y = 0;
	}
	if (player.postion.y > game.GetWindowHeigth() - player.heigth)
	{
		player.postion.y = game.GetWindowHeigth() - player.heigth;
	}

	if (keyboardState[SDL_SCANCODE_J])
	{
		auto currenttime = SDL_GetTicks();
		if (currenttime - player.LastShootTime > player.CoolDown)
		{
			PlayerShootBullte();
			player.LastShootTime = currenttime;
		}
	}
}

//��������ӵ��������������
void ScencMain::PlayerShootBullte()
{
	auto Player_Bulltes = new PlayerBullte(PlayerBullteTemple);
	Player_Bulltes->postion.x = player.postion.x + player.width / 2 - Player_Bulltes->width / 2;
	Player_Bulltes->postion.y = player.postion.y;
	PlayerBulltes.push_back(Player_Bulltes);
	Mix_PlayChannel(0, Sound["player_shoot"], 0);
}

//��������ӵ��ƶ����Ƿ���ײ�л�
void ScencMain::UpdataPleyerBullte(float deltatime)
{
	for (list<PlayerBullte*>::iterator it = PlayerBulltes.begin(); it != PlayerBulltes.end();)
	{
		auto Bullte = *it;
		Bullte->postion.y -= Bullte->Speed * deltatime;
		if (Bullte->postion.y + 32 < 0)
		{
			delete Bullte;
			it = PlayerBulltes.erase(it);//ɾ���������ָ����һ��λ��
			SDL_Log("Bullte had delete");
		}
		else
		{
			bool IsHit = false;
			for (auto& enemy : Enemies)
			{
				SDL_Rect enemyrect = {
					static_cast<int>(enemy->postion.x),
					static_cast<int>(enemy->postion.y),
					enemy->width,
					enemy->heigth
				};
				SDL_Rect playerbulletrect = {
					static_cast<int>(Bullte->postion.x),
					static_cast<int>(Bullte->postion.y),
					Bullte->width,
					Bullte->heigth
				};
				if (SDL_HasIntersection(&enemyrect, &playerbulletrect))
				{
					enemy->currenthealth -= Bullte->damage;
					delete Bullte;
					it = PlayerBulltes.erase(it);
					IsHit = true;
					Mix_PlayChannel(-1, Sound["hit"], 0);
					break;//��ײ���˳�ѭ�����������һ��enemy
				}
			}
			if (!IsHit)
			{
				++it;
			}
			
		}
	}
}

//������ң��Ƿ�����
void ScencMain::UpdataPlayer()
{
	if (IsPlayerDead == true)
	{
		return;
	}
	if (player.currentHealth <= 0)
	{
		IsPlayerDead = true;
		//���������ӱ�ը�����Ϣ
		auto currenttime = SDL_GetTicks();
		auto explosion = new Explode(ExplodeTemple);
		explosion->position.x = player.postion.x + player.width / 2 - explosion->width / 2;
		explosion->position.y = player.postion.y + player.heigth / 2 - explosion->heigth / 2;
		explosion->StratTime = currenttime;
		Explosions.push_back(explosion);
		Mix_PlayChannel(-1, Sound["player_explode"], 0);
		//�������ʱ��������¼
		game.SetFinalScore(Score);
		return;
	}
	for (auto& enemy : Enemies)
	{
		SDL_Rect enemyrect = {
			static_cast<int>(enemy->postion.x),
			static_cast<int>(enemy->postion.y),
			enemy->width,
			enemy->heigth
		};
		SDL_Rect playerrect = {
			static_cast<int>(player.postion.x),
			static_cast<int>(player.postion.y),
			player.width,
			player.heigth
		};
		if (SDL_HasIntersection(&enemyrect, &playerrect))
		{
			player.currentHealth -= 1;
			enemy->currenthealth = 0;
		}
	}
}

//��Ⱦ����ӵ�
void ScencMain::RenderPlayerBullte()
{
	for (auto Bulltes : PlayerBulltes)
	{
		SDL_Rect BullteRange= {
			static_cast<int>(Bulltes->postion.x),
			static_cast<int>(Bulltes->postion.y),
			Bulltes->width,
			Bulltes->heigth
		};
		SDL_RenderCopy(game.GetRenderer(), Bulltes->texture, nullptr, &BullteRange);
	}
}

//���������ɵл�
void ScencMain::SpawnEnemy()
{
	if (dis(gen) > 1 / 60.0f)
	{
		return;
	}
	auto enemy = new Enemy(EnemyTemple);
	enemy->postion.x = dis(gen) * (game.GetWindowWidth() - enemy->width);
	enemy->postion.y = -enemy->heigth;
	Enemies.push_back(enemy);
	
}

//���µл�״̬�������ӵ�
void ScencMain::UpdataEnemy(float deltatime)
{
	Uint32 CurrentTime = SDL_GetTicks();
	for (list<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end();)
	{
		auto enemy = *it;
		enemy->postion.y += enemy->Speed * deltatime;
		if (enemy->postion.y > game.GetWindowHeigth())
		{
			delete enemy;
			it = Enemies.erase(it);
			SDL_Log("Enemy has delete");
		}
		else
		{
			if (CurrentTime - enemy->LastShootTime > enemy->CoolDown && IsPlayerDead == false)
			{
				EnemyShoot(enemy);
				enemy->LastShootTime = CurrentTime;
			}
			if (enemy->currenthealth <= 0)
			{
				EnemyExplode(enemy);
				it = Enemies.erase(it);
			}
			else
			{
				++it;
			}
			
		}
	}
}

//���µл��ӵ�λ�ã��Ƿ�����ҷ�����ײ
void ScencMain::UpdataEnemyBullet(float deltatime)
{
	int brind = 32;
	for (list<EnemyBullet*>::iterator it = EnemyBullets.begin(); it != EnemyBullets.end();)
	{
		auto enemybullet = *it;
		enemybullet->postion.x += enemybullet->direction.x * deltatime * enemybullet->Speed;
		enemybullet->postion.y += enemybullet->direction.y * deltatime * enemybullet->Speed;
		if (enemybullet->postion.y > game.GetWindowHeigth() + brind ||
			enemybullet->postion.y<-brind ||
			enemybullet->postion.x>game.GetWindowWidth() + brind ||
			enemybullet->postion.x < -brind)
		{
			delete enemybullet;
			SDL_Log("Enemy Bullet destory");
			it=EnemyBullets.erase(it);
		}
		else
		{
			SDL_Rect enemybulletrect = {
				static_cast<int>(enemybullet->postion.x),
				static_cast<int>(enemybullet->postion.y),
				enemybullet->width,
				enemybullet->heigth
			};
			SDL_Rect playerrect = {
				static_cast<int>(player.postion.x),
				static_cast<int>(player.postion.y),
				player.width,
				player.heigth
			};

			if (SDL_HasIntersection(&enemybulletrect, &playerrect)&&!IsPlayerDead)
			{
				player.currentHealth -= enemybullet->damage;
				delete enemybullet;
				it = EnemyBullets.erase(it);
				Mix_PlayChannel(-1, Sound["hit"], 0);
			}
			else
			{
				++it;
			}
			
		}
	}
}

//��Ⱦ�л�
void ScencMain::RenderEnemy()
{
	for (auto enemy : Enemies)
	{
		SDL_Rect enemyRange = {
			static_cast<int>(enemy->postion.x),
			static_cast<int>(enemy->postion.y),
			enemy->width,
			enemy->heigth
		};
		SDL_RenderCopy(game.GetRenderer(), enemy->texture, NULL, &enemyRange);
	}
}

//��Ⱦ�л��ӵ�,��ת�ӵ�
void ScencMain::RenderEnemyBullet()
{
	for (auto& enemybullet : EnemyBullets)
	{
		SDL_Rect enemybulletrect = {
			static_cast<int>(enemybullet->postion.x),
			static_cast<int>(enemybullet->postion.y),
			enemybullet->width,
			enemybullet->heigth
		};
		float angle = atan2(enemybullet->direction.y, enemybullet->direction.x) * 180 / M_PI-90;
		//SDL_RenderCopy(game.GetRenderer(), enemybullet->texture, NULL, &enemybulletrect);
		SDL_RenderCopyEx(game.GetRenderer(), enemybullet->texture, NULL, &enemybulletrect, angle, NULL, SDL_FLIP_NONE);
	}
}

//�л������ӵ�
void ScencMain::EnemyShoot(Enemy* enemy)
{
	auto enemybullet = new EnemyBullet(EnemyBulletTemple);
	enemybullet->postion.x = enemy->postion.x + enemy->width / 2 - enemybullet->width / 2;
	enemybullet->postion.y = enemy->postion.y + enemy->heigth / 2 - enemybullet->heigth / 2;
	enemybullet->direction = GetDirection(enemy);
	EnemyBullets.push_back(enemybullet);
	Mix_PlayChannel(-1, Sound["enemy_shoot"], 0);
}

//����л�ָ����ҵĵ�λ�������������ӵ�
SDL_FPoint ScencMain::GetDirection(Enemy* enemy)
{
	auto x = (player.postion.x + player.width / 2) - (enemy->postion.x + enemy->width / 2);
	auto y = (player.postion.y + player.heigth / 2) - (enemy->postion.y + enemy->heigth / 2);
	auto length = sqrt(x * x + y * y);
	x /= length;
	y /= length;
	return SDL_FPoint{x,y};
}

//��ӵл���ը����Ч
void ScencMain::EnemyExplode(Enemy* enemy)
{
	auto currenttime = SDL_GetTicks();
	auto explode = new Explode(ExplodeTemple);
	explode->position.x = enemy->postion.x + enemy->width / 2 - explode->width / 2;
	explode->position.y = enemy->postion.y + enemy->heigth / 2 - explode->heigth / 2;
	explode->StratTime = currenttime;
	Mix_PlayChannel(-1, Sound["enemy_explode"], 0);
	Explosions.push_back(explode);
	if (dis(gen) < 0.5f)
	{
		DropItem(enemy);
	}
	Score += 10;
	delete enemy;
}

//���±�ը����Ч���ĳ�Ա����
void ScencMain::UpdataExplosion(float deltatime)
{
	//��Updata������ÿһ֡��ִ�иú�����������µ�ǰʱ�䣬����Ҫ��forѭ����
	auto currernttime = SDL_GetTicks();
	for (auto it = Explosions.begin(); it != Explosions.end();)
	{
		auto explosion = *it;
		explosion->CurrentFrame = (currernttime - explosion->StratTime) * explosion->FPS / 1000;
		if (explosion->CurrentFrame >= explosion->TotalFrame)
		{
			delete explosion;
			it = Explosions.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void ScencMain::RenderExxplosion()
{
	for (auto explosion : Explosions)
	{
		//���� ������Դ�ü����򡱣�src��
		SDL_Rect src = { explosion->CurrentFrame * explosion->width,0,explosion->width,explosion->heigth };
		SDL_Rect explosionrect =
		{
			static_cast<int>(explosion->position.x),
			static_cast<int>(explosion->position.y),
			explosion->width,
			explosion->heigth
		};
		SDL_RenderCopy(game.GetRenderer(), explosion->texture, &src, &explosionrect);
	}
}

void ScencMain::DropItem(Enemy* enemy)
{
	auto item = new Item(HealthTemple);
	item->position.x = enemy->postion.x + enemy->width / 2 - item->width / 2;
	item->position.y = enemy->postion.y + enemy->heigth / 2 - item->height / 2;
	float angle = dis(gen) * 2 * M_PI;
	item->direction.x = cos(angle);
	item->direction.y = sin(angle);
	Healths.push_back(item);
}

void ScencMain::UpdateItem(float deltatime)
{
	for (auto it = Healths.begin(); it != Healths.end();)
	{
		auto item = *it;

		item->position.x += item->direction.x * item->Speed * deltatime;
		item->position.y += item->direction.y * item->Speed * deltatime;
		//����
		if (item->position.x < 0 && item->bounceCount>0)
		{
			item->direction.x = -item->direction.x;
			item->bounceCount--;
		}
		if (item->position.x + item->width > game.GetWindowWidth() && item->bounceCount > 0)
		{
			item->direction.x = -item->direction.x;
			item->bounceCount--;
		}
		if (item->position.y < 0 && item->bounceCount>0)
		{
			item->direction.y = -item->direction.y;
			item->bounceCount--;
		}
		if (item->position.y + item->height > game.GetWindowHeigth() && item->bounceCount > 0)
		{
			item->direction.y = -item->direction.y;
			item->bounceCount--;
		}

		if (item->position.x + item->width<0 ||
			item->position.x>game.GetWindowWidth() ||
			item->position.y > game.GetWindowHeigth() ||
			item->position.y + item->height < 0)
		{
			delete item;
			it = Healths.erase(it);
		}
		else
		{
			SDL_Rect itemrect = {
				static_cast<int>(item->position.x),
				static_cast<int>(item->position.y),
				item->width,
				item->height
			};
			SDL_Rect playerrect = {
				static_cast<int>(player.postion.x),
				static_cast<int>(player.postion.y),
				player.width,
				player.heigth
			};
			if (SDL_HasIntersection(&itemrect, &playerrect)&&IsPlayerDead==false)
			{
				PlayerGetItem(item);
				delete item;
				it = Healths.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void ScencMain::PlayerGetItem(Item* item)
{
	Score += 5;
	if (item->type == ItemType::Health)
	{
		player.currentHealth += 1;
		if (player.currentHealth > player.MaxHealth)
		{
			player.currentHealth = player.MaxHealth;
		}
	}
	Mix_PlayChannel(-1, Sound["get_item"], 0);
}

void ScencMain::RenderItem()
{
	for (auto& item : Healths)
	{
		SDL_Rect itemrect = {
			static_cast<int>(item->position.x),
			static_cast<int>(item->position.y),
			item->width,
			item->height
		};
		SDL_RenderCopy(game.GetRenderer(), item->texture, NULL, &itemrect);
	}
}

void ScencMain::RenderUIhealth()
{
	int x = 10;
	int y = 10;
	int size = 32;
	int offest = 40;
	SDL_SetTextureColorMod(UIHealth, 100, 100, 100);
	for (int i = 0; i < player.MaxHealth; ++i)
	{
		SDL_Rect desRect = { x + i * offest,y,size,size };
		SDL_RenderCopy(game.GetRenderer(), UIHealth, NULL, &desRect);
	}
	SDL_SetTextureColorMod(UIHealth, 255, 255, 255);
	for (int i = 0; i < player.currentHealth; ++i)
	{
		SDL_Rect desRect = { x + i * offest,y,size,size };
		SDL_RenderCopy(game.GetRenderer(), UIHealth, NULL, &desRect);
	}

	auto text = "SCORE:" +to_string(Score);
	SDL_Color color = { 255,255,255,255 };
	SDL_Surface* surface = TTF_RenderUTF8_Solid(ScoreFont, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(game.GetRenderer(), surface);
	SDL_Rect des = { game.GetWindowWidth() - 10-surface->w,10,surface->w,surface->h};
	SDL_RenderCopy(game.GetRenderer(), texture, NULL, &des);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void ScencMain::ChengeSceneDelay(float deltatime, float delay)
{
	timerEnd += deltatime;
	if (timerEnd > delay)
	{
		auto Sceneend = new SceneEnd();
		game.ChangeScenc(Sceneend);
	}
}
