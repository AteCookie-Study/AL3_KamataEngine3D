#include "CameraController.h"
#include "Player.h"

using namespace KamataEngine;
using namespace MathUtility;



void CameraController::Initialize() { 
	camera_.Initialize();

}

void CameraController::Update() {
	const KamataEngine::Vector3& targetVelocity = target_->GetVelocity();
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform(); 
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBIas;

	camera_.translation_.x = Lerp(camera_.translation_.x, targetPosition_.x, kInterpolationRate);

	camera_.translation_.x = max(camera_.translation_.x, movableArea_.left);
	camera_.translation_.x = min(camera_.translation_.x, movableArea_.right);
	camera_.translation_.y = max(camera_.translation_.y, movableArea_.top);
	camera_.translation_.y = min(camera_.translation_.y, movableArea_.bottom);

    camera_.UpdateMatrix();

	camera_.translation_.x = max(camera_.translation_.x, targetPosition_.x + targetMargin.left);
	camera_.translation_.x = min(camera_.translation_.x, targetPosition_.x + targetMargin.right);
	camera_.translation_.y = max(camera_.translation_.y, targetPosition_.y + targetMargin.bottom);
	camera_.translation_.y = min(camera_.translation_.y, targetPosition_.y + targetMargin.top);
	
}

void CameraController::Reset() { 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}