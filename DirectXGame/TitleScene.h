#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "CameraController.h"
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

	float titlePosY_ = 1.0f; // 当前Y位置，初始中心1.0
	bool titleMoveUp_ = true; 

	

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
};
