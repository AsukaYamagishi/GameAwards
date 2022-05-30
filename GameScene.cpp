#include "GameScene.h"
#include "ParticleManager.h"
#include <cassert>
#include <time.h>
#include "FbxInput.h"
#include "FbxDraw.h"
#include "CollisionManager.h"
#include "FbxCollisionManager.h"
#include"CollisionTypes.h"
#include"FbxMeshCollider.h"
#include "FbxSphereCollider.h"

using namespace DirectX;
enum mesh
{
	Ghead = 5,
	Gbody = 4,
	Grightarm = 3,
	Gleftarm = 2,
	Grightleg = 1,
	Gleftleg = 0,
	headToPlayer = 7,
};

enum targetParts {
	Thead = 1,
	Tleftarm,
	Trightarm,
	Tbody,
	Tleftleg,
	Trightleg,
};

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	//safe_deleteはここで行う
	safe_delete(particleMan);
	safe_delete(testObject);
	safe_delete(player);
	safe_delete(boss);
	safe_delete(stage);
	safe_delete(weapon);
	safe_delete(camera);
	safe_delete(collisionManager);
	safe_delete(fbxcollisionManager);
	//safe_delete(testModel);
}

void GameScene::Finalize()
{
	collisionManager->Finalize();
	fbxcollisionManager->Finalize();
}

void GameScene::Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio)
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


	//カメラのせっち
	camera->Initialize({ 0,30,130 });
	camera = Camera::GetCam();
	camera->target = { 0 ,50 ,0 };

#pragma region デバッグテキスト読み込み
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNum, L"Resources/DebugText.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Init(debugTextTexNum);

#pragma endregion

