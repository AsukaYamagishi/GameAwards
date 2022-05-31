#include "Effects.h"

Effects::~Effects()
{
	delete firework;
}

void Effects::FwInit(DirectXCommon* dxCommon, Camera* camera)
{
	//エフェクトのインスタンス生成
	firework = new mEffekseer();
	//エフェクトのセット
	firework->EffekseerSetting(dxCommon, camera, (const EFK_CHAR*)L"Effects/10/firework.efk", (const EFK_CHAR*)L"Effects/Texture");
}

void Effects::FwLoad(bool &isfirework)
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

void Effects::FwUpdate(DirectXCommon* dxCommon, Camera* camera, Player* player, KeyboardInput* keyInput)
{
	//if (keyInput->PressKey(DIK_Z)) {
	//	fireFlag = true;
	//	fwFlag = true;
	//}
	//if (fireFlag == true) {
	//	fireTimer++;
	//	firework->SetPosition(player->GetPosX(), player->GetPosY(), player->GetPosZ());
	//	firework->SetScale(2, 2, 2);
	//	if (fireTimer >= 10) {
	//		fireFlag = false;
	//		fwFlag = false;
	//		fireTimer = 0;
	//	}
	//}
	firework->SetPosition(player->GetPosX(), player->GetPosY(), player->GetPosZ());
	firework->SetScale(4, 4, 4);
	firework->EffekseerUpdate(dxCommon, camera);
}

void Effects::FwDraw(DirectXCommon* dxCommon)
{
	//if (fireFlag == true) {
		firework->EffekseerDraw(dxCommon->GetCommandList());
	//}
}