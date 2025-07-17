#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "MyMath.h"

class Enemy;

class Player {
public:
	Player();
	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 position);
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
		KamataEngine::Vector3 move;
	};

	void InputMove();
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
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
	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 1.0f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.01f;
	static inline const float kAttenuationLanding = 0.1f;
	static inline const float kGroundSearchHeight = 0.06f;
	static inline const float kAttenuationWall = 0.1f;
	/*static inline const float kAttenuatuinWall = 0.1f;*/

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	enum class LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	// 判定結果を反映して移動させる
	void MoveByCollisionResult(const CollisionMapInfo& info);

	void CheckMapLanding(const CollisionMapInfo& info);
	// 壁に接触している場合の処理
	void CheckMapHitWall(const CollisionMapInfo& info);

	void CheckMapCeiling(const CollisionMapInfo& info);

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();

	void OnCollision(const Enemy* enemy);

	bool isDead_ = false;

	bool IsDead() const { return isDead_; }	

private:
	// ワルド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	MapChipField* mapChipField_ = nullptr;
};