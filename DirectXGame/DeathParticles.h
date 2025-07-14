#pragma once
#include "KamataEngine.h"
#include <array>	
#include "MapChipField.h"
#include "MyMath.h"
#include <numbers>
#include <cassert>
#include <algorithm>


// <summary>
// DeathParticles.h
// </summary>

class DeathParticles {
public:

    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 position);
	void Update();
	void Draw();

	
// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	private:
	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	static inline const float kDuration = 1.0f;
	// 粒子速度
	static inline const float kSpeed = 0.1f;
	// 粒子分割数
	static inline const float kAngleUnit = std::numbers::pi_v<float> * 2 / kNumParticles;
	// flag
	bool isFinished_ = false;
	// 倒计时
	float counter_ = 0.0f;

	KamataEngine::ObjectColor objectColor_;
	KamataEngine::Vector4 color_;
};
