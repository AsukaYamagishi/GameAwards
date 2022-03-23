#include "OBJObject.h"
#include <time.h>
#include <cassert>

OBJObject::OBJObject()
{
	/*model = ModelDraw::Create();
	model->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));*/
}

OBJObject::~OBJObject()
{
}
;
void OBJObject::Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio, ModelManager::ModelName modelNum)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;


	model = ModelDraw::Create();
	model->SetModel(ModelManager::GetIns()->GetModel(modelNum));
	model->SetScale(Vector3(10, 10, 10));
}

void OBJObject::Update()
{	
	model->Update();
}

void OBJObject::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList *cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	model->Draw();
	ModelDraw::PostDraw();
}
