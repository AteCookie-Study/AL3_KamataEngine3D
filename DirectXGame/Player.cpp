#include "Player.h"		
#include <numbers>
#include "MyMath.h"


using namespace KamataEngine;
using namespace MathUtility;

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, Camera* camera, Vector3 position) { 
	assert(model || camera); 
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();\
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; 
}

void Player::Update() { 

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); 

	//移動入力
	//左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
	
		//左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			acceleration.x -= kAcceleration;
		}
		velocity_ += acceleration;
	}

	//移動
	worldTransform_.translation_ += velocity_;

}

void Player::Draw() {
	
	// モデルの描画
	model_->Draw(worldTransform_, *camera_);
}
