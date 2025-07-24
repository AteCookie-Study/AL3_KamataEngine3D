#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;
using namespace MathUtility;

// GameScene::~GameScene() { delete sprite_; }
GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;
	delete sprite_;
	delete player_;
	delete cameraController_;
	delete skydome_;
	delete debugCamera_;
	delete mapChipField_;
	delete deathModel_;
	
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {
	// 初期化処理
	phase_ = Phase::kPlay;
	
	textureHandle_ = TextureManager::Load("Cookie.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	input_ = Input::GetInstance();

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenearteBlocks();

	// ワルドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラ
	camera_.Initialize();

	// 　サウンドデータの読込み
	soundDataHandle_ = Audio::GetInstance()->LoadWave("mokugyo.wav");

	// 音声再生
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, false);

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	playerModel_ = Model::CreateFromOBJ("player");
	player_ = new Player();
	player_->Initialize(playerModel_, &camera_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	model_ = Model::CreateFromOBJ("block");

	// 敵
	enemyModel_ = Model::CreateFromOBJ("enemy");
	/*Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(18, 18);


	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, &camera_, enemyPosition);*/

	for (int32_t i = 0; i < kEnemyNum; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(18, 18 - i);
		newEnemy->Initialize(enemyModel_, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovavleArea(cameraArea);

	deathModel_ = Model::CreateFromOBJ("deathParticle");
	deathParticles_ = new DeathParticles();
	deathParticles_->Initialize(deathModel_, &camera_, playerPosition);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

}

void GameScene::Update() {
	ChangePhase();

	// スペースキーを押した瞬間
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 音声を停止
		Audio::GetInstance()->StopWave(voiceHandle_);
	}

	if (deathParticles_ && deathParticles_->IsFinished()) {
		// 死亡パーティクルが終了したらフェーズを変更
		phase_ = Phase::kPlay;
		
	}
	

	//// デバッグテキストの表示
	// ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);

	

	// デバッグカメラの更新
	debugCamera_->Update();
	camera_.matView = debugCamera_->GetCamera().matView;
	camera_.matProjection = debugCamera_->GetCamera().matProjection;

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// カメラの更新
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		/*camera_.UpdateMatrix();*/
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
	}

	
}

void GameScene::Draw() {
	// 描画処理
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデルの描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	modelSkydome_->Draw(worldTransform_, camera_);
	/// player draw
	if (player_->isDead_ == false) {
		player_->Draw();
	}
	// ブロックの描画

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// モデルの描画
			model_->Draw(*worldTransformBlock, camera_);
		}
	}

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	/*enemy_->Draw();*/

	deathParticles_->Draw();

	// 3Dモデル描画後処理
	Model::PostDraw();

	ChenckAllCollisions();

	
}

void GameScene::GenearteBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::ChenckAllCollisions() {
#pragma region

	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	}

#pragma endregion
}

void GameScene::ChangePhase() {

	skydome_->Update();

	// ブロック更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

	// カメラの転送
	cameraController_->Update();
	// スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();

	// 座標を{2,1}移動
	position.x += 2.0f;
	position.y += 1.0f;

	// 移動した座標をスプライトに反応
	sprite_->SetPosition(position);

	// enemy update
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	switch (phase_) {
	case Phase::kPlay:
		// player update
		player_->Update();

		if (player_->isDead_) {
			// 死亡した場合はフェーズを変更
			phase_ = Phase::kDeath;
			const Vector3& deathParticlesposition = player_->GetWorldPosition();
			deathParticles_->Initialize(deathModel_, &camera_, deathParticlesposition);

		}

		break;
	case Phase::kDeath:
		deathParticles_->Update();
		finished_ = deathParticles_->IsFinished();


		break;
	}

}
