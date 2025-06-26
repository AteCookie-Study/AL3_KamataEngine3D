#define NOMINMAX
#include "Enemy.h"		
#include <numbers>
#include "MyMath.h"
#include <algorithm>
#include "MapChipField.h"


using namespace KamataEngine;
using namespace MathUtility;

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 position) {
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Enemy::Update() {

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();


}

void Enemy::Draw() {

	// モデルの描画
	model_->Draw(worldTransform_, *camera_);
}

void Enemy::AnimateTurn() {
	// 回旋制御
	{
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,       // 右向き
		    std::numbers::pi_v<float> * 3.0f / 2.0f // 左向き
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = destinationRotationY;
	}
}