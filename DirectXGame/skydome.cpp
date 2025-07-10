#include "skydome.h"

using namespace KamataEngine;

Skydome::Skydome() {}
Skydome::~Skydome() {}

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
}

void Skydome::Update() { worldTransform_.TransferMatrix(); }

void Skydome::Draw() { model_->Draw(worldTransform_, *camera_); }