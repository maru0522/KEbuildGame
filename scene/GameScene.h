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

	void PopEnemy(Vector3& pos);

	// 衝突判定と応答
	void CheckAllCollision();

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t reticleHandle_ = 0;

	ViewProjection viewProjection_;
	ViewProjection debugViewProjection_;

	//
	Sprite* sprite_ = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPosCommands_;
	bool isWaitEnemyPop_ = false;
	uint32_t waitTime_ = 0;


	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<RailCamera> railCamera_;

	// 3Dモデル
	Model* modelSkyDome_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};