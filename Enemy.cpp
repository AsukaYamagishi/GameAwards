#include "Enemy.h"
#include <stdlib.h>
#include <time.h>

DirectXCommon* Enemy::dxCommon = nullptr;

Enemy::Enemy()
{
	enemy = ModelDraw::Create();
	enemy->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Enemy));
	enemy->SetScale(Vector3(3, 3, 3));
	pos.x = rand() % 201 - 100;
	pos.y = rand() % 101 - 50;
	pos.z = 350.0f;
	enemy->SetPos(pos);
	isAlive = true;

	sphere.radius = 20.0f;
	sphere.centor = pos;
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(DirectXCommon* dxCommon)
{
	// nullptrチェック
	assert(dxCommon);

	Enemy::dxCommon = dxCommon;

	srand(time(NULL));
	
}

void Enemy::Update()
{
	pos.z -= 1.0f;
	enemy->SetPos(pos);

	sphere.centor = pos;

	enemy->Update();
}

void Enemy::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	enemy->Draw();
	ModelDraw::PostDraw();
}
