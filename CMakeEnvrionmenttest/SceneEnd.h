#pragma once
#include"Scenc.h"
#include<SDL.h>
#include<SDL_mixer.h>
#include<string>

class SceneEnd :public Scenc
{
public:
	void Init() override;
	void HandleEvent(SDL_Event* event) override;
	void Updata(float deltatime) override;
	void Render() override;
	void Clean() override;
private:
	bool IsTyping = true;
	void RenderPhase1();
	void RenderPhase2();
	std::string name = "";
};