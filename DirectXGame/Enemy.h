#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"

///< summary>
/// 敵
/// </summary>
class Enemy {

public:
	Enemy();
	~Enemy();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 position);
	void Update();
	void Draw();

	void AnimateTurn();

	enum class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kLeft;

	static inline const float kWalkSpeed = 0.0005f;

	//最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 15.0f;

	// 　アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;
	float walkTimer_ = 0.0f;

	// 歩く速度
	KamataEngine::Vector3 velocity_ = {};

		// ワルド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	MapChipField* mapChipField_ = nullptr;
};

