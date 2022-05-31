#include "Effects.h"

Effects::~Effects()
{
	delete firework, explosion;
}

void Effects::FwInit(DirectXCommon* dxCommon, Camera* camera)
{
	//エフェクトのインスタンス生成
	firework = new mEffekseer();
	//エフェクトのセット
	firework->EffekseerSetting(dxCommon, camera, (const EFK_CHAR*)L"Effects/10/firework.efk", (const EFK_CHAR*)L"Effects/Texture");

}

void Effects::Elinit(DirectXCommon* dxCommon, Camera* camera)
{
	explosion = new mEffekseer();
	explosion->EffekseerSetting(dxCommon, camera, (const EFK_CHAR*)L"Effects/10/explosion.efk", (const EFK_CHAR*)L"Effects/Texture");
}

void Effects::FwLoad(bool& isfirework)
{
	if (isfirework == true) {
		fwFlag = true;
	}
	if (fwFlag == true) {
		firework->Load_Effect();
		fwFlag = false;
		isfirework = false;
	}
}

void Effects::ElLoad(bool& isexplosion)
{
	if (isexplosion == true) {
		elFlag = true;
	}
	if (elFlag == true) {
		explosion->Load_Effect();
		elFlag = false;
		isexplosion = false;
	}
}

void Effects::FwUpdate(DirectXCommon* dxCommon, Camera* camera, Player* player, KeyboardInput* keyInput)
{
	firework->SetPosition(player->GetPosX(), player->GetPosY(), player->GetPosZ());
	firework->SetScale(4, 4, 4);
	firework->EffekseerUpdate(dxCommon, camera);
}

void Effects::ElUpdate(DirectXCommon* dxCommon, Camera* camera, Player* player, KeyboardInput* keyInput)
{
	explosion->SetPosition(player->GetPosX(), player->GetPosY(), player->GetPosZ());
	explosion->SetScale(10, 10, 10);
	explosion->EffekseerUpdate(dxCommon, camera);
}

void Effects::FwDraw(DirectXCommon* dxCommon)
{
	firework->EffekseerDraw(dxCommon->GetCommandList());
}

void Effects::ElDraw(DirectXCommon* dxCommon)
{
	explosion->EffekseerDraw(dxCommon->GetCommandList());
}
