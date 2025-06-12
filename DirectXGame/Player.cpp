#define NOMINMAX
#include "Player.h"		
#include <numbers>
#include "MyMath.h"
#include <algorithm>
#include "MapChipField.h"



using namespace KamataEngine;
using namespace MathUtility;

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, Camera* camera, Vector3 position) { 
	assert(model || camera); 
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; 
}

void Player::Update() {

	// 着地フラグ
	bool landing = false;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	//①移動入力 
	InputMove();

	//②移動量を加味して衝突判定する

	//  衝突情報を初期化
	CollisionMapInfo collisionMapInfo;

	//移动量に速度の値をコピー
	collisionMapInfo.move_ = velocity_;

	// マップ衝突判チェック
	CheckMapCollision(collisionMapInfo);

	//③判定結果を反映して移動させる
	//  移動
	worldTransform_.translation_ += collisionMapInfo.move_;

	//④天井に接触している場合の処理
		
	//⑤壁に接触している場合の処理

	//⑥接地状態の切り替え
	
	//  地面との当たり判定
	//  下降中？
	if (velocity_.y < 0) {
		// Y座標が地面以下なら接地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}
	if (onGround_) {
		// 空中状态に移行
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldTransform_.translation_.y = 1.0f; // 地面に着地
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
	//⑦旋回制御
	AnimateTurn();
	
	
}


void Player::Draw() {
	
	// モデルの描画
	model_->Draw(worldTransform_, *camera_);
}

void Player::InputMove() {

	// 移動入力
	// 接地状態
	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					// 右向きに変更
					lrDirection_ = LRDirection::kRight;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					// 左向きに変更
					lrDirection_ = LRDirection::kLeft;
				}
			}

			velocity_ += acceleration;
			// 速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 左右キーが押されていない場合は減速
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ

			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	} else { // 空中
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

}


void Player::AnimateTurn() {
	// 回旋制御
	{
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,       // 右向き
		    std::numbers::pi_v<float> * 3.0f / 2.0f // 左向き
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = destinationRotationY;
	}

}

KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) {
	
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, // kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, // kLeftTop
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}  // kLeftBottom
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}



void Player::CheckMapCollision(CollisionMapInfo& info) { 
	CheckMapCollisionUp(info);
    CheckMapCollisionDown(info);
	//CheckMapCollisionRight(info);
	//CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) { 
	if (info.move_.y < 0) {
		return;
	}
	std::array<KamataEngine::Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move_, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	bool hit = false;

	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + KamataEngine::Vector3(0,kHeight/2.0f,0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move_.y = std::max(0.0f, rect.bottom- worldTransform_.translation_.y-(kHeight/2.0f+kBlank));
		info.ceiling = true;
	}

}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) { 
	if (info.move_.y > 0) {
		return;
	}
	std::array<KamataEngine::Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move_, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	bool hit = false;
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + KamataEngine::Vector3(0, -kHeight / 2.0f, 0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move_.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		info.landing = true;
	}
	
}

void Player::CheckMapLanding(const CollisionMapInfo& info) { 
	if (onGround_) {
		// 着地処理
		if (info.landing) {

			onGround_ = true; // 着地状態にする
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
		}
		
		//ジャンプ開始
		if (velocity_.y > 0.0f) {

			onGround_ = false;
		}
	} else {	// 空中状態の処理
		
		MapChipType mapChipType;
		bool hit = false;
		IndexSet indexSet;
		std::array<KamataEngine::Vector3, 4> positionsNew;
		for (uint32_t i = 0; i < positionsNew.size(); ++i) {
			positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move_, static_cast<Corner>(i));
		}
		// 左下点の衝突判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		// 右下点の衝突判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]+Vector3(0,-kGroundSearchHeight,0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		if (!hit) {
			onGround_ = false;
		}

	}
}

void Player::MoveByCollisionResult(const CollisionMapInfo& info) { 
	worldTransform_.translation_ += info.move_; 
	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0.0f; 
	}

}

