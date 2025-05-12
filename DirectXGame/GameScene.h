#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "skydome.h"
#include <vector>





// ゲームシーン
class GameScene {
	private:

		KamataEngine::Input* input_ = nullptr;
	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;

	

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
	Skydome* skydome_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有效
	bool isDebugCameraActive_ = false;

	
	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

public:
	~GameScene();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
};
