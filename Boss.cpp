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
	rightaram->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Left_arm));
	leftaram = ModelDraw::Create();
	leftaram->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Right_arm));
	rightleg = ModelDraw::Create();
	rightleg->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Leftleg));
	leftleg = ModelDraw::Create();
	leftleg->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Rightleg));
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

	rightaram->SetPos(Vector3(8, 7, -18));
	leftaram->SetPos(Vector3(0, -2, 18));
	rightleg->SetPos(Vector3(0, 0, 0));
	leftleg->SetPos(Vector3(0, 0, 0));
}

void Boss::Update()
{
	//boss->SetRotation(Vector3(0.0f, 90.0f, 0.0f));
	//boss->SetPos(Vector3(0.0f, 10.0f, 0.0f));

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

void Boss::HitDamage(int part, int damage)
{
	hp -= damage;
	parthp[part] -= damage;
}

void Boss::Fall(int part)
{
	Vector3 pos;
	Vector3 fallspeed(0.0f, -0.5f, 0.0f);
	//頭
	if (part == Parts::head)
	{
		pos = head->GetPos();
		if (pos.y > -40)
		{
			head->SetPos(head->GetPos() + fallspeed);
		}
	}
	//からだ
	if (part == Parts::body)
	{
		pos = body->GetPos();
		if (pos.y > -20)
		{
			body->SetPos(body->GetPos() + fallspeed);
		}
	}
	//みぎうで
	if (part == Parts::rightaram)
	{
		pos = rightaram->GetPos();
		if (pos.y > -20)
		{
			rightaram->SetPos(rightaram->GetPos() + fallspeed);
		}
	}
	//ひだりうで
	if (part == Parts::leftaram)
	{
		pos = leftaram->GetPos();
		if (pos.y > -20)
		{
			leftaram->SetPos(leftaram->GetPos() + fallspeed);
		}
	}
	//みぎあし
	if (part == Parts::rightleg)
	{
		pos = rightleg->GetPos();
		if (pos.y > -10)
		{
			rightleg->SetPos(rightleg->GetPos() + fallspeed);
		}
	}
	//ひだりあし
	if (part == Parts::leftleg)
	{
		pos = leftleg->GetPos();
		if (pos.y > -10)
		{
			leftleg->SetPos(leftleg->GetPos() + fallspeed);
		}
	}
}
