#pragma once
#include "KamataEngine.h"

///< summary>
/// 敵
/// </summary>
class Enemy {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 position);
	void Update();
	void Draw();
};
