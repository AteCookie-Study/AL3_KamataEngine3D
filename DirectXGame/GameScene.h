#pragma once
#include "KamataEngine.h"
#include "Player.h"




// ゲームシーン
class GameScene {
	private:


	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;

	//　サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;

	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;
	
	// ワルドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	//　カメラ
	KamataEngine::Camera camera_;

	//Player
	Player* player_ = nullptr;


public:
	~GameScene();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
};
