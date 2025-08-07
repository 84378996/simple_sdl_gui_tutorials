#include "SceneBase.h"
#include "SceneManager.h"
#include "LaunchScene.h"

int main(int argc, char** argv)
{
	if (!sRender->InitRender())return 1;
	sRender->SetFontFile("D:/Projects/fonts/Arial.ttf");
	auto scene = SceneBase::Create<LaunchScene>();
	if (!scene) return 1;
	sSceneMgr->Run(scene);
	return 0;
}