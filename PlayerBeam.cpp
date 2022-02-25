#include "PlayerBeam.h"

PlayerBeam::PlayerBeam()
{
	beam = ModelDraw::Create();
	beam->SetModel(ModelManager::GetIns()->GetModel(ModelManager::PlayerBullet));

}

PlayerBeam::~PlayerBeam()
{
	
}

void PlayerBeam::Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	beam->SetScale(Vector3(10, 10, 10));
}

void PlayerBeam::Update()
{
	beam->Update();
}

void PlayerBeam::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	beam->Draw();
	ModelDraw::PostDraw();
}
