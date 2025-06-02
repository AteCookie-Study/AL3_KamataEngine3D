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
	static inline const float kAttenuation = 0.9f;	
	static inline const float kLimitRunSpeed = 0.5f;

	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.01f;
	static inline const float kLimitFallSpeed = 1.0;	
	static inline const float kJumpAcceleration = 0.2f;
	
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	enum class LRDirection {
		kRight, kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	
	private:

	//ワルド変換データ
 KamataEngine::WorldTransform worldTransform_;

 //モデル
 KamataEngine::Model* model_ = nullptr;

 

 // カメラ
 KamataEngine::Camera* camera_ = nullptr;



};