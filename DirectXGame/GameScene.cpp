#include "GameScene.h"
#include "MyMath.h"


using namespace KamataEngine;

//GameScene::~GameScene() { delete sprite_; }
GameScene::~GameScene() {
	delete model_;

	delete sprite_;
	delete player_;
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
	

	// ワルドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラ
	camera_.Initialize();

	// 　サウンドデータの読込み
	soundDataHandle_ = Audio::GetInstance()->LoadWave("mokugyo.wav");

	// 音声再生
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_,true);

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &camera_);

	// 要素数
	const uint32_t kNumBlockVertical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	// 要素数を変更する
	/*worldTransformBlocks_.resize(kNumBlockHorizontal);*/
	worldTransformBlocks_.resize(kNumBlockVertical);
	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++){
			if (j % 2 == 0)
				continue;
			
		// ワルドトランスフォームのインスタンスを生成
		worldTransformBlocks_[i][j] = new WorldTransform();
		worldTransformBlocks_[i][j]->Initialize();
		worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
		worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
	    }
	}

}



void GameScene::Update() {
	// スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();

	//座標を{2,1}移動
	position.x += 2.0f;
	position.y += 1.0f;
	
	// 移動した座標をスプライトに反応
	sprite_->SetPosition(position);

	// スペースキーを押した瞬間
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 音声を停止
		Audio::GetInstance()->StopWave(voiceHandle_);
	}

	//player update	
	player_->Update();

	//// デバッグテキストの表示
	//ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);	

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
	
	
}	

void GameScene::Draw() {
	// 描画処理
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	

	// 3Dモデルの描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

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