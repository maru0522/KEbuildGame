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

enum GameSceneStats {
	Operate,
	Clear,
	Gameover
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

	// スプライトテクスチャハンドル
	uint32_t texTutorialMessageBackHandle_ = 0;
	std::array<uint32_t, 15> texTutorialMessages_ = {};
	uint32_t texSpriteW_ = 0;
	uint32_t texSpriteA_ = 0;
	uint32_t texSpriteS_ = 0;
	uint32_t texSpriteD_ = 0;
	uint32_t texSpriteWPush_ = 0;
	uint32_t texSpriteAPush_ = 0;
	uint32_t texSpriteSPush_ = 0;
	uint32_t texSpriteDPush_ = 0;
	uint32_t texSpriteWASDBack_ = 0;
	uint32_t texSpriteC_ = 0;
	uint32_t texSpriteCPush_ = 0;
	uint32_t texSpriteCameraRIPTA_ = 0;
	uint32_t texSpriteCameraMIWATASHI_ = 0;
	std::array<uint32_t, 10> texSpriteScores_ = {};
	uint32_t texSpriteStringRemain_ = 0;
	uint32_t texTutorialDisplayMemoryBlock_ = 0;
	uint32_t texTutorialDisplayGoalBlock_ = 0;

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
	uint32_t texCLEARHandle_ = 0;
	uint32_t texGAMEOVERHandle_ = 0;


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
	Model* CLEARModel_ = nullptr;
	Model* GAMEOVERModel_ = nullptr;


	// スプライト
	std::unique_ptr<Sprite> tutorialMessageBack_;
	std::array<std::unique_ptr<Sprite>, 15> tutorialMessages_;
	std::unique_ptr<Sprite> spriteW_;
	std::unique_ptr<Sprite> spriteA_;
	std::unique_ptr<Sprite> spriteS_;
	std::unique_ptr<Sprite> spriteD_;
	std::unique_ptr<Sprite> spriteWPush_;
	std::unique_ptr<Sprite> spriteAPush_;
	std::unique_ptr<Sprite> spriteSPush_;
	std::unique_ptr<Sprite> spriteDPush_;
	std::unique_ptr<Sprite> spriteWASDBack_;
	std::unique_ptr<Sprite> spriteC_;
	std::unique_ptr<Sprite> spriteCPush_;
	std::unique_ptr<Sprite> spriteCameraRIPTA_;
	std::unique_ptr<Sprite> spriteCameraMIWATASHI_;
	std::unique_ptr<Sprite> spriteCCameraBack_;
	std::array<std::unique_ptr<Sprite>, 3> spriteRemain_;
	/*特別*/
	std::array<uint32_t, 3> indexRemain_;
	std::unique_ptr<Sprite> spriteStringRemain_;
	std::unique_ptr<Sprite> spriteStringRemainBack_;
	std::unique_ptr<Sprite> tutorialDisplayMemoryBlock_;
	std::unique_ptr<Sprite> tutorialDisplayGoalBlock_;



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
	std::unique_ptr<Object> selectStageGear8_;
	std::array<std::unique_ptr<Object>, 4> stage1_4Gears8_;
	std::array<std::unique_ptr<Object>, 4> stage5_8Gears12_;
	std::array<std::unique_ptr<Object>, 2> stage9_10Gears16_;

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
	std::unique_ptr<Object> CLEAR_;
	std::unique_ptr<Object> GAMEOVER_;


	// 音
	uint32_t clickSound_ = 0;
	uint32_t moveCursorSound_ = 0;
	uint32_t clearSound_ = 0;
	uint32_t gameOverSound_ = 0;
	uint32_t BGM_ = 0;

	bool isFanfare_ = false;

	// 3Dモデル
	Model* modelSkyDome_ = nullptr;


	// カメラ
	uint32_t indexCamera_ = Rail;

	// ゲームシーン
	uint32_t gameScene_ = Title;
	// ゲーム中の状態
	uint32_t gameStats_ = Operate;

	// チュートリアルメッセージ番号
	uint32_t indexMessage_ = 0;

	// SelectStage番号
	uint32_t indexSelectStage = 1;

	// PlayGameステージ番号
	uint32_t indexPlayGameStage = 1;


	//uint32_t fpsCount4TutorialCam_ = 0;
	//Vector3 moveValue4TutorialCam_ = { 0,0,0 };
	//bool isFinishEasing4TutorialCam_ = false;

	// Select
	bool isPlayTutorial = true;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};