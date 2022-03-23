#include "Boss.h"

using namespace DirectX;

Boss::Boss()
{
	boss = ModelDraw::Create();
	boss->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Plane));

	head = ModelDraw::Create();
	head->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Face));
	body = ModelDraw::Create();
	body->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Body));
	rightaram = ModelDraw::Create();
	rightaram->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Right_arm));
	leftaram = ModelDraw::Create();
	leftaram->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Left_arm));
	rightleg = ModelDraw::Create();
	rightleg->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Rightleg));
	leftleg = ModelDraw::Create();
	leftleg->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Leftleg));
}

Boss::~Boss()
{


}

void Boss::Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;


	head->SetParent(boss);
	body->SetParent(boss);
	rightaram->SetParent(boss);
	leftaram->SetParent(boss);
	rightleg->SetParent(boss);
	leftleg->SetParent(boss);
	//player->SetScale(Vector3(1, 1, 1));
	//player->SetPos(Vector3(0, 5, 0));
}

void Boss::Update()
{
	boss->SetRotation(Vector3(0.0f, 90.0f, 0.0f));
	boss->SetPos(Vector3(0.0f, 10.0f, 0.0f));

	boss->Update();
	head->Update();
	body->Update();
	rightaram->Update();
	leftaram->Update();
	rightleg->Update();
	leftleg->Update();
}

void Boss::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList *cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	head->Draw();
	body->Draw();
	rightaram->Draw();
	leftaram->Draw();
	rightleg->Draw();
	leftleg->Draw();
	ModelDraw::PostDraw();
}
