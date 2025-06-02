#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"

///< summary>
/// GameraController.h
/// </summary>
class Player;
class CameraController {
public:
	void Initialize();

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};
	const KamataEngine::Camera& GetViewProjection() const { return camera_; }
	
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};
	Rect movableArea_ = {0.0f, 100.0f, 0.0f, 100.0f};
	void SetMovavleArea(const Rect& area) { movableArea_ = area; }

     KamataEngine::Vector3 targetPosition_;
	static inline const float kInterpolationRate = 0.1f;

	static inline const float kVelocityBIas = 30.0f;
	
	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};

   private:
		//Camera
	    KamataEngine::Camera camera_;
	    Player* target_ = nullptr;

	   
};

