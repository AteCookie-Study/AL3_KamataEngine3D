#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"

class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const KamataEngine::Vector3 position);
	void Update();
	void Draw();
	KamataEngine::Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.1f;

	private:

	//ワルド変換データ
 KamataEngine::WorldTransform worldTransform_;

 //モデル
 KamataEngine::Model* model_ = nullptr;

 

 // カメラ
 KamataEngine::Camera* camera_ = nullptr;



};