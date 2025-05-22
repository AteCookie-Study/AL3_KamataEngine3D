#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"

class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const Vector3 position);
	void Update();
	void Draw();

	private:

	//ワルド変換データ
 KamataEngine::WorldTransform worldTransform_;

 //モデル
 KamataEngine::Model* model_ = nullptr;

 

 // カメラ
 KamataEngine::Camera* camera_ = nullptr;



};