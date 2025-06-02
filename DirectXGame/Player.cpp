#define NOMINMAX
#include "Player.h"		
#include <numbers>
#include "MyMath.h"
#include <algorithm>



using namespace KamataEngine;
using namespace MathUtility;

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, Camera* camera, Vector3 position) { 
	assert(model || camera); 
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; 
}

void Player::Update() {

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 移動入力
	// 接地状態
	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					// 右向きに変更
					lrDirection_ = LRDirection::kRight;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					// 左向きに変更
					lrDirection_ = LRDirection::kLeft;
				}
			}

			velocity_ += acceleration;
			// 速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 左右キーが押されていない場合は減速
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ

			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	} else { // 空中
			// 落下速度
			velocity_ += Vector3(0, -kGravityAcceleration, 0);
			// 落下速度制限
			velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

		

		// 接地判定
		bool landing = false;

		// 地面との当たり判定
		// 下降中？
		if (velocity_.y < 0) {
			// Y座標が地面以下なら接地
			if (worldTransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		}
		if (onGround_) {
			// 空中状态に移行
			if (velocity_.y > 0.0f) {
				onGround_ = false;
			}
		} else {
			if (landing) {
				worldTransform_.translation_.y = 1.0f; // 地面に着地
				velocity_.x *= (1.0f - kAttenuation);
				velocity_.y = 0.0f;
				onGround_ = true;
			}
		}
		// 回旋制御
		{
			float destinationRotationYTable[] = {
			    std::numbers::pi_v<float> / 2.0f,       // 右向き
			    std::numbers::pi_v<float> * 3.0f / 2.0f // 左向き
			};
			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			worldTransform_.rotation_.y = destinationRotationY;
		}

		// 移動
		worldTransform_.translation_ += velocity_;
	
}


void Player::Draw() {
	
	// モデルの描画
	model_->Draw(worldTransform_, *camera_);
}
