#pragma once
#include "KamataEngine.h"

//<simmary>
//天球
// </summary>

class Skydome {

	public:
	    Skydome();
	    ~Skydome();
	    ///<summary>
	    ///初期话
	    /// </summary>
	    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

		///<summary>
	    /// 更新
	    ///	</summary>
	    void Update();

		///< summary>
	    /// 描画
	    /// </summary>
	    void Draw();

		private:
	     KamataEngine::WorldTransform worldTransform_;
		 KamataEngine::Model* model_ = nullptr;	
         KamataEngine::Camera* camera_ = nullptr;
	     uint32_t textureHandle_ = 0u;
};

