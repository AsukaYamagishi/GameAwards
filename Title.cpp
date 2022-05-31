#include "Title.h"
#include <cassert>

using namespace DirectX;

Title::Title()
{
}

Title::~Title()
{
	//safe_deleteはここで行う
}

void Title::Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio)
{
#pragma region nullptrチェック/代入
	assert(dxCommon);
	assert(keyInput);
	assert(padInput);
	assert(audio);

	this->dxCommon = dxCommon;
	this->keyInput = keyInput;
	this->padInput = padInput;
	this->audio = audio;
#pragma endregion

#pragma region Sprite初期設定
	// テクスチャ読み込み(１番にするとよくわからんエラー起こる)
	if (!Sprite::LoadTexture(11, L"Resources/sprite/title.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(12, L"Resources/sprite/title_back.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(13, L"Resources/sprite/title_break.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(14, L"Resources/sprite/press_B.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(15, L"Resources/sprite/press_ENTER.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::CreateSprite(11, { 0.0f,0.0f });
	spriteBG->SetSize({ 1280, 720 });
	spriteBG->Update();

	background = Sprite::CreateSprite(12, { 0.0f,0.0f });
	background->SetSize({ 1280, 720 });
	background->Update();

	break_background = Sprite::CreateSprite(13, { 0.0f,0.0f });
	break_background->SetSize({ 1280, 720 });
	break_background->Update();

	press_B = Sprite::CreateSprite(14, { 70.0f,0.0f });
	press_B->SetSize({ 1280, 720 });
	press_B->Update();

	press_Enter = Sprite::CreateSprite(15, { 0.0f,0.0f });
	press_Enter->SetSize({ 1280, 720 });
	press_Enter->Update();
#pragma endregion

	
#pragma region 音楽リソース初期設定


	/*soundData = audio->SoundLoadWave("Resources/Sound/BGM/Title.wav");
	audio->SoundPlayWave(audio->xAudio2.Get(), soundData, Audio::loop, 0.2f);*/

#pragma endregion
	weaponFlag = false;
	breakFlag = false;
	timer = 0;
	weaponTimer = 0;
	rotZ = 180;
	posY = -280;
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			speed[x][y] = 0;
			rand_number[x][y] = 0;
		}
	}

#pragma region 3DモデルCreate・初期設定
	weapon = ModelDraw::Create();
	weapon->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Weapon));
	weapon->SetPos({ 0,posY,0 });
	weapon->SetScale({ 50,50,50 });
	weapon->SetRotation({ 0,90,rotZ });

	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			rubble[x][y] = ModelDraw::Create();
			rubble[x][y]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::rubble));
			rubble[x][y]->SetPos({ x * 10.0f - 30, 3.0f * y - 40, 0 });
			rubble[x][y]->SetScale({ 10,10,10 });
		}
	}
#pragma endregion

}

void Title::Update()
{
	
	/*if ((keyInput->PressKeyTrigger(DIK_RETURN)))
	{
		weaponFlag = true;
		
	}*/

	if (weaponFlag == true)
	{
		for (int y = 0; y < 6; y++)
		{
			for (int x = 0; x < 6; x++)
			{
				rand_number[x][y] = 0;
				speed[x][y] = 0;
			}
		}
		weaponTimer++;
		rotZ -= weaponTimer * 2;
		posY += weaponTimer * 2;
		weapon->SetRotation({ 0, 90, rotZ });
		weapon->SetPos({ 0,posY,0 });
		if (rotZ <= 0 && posY <= -80)
		{
			weaponFlag = false;
			breakFlag = true;
		}
	}
	
	if (breakFlag == true)
	{
		timer++;

		for (int y = 0; y < 6; y++)
		{
			for (int x = 0; x < 6; x++)
			{
				rand_number[x][y] = rand() % 5 + 1;
				speed[x][y] += rand_number[x][y];
				rubble[x][y]->SetPos({ x * (20.0f + speed[x][y]) - (50 + speed[x][y]), (13.0f + speed[x][y]) * y - (30 + speed[x][y]), -speed[x][y] + 60});
				rubble[x][y]->SetRotation({ speed[x][y] * 2, speed[x][y] * 2, 0});

			}
		}
	}

	if (timer >= 200)
	{
		breakFlag = false;
	}
	
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			rubble[x][y]->Update();
		}
	}
	weapon->Update();
	
}

void Title::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();

	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	//Object3D::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//object3d->Draw();

#pragma endregion

#pragma region 3Dモデル描画
	ModelDraw::PreDraw(cmdList);
	
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			rubble[x][y]->Draw();
		}
	}
	
	// 3Dオブジェクト描画後処理
	ModelDraw::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 背景スプライト描画
	if (breakFlag == true)
	{
		break_background->Draw();
	}
	else
	{
		background->Draw();
		spriteBG->Draw();
		if (isGamePad) {
			press_B->Draw();
		}
		if (isKeyboard) {
			press_Enter->Draw();
		}
		//press_Enter->Draw();
	}
	
	

	// デバッグテキストの描画
//debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();

	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dモデル描画
	ModelDraw::PreDraw(cmdList);

	if (weaponFlag == true || breakFlag == true)
	{
		weapon->Draw();
	}
	
	
	// 3Dオブジェクト描画後処理
	ModelDraw::PostDraw();
#pragma endregion
}
