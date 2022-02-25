#include "PlayerBullet.h"
#include <cassert>

using namespace DirectX;

DirectXCommon* PlayerBullet::dxCommon = nullptr;

PlayerBullet::PlayerBullet(Vector3 playerPos)
{
	bullet = ModelDraw::Create();
	bullet->SetModel(ModelManager::GetIns()->GetModel(ModelManager::PlayerBullet));
	pos = playerPos;;
	startPos = playerPos;;
	bullet->SetScale(Vector3(3, 3, 3));
	isAlive = true;
	sphere.radius = 8.0f;
	sphere.centor = playerPos;
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(DirectXCommon* dxCommon)
{
	// nullptrチェック
	assert(dxCommon);

	PlayerBullet::dxCommon = dxCommon;
}

void PlayerBullet::Update()
{
	pos.z += move;
	bullet->SetPos(pos);
	
	if (Vector3(startPos - pos).Length() > 300.0f)
	{
		isAlive = false;
	}

	sphere.centor = pos;

	bullet->Update();
}

void PlayerBullet::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	bullet->Draw();
	ModelDraw::PostDraw();
}
