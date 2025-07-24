#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Fade {

public:
	void Initialize();
	void Update();
	void Draw();

	KamataEngine::Sprite* sprite_ = nullptr;

	enum class Status {
		None,
		FadeIn,
		FadeOut,
	};

	Status status_ = Status::None;

	float duration_ = 0.0f;
	float counter_ = 0.0f;

	void Start(Status status, float duration);
	void Stop();

private:
	bool isFinished() const;
	
};