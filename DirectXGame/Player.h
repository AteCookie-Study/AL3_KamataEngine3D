#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "MapChipField.h"

class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const KamataEngine::Vector3 position);
	void Update();
	void Draw();

	struct CollisionMapInfo {
		// 天井衡突フラグ
		bool ceiling = false;
		// 床衡突フラグ
		bool landing = false;
		// 壁衡突フラグ
		bool hitWall = false;
		// 移動量
		KamataEngine::Vector3 move_;
	};

	void InputMove();
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapMove();
	void CheckMapCeiling();
	void CheckMapWall();
	void CheckGroundState();
	void AnimateTurn();

	KamataEngine::Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.9f;	
	static inline const float kLimitRunSpeed = 0.5f;

	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.01f;
	static inline const float kLimitFallSpeed = 1.0;	
	static inline const float kJumpAcceleration = 0.2f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	enum class LRDirection {
		kRight, kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	enum Corner {
		kRightBottom,
		kLeftBottom,
		KRightTop,
		kLeftTop,

		kNumCorner
	};
	
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& cneter, Corner corner);

	private:

	//ワルド変換データ
 KamataEngine::WorldTransform worldTransform_;

 //モデル
 KamataEngine::Model* model_ = nullptr;

 

 // カメラ
 KamataEngine::Camera* camera_ = nullptr;

 MapChipField* mapChipField_ = nullptr;

};