#include "GameScene.h"


using namespace KamataEngine;

//GameScene::~GameScene() { delete sprite_; }
GameScene::~GameScene() {
	delete model_;
	delete sprite_;
	delete player_;
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
}	

void GameScene::Draw() {
	// 描画処理
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	

	// 3Dモデルの描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// player draw
	player_->Draw();


	// 3Dモデル描画後処理
	Model::PostDraw();

	
}