#include "Player.h"		
#include <numbers>


using namespace KamataEngine;

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

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw() {
	
	// モデルの描画
	model_->Draw(worldTransform_, *camera_);
}
