#pragma once
#include "KamataEngine.h"

class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, uint32_t textureHanle, KamataEngine::Camera* camera);
	void Update();
	void Draw();

	private:

	//ワルド変換データ
 KamataEngine::WorldTransform worldTransform_;

 //モデル
 KamataEngine::Model* model_ = nullptr;

 // テクスチャハンドル
 uint32_t textureHandle_ = 0u;

 // カメラ
 KamataEngine::Camera* camera_ = nullptr;



};