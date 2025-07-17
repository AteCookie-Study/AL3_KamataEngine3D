#pragma once
#include "CameraController.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "skydome.h"
#include "DeathParticles.h"
#include <vector>

// ゲームシーン
class GameScene {
private:
	KamataEngine::Input* input_ = nullptr;
	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Camera camera_;
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Model* enemyModel_ = nullptr;
	KamataEngine::Model* deathModel_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;

	enum class Phase {
		kPlay,
		kDeath,
	};

	Phase phase_;

	// Number of enemies
	static inline const float kEnemyNum = 1.0f;

	// 　サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;

	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;

	// ワルドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// Player
	Player* player_ = nullptr;
	Skydome* skydome_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有效
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// マップチップフィールドのワールドトランスフォーム
	void GenearteBlocks();

	// CameraController
	CameraController* cameraController_ = nullptr;

	// 敵
	// Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;

	void ChenckAllCollisions();

	void ChangePhase();

public:
	~GameScene();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
};
