#include "Weapon.h"
#include "Camera.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"

using namespace DirectX;
Weapon::Weapon()
{
	weapon = ModelDraw::Create();
	weapon->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Weapon));
}

Weapon::~Weapon()
{
}

void Weapon::Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	weapon->SetScale(Vector3(1, 1, 1));
	weapon->SetPos(Vector3(-3.0f, -2, 1.7f));
	weapon->SetRotation(Vector3(0, 45, 0));

	//コライダーの追加
	float radius = 0.0f;
	float radius2 = 5.0f;
	//球の当たり判定
	weapon->SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0.0 }), radius2));
	weapon->collider->tag = CollisionTag::TagWeapon;
}

void Weapon::Update()
{
	weapon->Update();
}

void Weapon::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	weapon->Draw();
	ModelDraw::PostDraw();
}
