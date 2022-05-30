#include "Effects.h"

Effects::~Effects()
{
	delete effect;
}

void Effects::Initialize(DirectXCommon* dxCommon, Camera* camera)
{
	//�G�t�F�N�g�̃C���X�^���X����
	effect = new mEffekseer();

	//�G�t�F�N�g�̃Z�b�g
	effect->EffekseerSetting(dxCommon, camera, (const EFK_CHAR*)L"Effects/10/SimpleLaser.efk", (const EFK_CHAR*)L"Effects");
}

void Effects::Update(DirectXCommon* dxCommon, Camera* camera, Player* player)
{
	effect->SetPosition(player->GetPosX(), player->GetPosY(), player->GetPosZ());
	//effect->SetRotation()
	effect->Load_Effect();

	effect->EffekseerUpdate(dxCommon, camera);
}

void Effects::Draw(DirectXCommon* dxCommon) 

{
	effect->EffekseerDraw(dxCommon->GetCommandList());
}
