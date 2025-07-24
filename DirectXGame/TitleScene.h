#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "CameraController.h"
#include "Fade.h"
#include <vector>


/// <summary>
/// TitleScene.h
/// </summary>

class TitleScene{

private:
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;

	// ワルドトランスフォーム
	KamataEngine::WorldTransform titleTransform_; 
	KamataEngine::WorldTransform playerTransform_; 
	KamataEngine::Camera camera_; 

	float titlePosY_ = 1.0f; 
	bool titleMoveUp_ = true; 

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	Phase phase_ = Phase::kFadeIn; 
	
	static constexpr float kFadeTime = 1.0f;

public:
	TitleScene();
	~TitleScene();

	void TitleMove();
		

	void Initialize();
	//void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw();

	bool finished_ = false;
	bool IsFinished() const { return finished_; }

	Fade* fade_ = nullptr;

};
