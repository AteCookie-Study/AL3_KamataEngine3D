#define NOMINMAX
#include "Enemy.h"
#include "MapChipField.h"
#include "MyMath.h"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 position) {
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() {

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 移動
	worldTransform_.translation_ += velocity_;
	// X　アニメーション　スビート
	walkTimer_ += 1.0f / 20.0f;

	// 回转アニメーション
	float param = std::sin(walkTimer_);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd + (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = std::sin(degree);
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

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { (void)player; }
