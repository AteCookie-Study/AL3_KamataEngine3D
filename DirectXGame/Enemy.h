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
	LRDirection lrDirection_ = LRDirection::kRight;

		// ワルド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	MapChipField* mapChipField_ = nullptr;
};

