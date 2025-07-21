#include <Windows.h>
#include <KamataEngine.h>
#include "GameScene.h"
#include "TitleScene.h"
using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

enum class Scene {
	kUnknown = 0,

	kTitle = 1, 
	kGame = 2,

};

// 現在のシーン
Scene scene = Scene::kUnknown;

void ChangeScene();
void UpdateScene();
void DrawScene();


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	// エンジンの初期化
	KamataEngine::Initialize(L"GC2C_03_キョク_キンウ_AL3_3D");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();


	titleScene = new TitleScene;
	titleScene->Initialize();
	
	scene = Scene::kTitle; // 初期シーンをタイトルに設定
	

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

	    ChangeScene();

		UpdateScene();

	    dxCommon->PreDraw();

		DrawScene();
		
		//描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	// ゲームシーンの解放
	delete gameScene;
	// nullptrに代入
	gameScene = nullptr;
	titleScene = nullptr;

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}


void ChangeScene() {
	switch (scene) {
		case Scene::kTitle:
		if (titleScene->IsFinished()) {
			scene = Scene::kGame; //
			delete titleScene;  
			titleScene = nullptr;
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	    case Scene::kGame:
		    if (gameScene->IsFinished()) {
			    scene = Scene::kTitle; //
			    delete gameScene;
			    gameScene = nullptr;
			    titleScene = new TitleScene();
			    titleScene->Initialize();
		    }
		    break;
	}
};

void UpdateScene() {
	switch (scene) {
		case Scene::kTitle:
			titleScene->Update();
			break;
		case Scene::kGame:
			gameScene->Update();
			break;
	}
}

void DrawScene() {
	switch (scene) {
		case Scene::kTitle:
			titleScene->Draw();
			break;
		case Scene::kGame:
			gameScene->Draw();
			break;
	}
}

