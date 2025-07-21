#include "TitleScene.h"
#include <cmath>

using namespace KamataEngine;

TitleScene::TitleScene() {};
TitleScene ::~TitleScene(){ 
	delete titleModel_;
	delete playerModel_;

};

void TitleScene::Initialize() {
	titleModel_ = Model::CreateFromOBJ("title");
	playerModel_ = Model::CreateFromOBJ("player");
	// ワルドトランスフォームの初期化
	titleTransform_.Initialize();
	playerTransform_.Initialize();
	titleTransform_.translation_ = {5.0f, 5.0f, 5.0f}; 
	playerTransform_.translation_ = {5.0f, 5.0f, 5.0f}; 
    camera_.Initialize(); 

};

void TitleScene::Update() { 

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true; // 空格键触发时设置为完成状态
	}	

	const float speed = 0.01f;

	// 根据当前方向修改y坐标
	if (titleMoveUp_) {
		titlePosY_ += speed;
		if (titlePosY_ >= 1.3f) { // 到达上限，切换方向
			titlePosY_ = 1.3f;
			titleMoveUp_ = false;
		}
	} else {
		titlePosY_ -= speed;
		if (titlePosY_ <= 0.8f) { // 到达下限，切换方向
			titlePosY_ = 0.8f;
			titleMoveUp_ = true;
		}
	}
	titleTransform_.matWorld_ = MakeAffineMatrix(titleTransform_.scale_, titleTransform_.rotation_, titleTransform_.translation_);
	playerTransform_.matWorld_ = MakeAffineMatrix(playerTransform_.scale_, playerTransform_.rotation_, playerTransform_.translation_);
	titleTransform_.TransferMatrix();
	playerTransform_.TransferMatrix();
	playerTransform_.rotation_.y = 3.14159f; 
	titleTransform_.translation_ = {0.0f, titlePosY_, -45.0f}; 
	playerTransform_.translation_ = {0.0f, -0.8f, -45.0f}; 

}

void TitleScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	titleModel_->Draw(titleTransform_, camera_);
	playerModel_->Draw(playerTransform_, camera_);

	// 3Dモデル描画後処理
	Model::PostDraw();
}


