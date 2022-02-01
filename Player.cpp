#include "Player.h"
#include <time.h>
#include <cassert>

using namespace DirectX;

Player::Player()
{
}

Player::~Player()
{
	safe_delete(activePlayerObj);
	safe_delete(*playerObj);

}

void Player::Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	//プレイヤー初期化
	{
		playerObj[DOWN] = PlayerModel::Create();
		playerObj[DOWN]->SetPosition({ 0.0f,-42.0f,0.0f });
		playerObj[DOWN]->SetScale({ 2.0f,2.0f,2.0f });
		playerObj[DOWN]->Update();

		playerObj[LEFT] = PlayerModel::Create();
		playerObj[LEFT]->SetPosition({ -42.0f,0.0f,0.0f });
		playerObj[LEFT]->SetScale({ 2.0f,2.0f,2.0f });
		playerObj[LEFT]->Update();

		playerObj[UP] = PlayerModel::Create();
		playerObj[UP]->SetPosition({ 0.0f,42.0f,0.0f });
		playerObj[UP]->SetScale({ 2.0f,2.0f,2.0f });
		playerObj[UP]->Update();

		playerObj[RIGHT] = PlayerModel::Create();
		playerObj[RIGHT]->SetPosition({ 42.0f,0.0f,0.0f });
		playerObj[RIGHT]->SetScale({ 2.0f,2.0f,2.0f });
		playerObj[RIGHT]->Update();
	}

	//アチーブ化プレイヤー初期化
	{
		activePlayerObj = PlayerActiveModel::Create();
		activePlayerObj->SetPosition({ 0.0f,-42.0f,0.0f });
		activePlayerObj->SetScale({ 3.0f,3.0f,3.0f });
		activePlayerObj->Update();
	}


	/*bullet2 = new Bullet2();
	bullet2->Initialize(dxCommon);*/
}

void Player::Update()
{
	XMFLOAT3 movePosition[4];
	for (int i = 0; i < 4; i++)
	{
		movePosition[i] = playerObj[i]->GetPosition();
	}
	
	
#pragma region	プレイヤー操作と弾
	/*if (bulletMoveFlag == 0) {
		if (input->PressKeyTrigger(DIK_SPACE)) {
			activePlayer++;

			if (activePlayer == 5) {
				activePlayer = 1;
			}
			if (activePlayer == 1) {
				bulletMoveFlag = 1;
				playerActive[DOWN] = 0;
				playerActive[RIGHT] = 1;
				bulletPos.x = movePosition[0].x;
				bulletPos.y = movePosition[0].y;
				bulletPos.z = movePosition[0].z;
				bullet->SetPosition(bulletPos);
			}
			if (activePlayer == 2) {
				bulletMoveFlag = 1;
				playerActive[RIGHT] = 0;
				playerActive[UP] = 1;
				bulletPos.x = movePosition[1].x;
				bulletPos.y = movePosition[1].y;
				bulletPos.z = movePosition[1].z;
				bullet->SetPosition(bulletPos);
			}
			if (activePlayer == 3) {
				bulletMoveFlag = 1;
				playerActive[UP] = 0;
				playerActive[LEFT] = 1;
				bulletPos.x = movePosition[2].x;
				bulletPos.y = movePosition[2].y;
				bulletPos.z = movePosition[2].z;
				bullet->SetPosition(bulletPos);
			}
			if (activePlayer == 4) {
				bulletMoveFlag = 1;
				playerActive[LEFT] = 0;
				playerActive[DOWN] = 1;
				bulletPos.x = movePosition[3].x;
				bulletPos.y = movePosition[3].y;
				bulletPos.z = movePosition[3].z;
				bullet->SetPosition(bulletPos);
			}
		}

		
	}
	if (bulletMoveFlag == 1)
	{
		if (activePlayer == 1) {
			bulletPos.y += 2;
			if (bulletPos.y >= 50) {
				bulletMoveFlag = 0;
				bulletPos = { -500,-500,-500 };
			}
		}
		if (activePlayer == 2) {
			bulletPos.x += 2;
			if (bulletPos.x >= 50) {
				bulletMoveFlag = 0;
				bulletPos = { -500,-500,-500 };
			}
		}
		if (activePlayer == 3) {
			bulletPos.y -= 2;
			if (bulletPos.y <= -50) {
				bulletMoveFlag = 0;
				bulletPos = { -500,-500,-500 };
			}
		}
		if (activePlayer == 4) {
			bulletPos.x -= 2;
			if (bulletPos.x <= -50) {
				bulletMoveFlag = 0;
				bulletPos = { -500,-500,-500 };
			}
			
		}
	}*/
#pragma endregion

#pragma region	プレイヤー移動
	// プレイヤーオブジェクト移動
	for (int i = 0; i < 4; i++)
	{
		if (i % 2 == 0)movePosition[i].x += playerSpeed[i];
		else movePosition[i].y += playerSpeed[i];
	}
	
	//反転通常処理
	{
		if (movePosition[0].x <= -43) { playerSpeed[0] = 0.5f; }
		else if (movePosition[0].x >= 43) { playerSpeed[0] = -0.5f; }
		if (movePosition[1].y >= 43) { playerSpeed[1] = -0.5f; }
		else if (movePosition[1].y <= -43) { playerSpeed[1] = 0.5f; }
		if (movePosition[2].x >= 43) { playerSpeed[2] = -0.5f; }
		else if (movePosition[2].x <= -43) { playerSpeed[2] = 0.5f; }
		if (movePosition[3].y <= -43) { playerSpeed[3] = 0.5f; }
		else if (movePosition[3].y >= 43) { playerSpeed[3] = -0.5f; }

	}
	
#pragma region	エネミー以外のSetter

	for (int i = 0; i < 4; i++)
	{
		playerObj[i]->SetPosition(movePosition[i]);
	}
	activePlayerObj->SetPosition(movePosition[activePlayer]);
	
	
	//bullet2->SetPosition(position2);

#pragma endregion

	for (int i = 0; i < 4; i++)
	{
		playerObj[i]->Update();
	}
	activePlayerObj->Update();
	
#pragma endregion

}

void Player::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();

	//全プレイヤー描画
	PlayerModel::PreDraw(cmdList);
	for (int i = 0; i < MAX; i++) {
		playerObj[i]->Draw();
	}
	PlayerModel::PostDraw();
	
	//アクティブなプレイヤーを描画
	PlayerActiveModel::PreDraw(cmdList);
	activePlayerObj->Draw();
	PlayerActiveModel::PostDraw();

}

void Player::ChangeActivePlayer()
{
	activePlayer++;
	if (activePlayer == MAX) {
		activePlayer = DOWN;
	}

}
