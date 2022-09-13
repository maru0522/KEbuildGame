#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "math.h"
#include "Player.h"
#include <memory>
#include "Enemy.h"
#include "SkyDome.h"
#include "RailCamera.h"
#include <sstream>
#include "Stage.h"
#include "Object.h"
#include "FPSCamera.h"


enum Scene {
	Title,			// 0
	GuidQuestion,	// 1
	Tutorial,		// 2
	SelectStage,	// 3
	PlayGame		// 4
};

enum Camera {
	Rail,
	FPS
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	void PlayerMove();
	void SearchUnderfootBlock();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	Stage* stage_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t testPlayerHandle_ = 0;
	uint32_t testPlayerHandle2_ = 0;
	uint32_t testBlockHandle_ = 0;
	uint32_t texArrowHandle_ = 0;
	uint32_t texGear8Handle_ = 0;
	uint32_t texGear12Handle_ = 0;
	uint32_t texGear16Handle_ = 0;

	// 文字テクスチャハンドル
	uint32_t texRIPTAHandle_ = 0;
	uint32_t texPRESSSPACEHandle_ = 0;
	uint32_t texYESHandle_ = 0;
	uint32_t texNOHandle_ = 0;
	uint32_t texPLAYTUTORIALHandle_ = 0;
	uint32_t texWASDHandle_ = 0;


	ViewProjection viewProjection_;
	ViewProjection debugViewProjection_;

	// playerModel
	Model* pModel_ = nullptr;
	// arrowModel
	Model* arrowModel_ = nullptr;
	// Gear_8Model
	Model* gear8Model_ = nullptr;
	// Gear_12Model
	Model* gear12Model_ = nullptr;
	// Gear_16Model
	Model* gear16Model_ = nullptr;

	// 文字Model
	Model* RIPTAModel_ = nullptr;
	Model* PRESSSPACEModel_ = nullptr;
	Model* YESModel_ = nullptr;
	Model* NOModel_ = nullptr;
	Model* PLAYTUTORIALModel_ = nullptr;
	Model* WModel_ = nullptr;
	Model* SModel_ = nullptr;
	Model* AModel_ = nullptr;
	Model* DModel_ = nullptr;


	//
	Sprite* sprite_ = nullptr;


	std::unique_ptr<Player> player_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<RailCamera> railCamera_;
	std::unique_ptr<FPSCamera> fpsCamera_;

	// オブジェクト
	std::unique_ptr<Object> arrowUp_;
	std::unique_ptr<Object> arrowDown_;
	std::unique_ptr<Object> gear8_;
	std::unique_ptr<Object> gear12_;
	std::unique_ptr<Object> gear16_;

	// 文字
	std::unique_ptr<Object> RIPTA_;
	std::unique_ptr<Object> RIPTAGear16L_;
	std::unique_ptr<Object> RIPTAGear16R_;
	std::unique_ptr<Object> PRESSSPACE_;
	std::unique_ptr<Object> YES_;
	std::unique_ptr<Object> NO_;
	std::unique_ptr<Object> YESGear12_;
	std::unique_ptr<Object> NOGear12_;
	std::unique_ptr<Object> PLAYTUTORIAL_;
	std::unique_ptr<Object> W_;
	std::unique_ptr<Object> S_;
	std::unique_ptr<Object> A_;
	std::unique_ptr<Object> D_;


	// 3Dモデル
	Model* modelSkyDome_ = nullptr;


	// カメラ
	uint32_t indexCamera_ = Rail;

	// 足元のブロック
	uint32_t underfootBlock_ = 0;

	// ゲームシーン
	uint32_t gameScene_ = 0;

	// Select
	bool isPlayTutorial = true;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};