#pragma region Sprite初期設定
	// テクスチャ読み込み
	if (!Sprite::LoadTexture(2, L"Resources/sprite/redHP.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(3, L"Resources/sprite/blackHP.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(4, L"Resources/sprite/playerHP.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(5, L"Resources/sprite/rule.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(6, L"Resources/sprite/rule_keyboard.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(7, L"Resources/sprite/pose.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(8, L"Resources/sprite/pose_key.png")) {
		assert(0);
		return;
	}
	//// スプライト生成
	boss1HP_Red = Sprite::CreateSprite(2, { 310.0f,10.0f });
	boss1HP_Black = Sprite::CreateSprite(3, { 310.0f,10.0f });
	playerHP = Sprite::CreateSprite(4, { 1070, 522 });
	controler_rule = Sprite::CreateSprite(5, { 0,0 });
	ketboard_rule = Sprite::CreateSprite(6, { 0,0 });
	pose= Sprite::CreateSprite(7, { 1130,0 });
	pose_key= Sprite::CreateSprite(8, { 1130,-10 });
#pragma endregion
	//デバイスをセット
	FbxDraw::SetDevice(dxCommon->GetDevice());
	//カメラをセット
	FbxDraw::SetCamera(camera);
	//グラフィックスパイプライン生成
	FbxDraw::CreateGraphicsPipeline();
	cameraNumber = 0;
#pragma region 3DモデルCreate・初期設定

	//モデルを指定して読み込み
	testModel = FbxInput::GetInstance()->LoadFbxFromFile("Left_arm_walk");
	//3Dオブジェクト生成とモデルのセット
	testObject = new FbxDraw();
	testObject->Init();
	testObject->SetModel(testModel.get());
	//testObject->SetScale({ 0.01,0.0001,0.001 });
	testObject->SetScale({ 0.1,0.1,0.1 });
	testObject->SetRotation({ 0,0,0 });
	testObject->SetPosition({ 0,5,3 });
	testObject->PlayAnimation(true);
	FbxMeshCollider* testcollider = new FbxMeshCollider;
	testObject->SetCollider(testcollider);
	testcollider->ConstrucTriangles(testObject->GetModel());
	testcollider->tag = CollisionTag::TagHead;

	testsphereObject = new FbxDraw();
	testsphereObject->Init();
	testsphereObject->SetModel(testModel.get());
	//testObject->SetScale({ 0.01,0.0001,0.001 });
	testsphereObject->SetScale({ 0.01,0.01,0.01 });
	testsphereObject->SetRotation({ 0,0,0 });
	testsphereObject->SetPosition({ 0,0,0 });
	testsphereObject->PlayAnimation(true);

	testsphereObject->SetCollider(new FbxSphereCollider(XMVECTOR({ 0, 0, 0.0 }), 100));
	testsphereObject->collider->tag = CollisionTag::TagPlayer;

	//拾える武器用の矢印
	for (int i = 0; i < 5; i++)
	{
		arrow[i] = ModelDraw::Create();
		arrow[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::arrow));
		arrow[i]->SetRotation({90, 90, -90});
		arrow[i]->SetScale({7,7,7});
		DrawFlag[i] = false;
		frame = 0;
		downTimer[i] = 0;
	}

	//プレイヤーなど生成
	player = new Player();
	player->Initialize(dxCommon, keyInput,padInput, audio);

	boss = new Boss();
	boss->Initialize(dxCommon, keyInput, audio, player->player);
	boss->boss->SetPos(Vector3(0, 5, 0));
	boss->boss->SetRotation(Vector3(0.0f, 90.0f, 0.0f));

	stage = new OBJObject();
	stage->Initialize(dxCommon, keyInput, audio, ModelManager::Stage);
	stage->model->SetScale({ 30,30,30 });
	skydome = new OBJObject();
	skydome->Initialize(dxCommon, keyInput, audio, ModelManager::Skydome);

	weapon = new Weapon();
	weapon->Initialize(dxCommon, keyInput, audio);

	effects = std::make_unique<Effects>();
	effects->Initialize(dxCommon, camera);

	//プレイヤーに追従
	weapon->weapon->SetOBJParent(player->player);

	//パーティクルの生成
	particleMan = ParticleManager::Create();
	particleMan->Update();
#pragma endregion

#pragma region 音楽リソース初期設定

	soundNo = 0;

	soundData[0] = audio->SoundLoadWave("Resources/Sound/BGM/Title.wav");
	soundData[1] = audio->SoundLoadWave("Resources/Sound/BGM/Boss_01.wav");
	soundSE[0] = audio->SoundLoadWave("Resources/Sound/SE/Attacked_Boss01.wav");
	soundSE[1] = audio->SoundLoadWave("Resources/Sound/SE/Attacked_Boss02.wav");
	soundSE[2] = audio->SoundLoadWave("Resources/Sound/SE/Attacked_Player.wav");
	soundSE[3] = audio->SoundLoadWave("Resources/Sound/SE/Charge.wav");
	soundSE[4] = audio->SoundLoadWave("Resources/Sound/SE/Disassembly.wav");
	soundSE[5] = audio->SoundLoadWave("Resources/Sound/SE/WeaponAttack_Boss01.wav");
	soundSE[6] = audio->SoundLoadWave("Resources/Sound/SE/WeaponAttack_Normal.wav");
	//audio->SoundPlayWave(audio->xAudio2.Get(), soundData[soundNo], Audio::loop, 0.2f);

#pragma endregion

	gameFlag = false;
	poseFlag = false;

	//コリジョンマネージャーの生成
	collisionManager = CollisionManager::GetInstance();
	fbxcollisionManager = FbxCollisionManager::GetInstance();
}

bool GameScene::Update()
{
	for (int i = 0; i < 256; i++) {
		if (keyInput->PressKey(i)) {
			isKeyBoard = true;
			isGamePad = false;
		}
	}

	for (int i = 0; i < 32; i++) {
		if (padInput->IsPadButton(i) || padInput->IsPadStick(i + 1, 0.1f)) {
			isGamePad = true;
			isKeyBoard = false;
		}
	}

	if (cameraFlag == false) {
		//カメラ操作
		if (keyInput->PressKey(DIK_RIGHT)) {
			camera->matRot *= XMMatrixRotationY(0.1f);
		}
		else if (keyInput->PressKey(DIK_LEFT)) {
			camera->matRot *= XMMatrixRotationY(-0.1f);
		}
	}

	//コントローラによるカメラ操作
	camera->matRot *= XMMatrixRotationY(0.08f * padInput->IsPadStick(INPUT_AXIS_RX, 0.01f) / 1000);


#pragma region 当たり判定
	//Capsule capsule(Vector3(-5, +10, -30), Vector3(+5, -10, -20), 5, (0, 255, 255));
	////体の各部位のカプセル
	//Capsule headCapsule(Vector3(0, +50, 0), Vector3(0, 40, 0), 3, (0, 255, 255));
	//Capsule bodyCapsule(Vector3(0, 50, 5), Vector3(0, 20, 5), 6, (0, 255, 255));
	//Capsule rightAramCapsule(Vector3(-10, 50, 5), Vector3(-20, 5, -5), 5, (0, 255, 255));
	//Capsule leftAramCapsule(Vector3(20, 50, 5), Vector3(20, 5, 20), 1, (0, 255, 255));
	//Capsule rightLegCapsule(Vector3(-5, 20, 5), Vector3(-5, 0, 5), 1, (0, 255, 255));
	//Capsule leftLegCapsule(Vector3(5, 20, 5), Vector3(+5, 0, 5), 1, (0, 255, 255));

	////プレイヤーの攻撃範囲
	//Vector3 startpoint = player->player->GetPos();
	//Vector3 endpoint = player->player->GetPos() + Vector3(0.0f, 0.0f, 5.0f);
	//Capsule attackCapsule(startpoint, endpoint, 3, (0, 255, 255));
	int damage = 1;
	//敵のパーツ保持中（今回は左腕）
	if (player->enemyWepon == true)
	{
		/*endpoint = player->player->GetPos() + Vector3(0.0f, 0.0f, 80.0f);
		attackCapsule.endPosition = endpoint;
		attackCapsule.radius = 6;*/
		damage = 3;
	}
	else
	{
		damage = 1;
	}
#pragma endregion
	if (hit[headToPlayer])
	{
		debugText.PrintDebugText("headToPlayer", 500, 0);
	}

#pragma region プレイヤーのダメージ処理
	if (boss->GetAttackType() == AttackType::BEAM)
		if (hit[BulletToPlayer]) {
			player->HitDamege();
			debugText.PrintDebugText("aaaaaaaaaaaaaaaaitaiiiiiiiiiiiiiiiii", 0, 0);
		}

	if (boss->GetAttackType() == AttackType::PRESS && boss->timer > 20)
	{
		if (hit[BossAttackToPlayer])
		{
			player->HitDamege();
		}
	}

#pragma endregion


#pragma region 攻撃処理
	if (player->attack)
	{
		if (hit[WwaponToHead] && boss->parthp[head] > 0) {
			boss->HitDamage(head, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToBody]) {
			boss->HitDamage(body, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToRightArm] && boss->parthp[rightarm] > 0) {
			boss->HitDamage(rightarm, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToLeftArm] && boss->parthp[leftarm] > 0) {
			boss->HitDamage(leftarm, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToRightLeg] && boss->parthp[rightleg] > 0) {
			boss->HitDamage(rightleg, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToLeftLeg] && boss->parthp[leftleg] > 0) {
			boss->HitDamage(leftleg, damage);
			player->attack = false;
			particleMan->HitParticle();
		}

		/*if (mCollision::testCapsuleCapsule(headCapsule, attackCapsule) && boss->parthp[0] > 0)
		{
			debugText.PrintDebugText("head", 0, 0);
			boss->HitDamage(head, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(bodyCapsule, attackCapsule))
		{
			debugText.PrintDebugText("body", 0, 15);
			boss->HitDamage(body, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(rightAramCapsule, attackCapsule) && boss->parthp[2] > 0)
		{
			debugText.PrintDebugText("rightAram", 0, 30);
			boss->HitDamage(rightarm, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(leftAramCapsule, attackCapsule) && boss->parthp[3] > 0)
		{
			debugText.PrintDebugText("leftAram", 0, 45);
			boss->HitDamage(leftarm, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(rightLegCapsule, attackCapsule) && boss->parthp[4] > 0)
		{
			debugText.PrintDebugText("rightLeg", 0, 60);
			boss->HitDamage(rightleg, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(leftLegCapsule, attackCapsule) && boss->parthp[5] > 0)
		{
			debugText.PrintDebugText("leftLeg", 0, 75);
			boss->HitDamage(leftleg, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (!player->oldattack){
			audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[soundNo], Audio::not);
		}*/

#pragma region メッシュとの
			//頭
		if (hit[Ghead] && boss->parthp[head] > 0)
		{
			debugText.PrintDebugText("head", 0, 0);
			//boss->HitDamage(head, damage);
			player->attack = false;
			//particleMan->HitParticle();			
		}
		//体
		if (hit[Gbody] && boss->parthp[body] > 0)
		{
			debugText.PrintDebugText("body", 0, 15);
			//boss->HitDamage(body, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
		//右腕
		if (hit[Grightarm] && boss->parthp[rightarm] > 0)
		{
			debugText.PrintDebugText("rightAram", 0, 30);
			//boss->HitDamage(rightarm, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
		//左腕
		if (hit[Gleftarm] && boss->parthp[leftarm] > 0)
		{
			debugText.PrintDebugText("leftAram", 0, 45);
			//boss->HitDamage(leftarm, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
		//右足
		if (hit[Grightleg] && boss->parthp[rightleg] > 0)
		{
			debugText.PrintDebugText("rightLeg", 0, 60);
			//boss->HitDamage(rightleg, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
		//左足
		if (hit[Gleftleg] && boss->parthp[leftleg] > 0)
		{
			debugText.PrintDebugText("leftLeg", 0, 75);
			//boss->HitDamage(leftleg, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
#pragma endregion


		if (!player->oldattack) {
			//audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[soundNo], Audio::not);
		}

	}
#pragma endregion



#pragma region 落下処理
	//ボスのHPが 0 になったらパーツのHPも全部 0 にして全部落下させる
	if (boss->hp <= 0)
	{
		for (int i = 0; i < 5; i++)
		{
			boss->parthp[i] = 0;
		}
	}

	//矢印の座標
	arrowPos[0] = boss->head->GetPos();
	arrowPos[1] = boss->rightarm->GetPos();
	arrowPos[2] = boss->leftarm->GetPos();
	arrowPos[3] = boss->rightleg->GetPos();
	arrowPos[4] = boss->leftleg->GetPos();

	frame++;

	//パーツ落下処理
	if (boss->parthp[head] <= 0)
	{
		if (boss->head->GetOBJParent() == boss->boss) {
			boss->head->SetOBJParent(nullptr);
			boss->head->SetPos(boss->boss->GetPos());
		}
		if (boss->head->GetOBJParent() == nullptr) {
			boss->Fall(head);
			DrawFlag[0] = true;
			arrow[0]->SetRotation({ 90, 90, -90 + frame });

			downTimer[0]++;

			if (downTimer[0] >= 50)
			{
				arrow[0]->SetPos({ arrowPos[0].x, arrowPos[0].y + 70 - (downTimer[0] / 10), arrowPos[0].z - 10 });
				downTimer[0] = 0;
			}
			else {
				arrow[0]->SetPos({ arrowPos[0].x, arrowPos[0].y + 65 + (downTimer[0] / 10), arrowPos[0].z - 10 });
			}
		}
	}
	if (boss->parthp[body] <= 0)
	{
		if (boss->hp <= 0)
		{
			boss->Fall(body);
		}
	}
	if (boss->parthp[rightarm] <= 0)
	{
		if (boss->rightarm->GetOBJParent() == nullptr) {
			boss->Fall(rightarm);
			arrow[1]->SetRotation({ 90, 90, -90 + frame });
			DrawFlag[1] = true;

			downTimer[1]++;

			if (downTimer[1] >= 50)
			{
				arrow[1]->SetPos({ arrowPos[1].x, arrowPos[1].y + 40 - (downTimer[1] / 10), arrowPos[1].z - 50 });
				downTimer[1] = 0;
			}
			else {
				arrow[1]->SetPos({ arrowPos[1].x, arrowPos[1].y + 35 + (downTimer[1] / 10), arrowPos[1].z - 50 });
			}
		}
		if (boss->rightarm->GetOBJParent() == boss->boss) {
			boss->rightarm->SetOBJParent(nullptr);
			boss->rightarm->SetPos(boss->boss->GetPos());
		}
	}
	if (boss->parthp[leftarm] <= 0)
	{
		if (boss->leftarm->GetOBJParent() == nullptr) {
			boss->Fall(leftarm);
			
			arrow[2]->SetRotation({ 90, 90, -90 + frame });
			DrawFlag[2] = true;

			downTimer[2]++;

			if (downTimer[2] >= 50)
			{
				arrow[2]->SetPos({ arrowPos[2].x - 10, arrowPos[2].y + 40 - (downTimer[2] / 10), arrowPos[2].z + 10});
				downTimer[2] = 0;
			}
			else {
				arrow[2]->SetPos({ arrowPos[2].x - 10, arrowPos[2].y + 70, arrowPos[2].z + 10 });
			}
		}
		if (boss->leftarm->GetOBJParent() == boss->boss) {
			boss->leftarm->SetOBJParent(nullptr);
			boss->leftarm->SetPos(boss->boss->GetPos());
		}
	}
	if (boss->parthp[rightleg] <= 0)
	{
		if (boss->rightleg->GetOBJParent() == nullptr) {
			boss->Fall(rightleg);
			arrow[3]->SetRotation({ 90, 90, -90 + frame });
			DrawFlag[3] = true;

			downTimer[3]++;

			if (downTimer[3] >= 50)
			{
				arrow[3]->SetPos({ arrowPos[3].x - 10, arrowPos[3].y + 40 - (downTimer[3] / 10), arrowPos[3].z});
				downTimer[3] = 0;
			}
			else {
				arrow[3]->SetPos({ arrowPos[3].x - 10, arrowPos[3].y + 35 + (downTimer[3] / 10), arrowPos[3].z});
			}
		}
		if (boss->rightleg->GetOBJParent() == boss->boss) {
			boss->rightleg->SetOBJParent(nullptr);
			boss->rightleg->SetPos(boss->boss->GetPos());
		}

	}
	if (boss->parthp[leftleg] <= 0)
	{
		if (boss->leftleg->GetOBJParent() == nullptr) {
			boss->Fall(leftleg);
			arrow[4]->SetRotation({ 90, 90, -90 + frame });
			DrawFlag[4] = true;

			downTimer[4]++;

			if (downTimer[4] >= 50)
			{
				arrow[4]->SetPos({ arrowPos[4].x - 10, arrowPos[4].y + 40 - (downTimer[4] / 10), arrowPos[4].z });
				downTimer[4] = 0;
			}
			else {
				arrow[4]->SetPos({ arrowPos[4].x - 10, arrowPos[4].y + 35 + (downTimer[4] / 10), arrowPos[4].z });
			}
		}
		if (boss->leftleg->GetOBJParent() == boss->boss) {
			boss->leftleg->SetOBJParent(nullptr);
			boss->leftleg->SetPos(boss->boss->GetPos());
		}

	}

	//デバッグ用にパーツに直接ダメージ
	if (keyInput->PressKeyTrigger(DIK_1))
	{
		boss->parthp[rightarm]--;
	}
	if (keyInput->PressKeyTrigger(DIK_2))
	{
		boss->parthp[leftarm]--;
		particleMan->HitParticle();
	}
	if (keyInput->PressKeyTrigger(DIK_3))
	{
		boss->parthp[rightleg]--;
	}
	if (keyInput->PressKeyTrigger(DIK_4))
	{
		boss->parthp[leftleg]--;
	}
#pragma endregion

#pragma region HPバーのサイズ
	/*-------------ボス-------------*/
	//float hpSize = (boss1HP_SizeX / boss->hp) * boss->hp;
	float hpSize = (boss1HP_SizeX / boss->maxhp) * boss->hp;
	if (boss->hp < 0) { boss->hp = 0; }
	boss1HP_Red->SetSize({ hpSize, boss1HP_SizeY });

	//char str[256];
	//sprintf_s(str, "hpSize : %f", hpSize);
	//debugText.PrintDebugText(str, 0, 0, 1);

	/*-------------プレイヤー-------------*/
	playerHP->SetSize({ playerHPX, playerHPY });
	playerHP->SetTextureRect({ 128.0f * (playerMaxHp - player->hp),0 }, { 128, 128 });

#pragma endregion

#pragma region 部位の取得
	if (keyInput->PressKey(DIK_R) || padInput->IsPadButtonTrigger(XBOX_INPUT_Y)) {

		if (hit[WwaponToBody]) {
			//ボディが壊れたらボス死亡
		}
		if (hit[WwaponToHead] && boss->parthp[head] <= 0 && boss->head->GetOBJParent() == nullptr) {
			boss->head->SetPos(Vector3(0, 0, 0));
			boss->head->SetRotation(Vector3(0, 0, 0));
			boss->head->SetOBJParent(player->player);
			boss->head->SetPos(Vector3(5, -35, 0));
			boss->head->SetRotation(Vector3(20, -90, 0));
			player->enemyWepon = true;
			//球を拾ったフラグ
			player->headFlag = true;
		}
		if (hit[WwaponToRightArm] && boss->parthp[rightarm] <= 0 && boss->rightarm->GetOBJParent() == nullptr) {
			boss->rightarm->SetPos(Vector3(0, 0, 0));
			boss->rightarm->SetRotation(Vector3(0, 0, 0));
			boss->rightarm->SetOBJParent(player->player);
			boss->rightarm->SetPos(Vector3(-12, 19, 51));
			boss->rightarm->SetRotation(Vector3(0, 0, 190));
			player->enemyWepon = true;
		}
		if (hit[WwaponToLeftArm] && boss->parthp[leftarm] <= 0 && boss->leftarm->GetOBJParent() == nullptr) {
			boss->leftarm->SetPos(Vector3(0, 0, 0));
			boss->leftarm->SetRotation(Vector3(0, 0, 0));
			boss->leftarm->SetOBJParent(player->player);
			boss->leftarm->SetPos(Vector3(-30, 45, -7));
			boss->leftarm->SetRotation(Vector3(0, 0, -170));
			player->enemyWepon = true;
		}
		if (hit[WwaponToRightLeg] && boss->parthp[rightleg] <= 0 && boss->rightleg->GetOBJParent() == nullptr) {
			boss->rightleg->SetPos(Vector3(0, 0, 0));
			boss->rightleg->SetRotation(Vector3(0, 0, 0));
			boss->rightleg->SetOBJParent(player->player);
			boss->rightleg->SetPos(Vector3(-16, 5, 10));
			boss->rightleg->SetRotation(Vector3(0, 0, 200));
			player->enemyWepon = true;
		}
		if (hit[WwaponToLeftLeg] && boss->parthp[leftleg] <= 0 && boss->leftleg->GetOBJParent() == nullptr) {
			boss->leftleg->SetPos(Vector3(0, 0, 0));
			boss->leftleg->SetRotation(Vector3(0, 0, 0));
			boss->leftleg->SetOBJParent(player->player);
			boss->leftleg->SetPos(Vector3(-15, 7, -3));
			boss->leftleg->SetRotation(Vector3(0, 0, 190));
			player->enemyWepon = true;
		}

		for (int i = 0; i < 5; i++)
		{
			DrawFlag[i] = false;
		}
	}
#pragma endregion

#pragma region 武器にした部位を落とす
	else if (keyInput->PressKey(DIK_G) || (padInput->IsPadButtonTrigger(XBOX_INPUT_X) && player->enemyWepon))
	{
		//boss->head->SetParent(nullptr);
		//boss->body->SetParent(nullptr);
		//boss->rightarm->SetParent(nullptr);
		//boss->leftarm->SetParent(nullptr);
		//boss->rightleg->SetParent(nullptr);
		//boss->leftleg->SetParent(nullptr);
		player->enemyWepon = false;
		player->headFlag = false;

		//ボスの胴体は最後まで残る
		if (boss->parthp[body] <= 0)
		{
			if (boss->hp <= 0)
			{
				//boss->Fall(body);
			}
		}
		//パーツ落下処理
		if (boss->parthp[head] <= 0)
		{
			if (boss->head->GetOBJParent() == player->player) {
				boss->head->SetPos(Vector3(0, 0, 0));
				boss->head->SetOBJParent(nullptr);
				boss->head->SetPos(player->player->GetPos());
			}
			//boss->Fall(head);
		}
		if (boss->parthp[rightarm] <= 0)
		{
			if (boss->rightarm->GetOBJParent() == player->player) {
				boss->rightarm->SetPos(Vector3(0, 0, 0));
				boss->rightarm->SetOBJParent(nullptr);
				boss->rightarm->SetPos(player->player->GetPos());
			}
			//boss->Fall(rightarm);
		}
		if (boss->parthp[leftarm] <= 0)
		{
			if (boss->leftarm->GetOBJParent() == player->player) {
				boss->leftarm->SetOBJParent(nullptr);
				boss->leftarm->SetPos(player->player->GetPos() + Vector3(0.0f, 20.0f, 0.0f));
				boss->leftarm->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			}
			//boss->Fall(leftarm);
		}
		if (boss->parthp[rightleg] <= 0)
		{
			///boss->Fall(rightleg);
			if (boss->rightleg->GetOBJParent() == player->player) {
				boss->rightleg->SetOBJParent(nullptr);
				boss->rightleg->SetPos(player->player->GetPos());
				boss->rightleg->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			}

		}
		if (boss->parthp[leftleg] <= 0)
		{
			//boss->Fall(leftleg);
			if (boss->leftleg->GetOBJParent() == player->player) {
				boss->leftleg->SetOBJParent(nullptr);
				boss->leftleg->SetPos(player->player->GetPos());
			}

		}
	}

	if (keyInput->PressKeyTrigger(DIK_L)) {
		testObject->StopAnimation();
	}
#pragma endregion


	if (keyInput->PressKeyTrigger(DIK_P)) {
		audio->SoundStop(audio->xAudio2.Get(), Audio::IsLoop::loop);
		if (soundNo < 1) {
			soundNo++;
		}
		else {
			soundNo = 0;
		}
		audio->SoundPlayWave(audio->xAudio2.Get(), soundData[soundNo], Audio::loop, 0.2f);
	}

	if (keyInput->PressKeyTrigger(DIK_L)) {
		audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[seNo]);
		if (seNo < 6) {
			seNo++;
		}
		else {
			seNo = 0;
		}
	}

	if (keyInput->PressKeyTrigger(DIK_K)) {
		player->KnockBack();
	}

	if (keyInput->PressKeyTrigger(DIK_H)) {
		cameraNumber += 1;
	}
	if (boss->head->GetOBJParent() != boss->boss && cameraNumber == Thead) {
		cameraNumber += 1;
	}
	if (boss->rightarm->GetOBJParent() != boss->boss && cameraNumber == Tleftarm) {
		cameraNumber += 1;
	}
	if (boss->leftarm->GetOBJParent() != boss->boss && cameraNumber == Trightarm) {
		cameraNumber += 1;
	}
	if (boss->leftleg->GetOBJParent() != boss->boss && cameraNumber == Tleftleg) {
		cameraNumber += 1;
	}
	if (boss->rightleg->GetOBJParent() != boss->boss && cameraNumber == Trightleg) {
		cameraNumber += 1;
	}


	//debugText.PrintDebugText("WASD:MOVE", 25, 15, 1.5f);
	//debugText.PrintDebugText("SHIFT:JUMP", 25, 45, 1.5f);
	//debugText.PrintDebugText("SPACE:ATTACK", 25, 75, 1.5f);
	//debugText.PrintDebugText("R:PICK UP", 25, 105, 1.5f);
	//debugText.PrintDebugText("G:DROP", 25, 135, 1.5f);
	//debugText.PrintDebugText(":DROP", 25, 135, 1.5f);
	//debugText.PrintDebugText("M:RESET", 25, 135, 1.5f);
	/*player->Update(*camera, boss->boss->GetPos(), cameraFlag);
	stage->Update();
	skydome->Update();
	weapon->Update();*/
	testObject->Update();
	//testObject->Update();
	//カメラの設定
	//camera->eye = player->player->GetPos() + meye;
	//camera->eye.y -= 1.0f;
	//camera->eye.z -= 15.0f;
	//camera->target = player->player->GetPos() + mtarget;
	//camera->target.y = 10.0f;
	//testObject->Update();

	XMFLOAT3 rote = player->GetNoAttackRotation();
	XMFLOAT3 pos = player->player->GetPos();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, camera->matRot);

	movement *= XMVECTOR{ -1, -1, -1 };
	if (player->attack == false)
	{
		matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	}
	camera->eye = player->player->GetPos() + movement * XMVECTOR{ 100, 100, 100 };
	camera->eye.y = 20;
	camera->target = player->player->GetPos();
	//プレイヤーがジャンプした時視点だけ上に向くのを防止するための処理
	camera->target.y = player->graundheight;


	if (cameraNumber == 0) {
		cameraFlag = false;
	}
	else if (cameraNumber == Thead) {
		cameraFlag = true;
		Vector3 dir = boss->boss->GetPos() - player->player->GetPos();
		dir.Normalize();
		dir = dir * Vector3( -1, -1, -1 );
		camera->eye = player->player->GetPos() + dir * XMVECTOR{ 100, 100, 100 };
		camera->target = boss->boss->GetPos();
		camera->target.y = player->graundheight;
	}
	else if (cameraNumber == Tleftarm) {
		cameraFlag = true;
		Vector3 dir = boss->leftarm->GetPos() - player->player->GetPos();
		dir.Normalize();
		dir = dir * Vector3(-1, -1, -1);
		camera->eye = player->player->GetPos() + dir * XMVECTOR{ 100, 100, 100 };
		camera->target = boss->leftarm->GetPos();
		camera->target.y = player->graundheight;
	}
	else if (cameraNumber == Trightarm) {
		cameraFlag = true;
		Vector3 dir = boss->rightarm->GetPos() - player->player->GetPos();
		dir.Normalize();
		dir = dir * Vector3(-1, -1, -1);
		camera->eye = player->player->GetPos() + dir * XMVECTOR{ 100, 100, 100 };
		camera->target = boss->rightarm->GetPos();
		camera->target.y = player->graundheight;
	}
	else if (cameraNumber == Tbody) {
		cameraFlag = true;
		Vector3 dir = boss->body->GetPos() - player->player->GetPos();
		dir.Normalize();
		dir = dir * Vector3(-1, -1, -1);
		camera->eye = player->player->GetPos() + dir * XMVECTOR{ 100, 100, 100 };
		camera->target = boss->body->GetPos();
		camera->target.y = player->graundheight;
	}
	else if (cameraNumber == Tleftleg) {
		cameraFlag = true;
		Vector3 dir = boss->leftleg->GetPos() - player->player->GetPos();
		dir.Normalize();
		dir = dir * Vector3(-1, -1, -1);
		camera->eye = player->player->GetPos() + dir * XMVECTOR{ 100, 100, 100 };
		camera->target = boss->leftleg->GetPos();
		camera->target.y = player->graundheight;
	}
	else if (cameraNumber == Trightleg) {
		cameraFlag = true;
		Vector3 dir = boss->rightleg->GetPos() - player->player->GetPos();
		dir.Normalize();
		dir = dir * Vector3(-1, -1, -1);
		camera->eye = player->player->GetPos() + dir * XMVECTOR{ 100, 100, 100 };
		camera->target = boss->rightleg->GetPos();
		camera->target.y = player->graundheight;
	}
	else if (cameraNumber == 7) {
		cameraNumber = 0;
		cameraFlag = false;
	}


#pragma region ポーズ
	if (keyInput->PressKeyTrigger(DIK_ESCAPE) && poseFlag == false || (padInput->IsPadButtonTrigger(XBOX_INPUT_START) && poseFlag == false))
	{
		poseFlag = true;
	}
	else if (keyInput->PressKeyTrigger(DIK_ESCAPE) && poseFlag == true || (padInput->IsPadButtonTrigger(XBOX_INPUT_START) && poseFlag == true))
	{
		poseFlag = false;
	}
#pragma endregion

	if (poseFlag == false) {
		player->Update(*camera, boss->boss->GetPos(), cameraFlag);
		stage->Update();
		skydome->Update();
		weapon->Update();
		//testObject->Update();
		camera->SetCam(camera);
		camera->Update();
		boss->Update();
		//パーティクルマネージャ
		particleMan->Update();
		for (int i = 0; i < 5; i++)
		{
			arrow[i]->Update();
		}
		//スプライト更新
		boss1HP_Red->Update();
		boss1HP_Black->Update();
		playerHP->Update();
		controler_rule->Update();
		ketboard_rule->Update();
		pose->Update();
		pose_key->Update();
		effects->Update(dxCommon, camera, player);
	}
	
#pragma region デバッグテキスト設定
	//int型からatr型へ変換
	std::ostringstream oss;
	std::ostringstream ass;
	std::ostringstream iss;
	std::ostringstream uss;
	oss << boss->hp;
	debugText.PrintDebugText(oss.str(), 500, 0);
	ass << boss->angle;
	debugText.PrintDebugText(ass.str(), 700, 0);
	iss << player->knockBackFlag;
	debugText.PrintDebugText(iss.str(), 900, 0);
	uss << boss->watchB;
	debugText.PrintDebugText(uss.str(), 900, 100);


#pragma endregion
	//フラグを毎フレームリセットする
	for (int i = 0; i < 20; i++)
	{
		hit[i] = 0;
	}

	attackFlag[PlayerAttack] = player->attack;
	attackFlag[BossPress] = boss->attackType;
	attackFlag[BossBeam] = boss->attackType;
	//全ての衝突をチェック
	collisionManager->CheckAllCollision(hit, attackFlag, *keyInput);
	//fbxcollisionManager->CheckAllCollision(hit);

	//return false;
	//ボスが死んだらエンドシーンに移行
	if (keyInput->PressKeyTrigger(DIK_END) || boss->hp <= 0 || player->hp <= 0)
	{
		gameFlag = true;
		if (boss->hp <= 0)
		{
			winJudeg = true;
			return true;
		}
		else
		{
			winJudeg = false;
			return false;
		}
	}
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 背景スプライト描画
	//spriteBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

	//深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ModelDraw::PreDraw(cmdList);


	// 3Dオブクジェクトの描画
	for (int i = 0; i < 5; i++)
	{
		if (DrawFlag[i] == true)
		{
			arrow[i]->Draw();
		}
	}

	// 3Dオブジェクト描画後処理
	ModelDraw::PostDraw();

#pragma endregion

#pragma region 3Dモデル描画
	//testObject->Draw(cmdList);
	player->Draw();
	weapon->Draw();
	boss->Draw();
	stage->Draw();
	skydome->Draw();
	ParticleManager::PreDraw(cmdList);
	particleMan->Draw();
	ParticleManager::PostDraw();
	//effects->Draw(dxCommon);

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//前景スプライト描画
	boss1HP_Black->Draw();
	boss1HP_Red->Draw();
	playerHP->Draw();
	if (poseFlag == true)
	{
		if (isGamePad) {
			controler_rule->Draw();
		}
		if (isKeyBoard) {
			ketboard_rule->Draw();
		}
	}
	else {
		if (isGamePad) {
			pose->Draw();
		}
		if (isKeyBoard) {
			pose_key->Draw();
		}
	}
	
	
	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
