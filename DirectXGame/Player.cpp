#include "Player.h"		


using namespace KamataEngine;

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHanle, Camera* camera) { 
	assert(model || camera); 
	model_ = model;
	camera_ = camera;
	textureHandle_ = textureHanle;
	worldTransform_.Initialize();
}

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw() {
	
	// モデルの描画
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}
