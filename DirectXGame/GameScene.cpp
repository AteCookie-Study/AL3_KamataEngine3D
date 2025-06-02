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
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();
}


void GameScene::Initialize() {
	// 初期化処理
	textureHandle_ = TextureManager::Load("Cookie.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	input_ = Input::GetInstance();

	
	// ワルドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラ
	camera_.Initialize();

	// 　サウンドデータの読込み
	soundDataHandle_ = Audio::GetInstance()->LoadWave("mokugyo.wav");

	// 音声再生
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_,true);

	model_ = Model::CreateFromOBJ("player");
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1,18);
	player_->Initialize(model_, &camera_,playerPosition);
	

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_ ,&camera_);

	model_ = Model::CreateFromOBJ("block");

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenearteBlocks();

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	

	// 要素数
	//const uint32_t kNumBlockVertical = 10;
	//const uint32_t kNumBlockHorizontal = 20;
	//// ブロック1個分の横幅
	//const float kBlockWidth = 2.0f;
	//const float kBlockHeight = 2.0f;
	//// 要素数を変更する
	///*worldTransformBlocks_.resize(kNumBlockHorizontal);*/
	//worldTransformBlocks_.resize(kNumBlockVertical);
	//// キューブの生成
	//for (uint32_t i = 0; i < kNumBlockVertical; i++) {
	//	worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//	for (uint32_t j = 0; j < kNumBlockHorizontal; j++){
	//		if ((i + j) % 2 == 0)
	//			continue;
	//		
	//	// ワルドトランスフォームのインスタンスを生成
	//	worldTransformBlocks_[i][j] = new WorldTransform();
	//	worldTransformBlocks_[i][j]->Initialize();
	//	worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
	//	worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
	//    }
	//}

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280,720);
}



void GameScene::Update() {
	// スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();

	// 座標を{2,1}移動
	position.x += 2.0f;
	position.y += 1.0f;

	// 移動した座標をスプライトに反応
	sprite_->SetPosition(position);

	// スペースキーを押した瞬間
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 音声を停止
		Audio::GetInstance()->StopWave(voiceHandle_);
	}

	// player update
	player_->Update();

	//// デバッグテキストの表示
	// ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);

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

	// カメラの転送
	cameraController_->Update();
}


void GameScene::Draw() {
	// 描画処理
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	

	// 3Dモデルの描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	modelSkydome_->Draw(worldTransform_, camera_);
	// player draw
	player_->Draw();

	// ブロックの描画
	
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// モデルの描画
			model_->Draw(*worldTransformBlock, camera_);
		}
	}
	

	// 3Dモデル描画後処理
	Model::PostDraw();

	
}

void GameScene::GenearteBlocks() { 
	uint32_t numBlockVirtical = mapChipField_-> GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

    worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_-> GetMapChipTypeByIndex(j,i) == MapChipType::kBlock ){
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i); 
			}
		}
	}
}
