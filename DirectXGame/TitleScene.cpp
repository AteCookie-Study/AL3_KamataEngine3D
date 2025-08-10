#include "TitleScene.h"
#include <cmath>
#include <numbers>

using namespace KamataEngine;

TitleScene::TitleScene() {};
TitleScene ::~TitleScene(){ 
	delete titleModel_;
	delete playerModel_;
	delete bgModel_;
	delete fade_;

};

void TitleScene::Initialize() {
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, kFadeTime); 

	titleModel_ = Model::CreateFromOBJ("title");
	playerModel_ = Model::CreateFromOBJ("slime");
	bgModel_ = Model::CreateFromOBJ("BG");
	// ワルドトランスフォームの初期化
	titleTransform_.Initialize();
	playerTransform_.Initialize();
	bgTransform_.Initialize();
	titleTransform_.translation_ = {5.0f, 5.0f, 5.0f}; 
	playerTransform_.translation_ = {5.0f, 5.0f, 5.0f}; 
	playerTransform_.translation_ = {10.0f, 10.0f, 10.0f}; 
    camera_.Initialize(); 
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, kFadeTime);
};

void TitleScene::Update() {
	fade_->Update();

	switch (phase_) { case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, kFadeTime);
			phase_ = Phase::kFadeOut; 
			
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true; 
		}
		break;

	}
	//counter_ += 1.0f / 60.0f; // 假设每秒60帧
	//counter_ = std::fmod(counter_, 2.0f); // 保持在0到100之间
	//float angle = counter_ * 2.0f * std::numbers::pi_v<float>; // 计算角度


										 

	//if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	//	finished_ = true; // 空格键触发时设置为完成状态
	//}	

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
	bgTransform_.matWorld_ = MakeAffineMatrix(bgTransform_.scale_, bgTransform_.rotation_, bgTransform_.translation_);
	titleTransform_.TransferMatrix();
	playerTransform_.TransferMatrix();
	bgTransform_.TransferMatrix();
	playerTransform_.rotation_.y = 3.14159f; 
	titleTransform_.translation_ = {0.0f, titlePosY_, -45.0f}; 
	playerTransform_.translation_ = {0.0f, -0.8f, -45.0f}; 
	bgTransform_.translation_ = {0.0f, -0.80f, -40.0f}; 

}

void TitleScene::Draw() {
	

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	titleModel_->Draw(titleTransform_, camera_);
	playerModel_->Draw(playerTransform_, camera_);
	bgModel_->Draw(bgTransform_, camera_);
	fade_->Draw();

	// 3Dモデル描画後処理
	Model::PostDraw();
}


