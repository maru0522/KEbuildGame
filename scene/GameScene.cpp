#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
    delete model_;
    delete debugCamera_;
    delete modelSkyDome_;
}

void GameScene::Initialize() {

    dxCommon_ = DirectXCommon::GetInstance();
    input_ = Input::GetInstance();
    audio_ = Audio::GetInstance();
    debugText_ = DebugText::GetInstance();
    model_ = Model::Create();
    //ファイル名を指定してテクスチャを読み込む
    textureHandle_ = TextureManager::Load("Task1_2Resources/player.png");

    //デバックカメラの生成
    debugCamera_ = new DebugCamera(1280, 720);

    viewProjection_.Initialize();

    //軸方向表示の有効化
    AxisIndicator::GetInstance()->SetVisible(true);
    //軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
    AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
    //ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
    PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

    // レールカメラの生成
    railCamera_ = std::make_unique<RailCamera>();
    Vector3 pos(0, 0, -50);
    Vector3 rad(0, 0, 0);
    // レールカメラの初期化
    railCamera_->Initialize(pos, rad);

    // 自キャラの生成
    player_ = std::make_unique<Player>();
    // 自キャラの初期化
    player_->Initialize(model_, textureHandle_);
    player_->SetParent(railCamera_->GetWorldTransform());

    // 3Dモデルの生成
    modelSkyDome_ = Model::CreateFromOBJ("skydome", true);

    // 天球の生成
    skydome_ = std::make_unique<Skydome>();
    // 天球の初期化
    skydome_->Initialize(modelSkyDome_);
    skydome_->SetParent(railCamera_->GetWorldTransform());
}

void GameScene::Update() {

    // 自キャラの更新
    player_->Update();

    //input_->IsTriggerMouse

    // 天球の更新
    skydome_->Update();
    // レールカメラの更新
    railCamera_->Update();

#ifdef _DEBUG
    if (input_->TriggerKey(DIK_C)) {
        if (isDebugCameraActive_) {
            isDebugCameraActive_ = false;
        }
        else {
            isDebugCameraActive_ = true;
        }
    }

    // カメラの処理
    if (isDebugCameraActive_) {
        //デバックカメラの更新
        debugCamera_->Update();
        debugViewProjection_ = debugCamera_->GetViewProjection();
        viewProjection_.matView = debugViewProjection_.matView;
        viewProjection_.matProjection = debugViewProjection_.matProjection;
    }
    else {
        viewProjection_.UpdateMatrix();
    }
#endif
}

void GameScene::Draw() {

    // コマンドリストの取得
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
    // 背景スプライト描画前処理
    Sprite::PreDraw(commandList);

    /// <summary>
    /// ここに背景スプライトの描画処理を追加できる
    /// </summary>

    // スプライト描画後処理
    Sprite::PostDraw();
    // 深度バッファクリア
    dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
    // 3Dオブジェクト描画前処理
    Model::PreDraw(commandList);

    /// <summary>
    /// ここに3Dオブジェクトの描画処理を追加できる
    /// </summary>
    ////3Dモデルの描画
    // model_->Draw(worldTransform_,viewProjection_,textureHandle_);
    ////モデルと連動させるカメラの描画
     //model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

    // 自キャラの描画
    player_->Draw(railCamera_->GetViewProjection());

    // 天球の描画
    skydome_->Draw(railCamera_->GetViewProjection());

    //ラインの描画

    // 3Dオブジェクト描画後処理
    Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
    // 前景スプライト描画前処理
    Sprite::PreDraw(commandList);

    /// <summary>
    /// ここに前景スプライトの描画処理を追加できる
    /// </summary>

    // デバッグテキストの描画
    debugText_->DrawAll(commandList);
    //
    // スプライト描画後処理
    Sprite::PostDraw();

#pragma endregion
}
