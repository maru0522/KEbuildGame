﻿#include "GameScene.h"
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
    stage_ = GetInstanceStage();

    //ファイル名を指定してテクスチャを読み込む
    textureHandle_ = TextureManager::Load("Task1_2Resources/player.png");
    testBlockHandle_ = TextureManager::Load("Task1_2Resources/bullet.png");

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
    Vector3 pos(100, 100, 100);
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
    //skydome_->SetParent(railCamera_->GetWorldTransform());
    
    stage_->InitStage(model_, textureHandle_);
    stage_->LoadStage("Resources/Task1_2Resources/tutorial.csv");
}

void GameScene::Update() {

    // レールカメラの更新
    railCamera_->Update();

    // 自機更新 *座標更新は->RailCamera->GameSceneの順に委託している。
    player_->Update();
    PlayerMove();

    stage_->UpdateStage();

    //input_->IsTriggerMouse

    // 天球の更新
    skydome_->Update();

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
    //player_->Draw(debugCamera_->GetViewProjection());

    // 天球の描画
    //skydome_->Draw(railCamera_->GetViewProjection());

    stage_->DrawStage(railCamera_->GetViewProjection());

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

void GameScene::PlayerMove()
{
    // 移動座標宣言
    Vector3 pos = { 0,0,0 };

    nSquarePos_.x = static_cast<int32_t>(railCamera_->GetWorldPosition().x / stage_->blockSideLength_);
    nSquarePos_.y = static_cast<int32_t>(railCamera_->GetWorldPosition().y / stage_->blockSideLength_);
    nSquarePos_.z = static_cast<int32_t>(railCamera_->GetWorldPosition().z / stage_->blockSideLength_);

#pragma region デフォルト
    // if() *プレイヤーのrotateが{0,0,0}の時
    if (input_->TriggerKey(DIK_W)) {
        if (nSquarePos_.y > 1 && nSquarePos_.z < stage_->squareLengthZ_ - 2) { // マス目単位：y > 1 & z < (200 - 2) 
            if (stage_->isFillCubes_[nSquarePos_.z + 1][nSquarePos_.y][nSquarePos_.x] == false &&
                stage_->isFillCubes_[nSquarePos_.z + 1][nSquarePos_.y - 1][nSquarePos_.x] == true) { // 正面のマスにブロックがない & 正面のマスの1マス下にブロックがある（床がある）

                // 1マス進む
                pos = {
                    nSquarePos_.x * stage_->blockSideLength_,
                    nSquarePos_.y * stage_->blockSideLength_,
                    (nSquarePos_.z + 1) * stage_->blockSideLength_
                };
                railCamera_->SetWorldPosition(pos);
            }
            else if (stage_->isFillCubes_[nSquarePos_.z + 1][nSquarePos_.y][nSquarePos_.x] == true) { // 正面のマスにブロックがある

                // 座標は変わらない
                // TODO: 床に対して向きが変わるようにする
                // guess: rotation { CTR(90),0,0}
            }
            else if (stage_->isFillCubes_[nSquarePos_.z + 1][nSquarePos_.y][nSquarePos_.x] == false &&
                     stage_->isFillCubes_[nSquarePos_.z + 1][nSquarePos_.y - 1][nSquarePos_.x] == false) { // 正面のマスにブロックがない & 正面のマスの1マス下にもブロックがない（床がない）

                // 正面斜め下のマスに進む（足元の同じブロックの違う面の上に立つイメージ）
                pos = {
                    nSquarePos_.x * stage_->blockSideLength_,
                    (nSquarePos_.y - 1) * stage_->blockSideLength_,
                    (nSquarePos_.z + 1) * stage_->blockSideLength_
                };
                railCamera_->SetWorldPosition(pos);

                // TODO: 床に対して向きが変わるようにする
                // guess: rotation { CTR(-90),0,0}
            }
        }
    }
    if (input_->TriggerKey(DIK_S)) {
        if (nSquarePos_.y > 1 && nSquarePos_.z > 1) { // マス目単位：y > 1 & z > 1 
            if (stage_->isFillCubes_[nSquarePos_.z - 1][nSquarePos_.y][nSquarePos_.x] == false &&
                stage_->isFillCubes_[nSquarePos_.z - 1][nSquarePos_.y - 1][nSquarePos_.x] == true) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にブロックがある（床がある）

                // 1マス下がる
                pos = {
                    nSquarePos_.x * stage_->blockSideLength_,
                    nSquarePos_.y * stage_->blockSideLength_,
                    (nSquarePos_.z - 1) * stage_->blockSideLength_
                };
                railCamera_->SetWorldPosition(pos);
            }
            else if (stage_->isFillCubes_[nSquarePos_.z - 1][nSquarePos_.y][nSquarePos_.x] == true) { // 真後ろのマスにブロックがある

                // 座標は変わらない
                // TODO: 床に対して向きが変わるようにする
                // guess: rotation { CTR(-90),0,0 }
            }
            else if (stage_->isFillCubes_[nSquarePos_.z - 1][nSquarePos_.y][nSquarePos_.x] == false &&
                     stage_->isFillCubes_[nSquarePos_.z - 1][nSquarePos_.y - 1][nSquarePos_.x] == false) { // 真後ろのマスにブロックがない & 真後ろのマスの1マス下にもブロックがない（床がない）

                // 後ろ斜め下のマスに下がる（足元の同じブロックの違う面の上に立つイメージ）
                pos = {
                    nSquarePos_.x * stage_->blockSideLength_,
                    (nSquarePos_.y - 1) * stage_->blockSideLength_,
                    (nSquarePos_.z - 1) * stage_->blockSideLength_
                };
                railCamera_->SetWorldPosition(pos);

                // TODO: 床に対して向きが変わるようにする
                // guess: rotation { CTR(90),0,0}
            }
        }
    }
    if (input_->TriggerKey(DIK_A)) {
        if (nSquarePos_.x > 1 && nSquarePos_.y > 1) { // マス目単位：x > 1 & y > 1 
            if (stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y][nSquarePos_.x - 1] == false &&
                stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y - 1][nSquarePos_.x - 1] == true) { // 左のマスにブロックがない & 左のマスの1マス下にブロックがある（床がある）

                // 1マス左に動く
                pos = {
                    (nSquarePos_.x - 1) * stage_->blockSideLength_,
                    nSquarePos_.y * stage_->blockSideLength_,
                    nSquarePos_.z * stage_->blockSideLength_
                };
                railCamera_->SetWorldPosition(pos);
            }
            else if (stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y][nSquarePos_.x - 1] == true) { // 左のマスにブロックがある

                // 座標は変わらない
                // TODO: 床に対して向きが変わるようにする
                // guess: rotation { 0,0,CTR(90) }
            }
            else if (stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y][nSquarePos_.x - 1] == false &&
                     stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y - 1][nSquarePos_.x - 1] == false) { // 左のマスにブロックがない & 左のマスの1マス下にもブロックがない（床がない）

                // 左下のマスに下がる（足元の同じブロックの違う面の上に立つイメージ）
                pos = {
                    (nSquarePos_.x - 1) * stage_->blockSideLength_,
                    (nSquarePos_.y - 1) * stage_->blockSideLength_,
                    nSquarePos_.z * stage_->blockSideLength_
                };
                railCamera_->SetWorldPosition(pos);

                // TODO: 床に対して向きが変わるようにする
                // guess: rotation { 0,0,CTR(-90)}
            }
        }
    }
    if (input_->TriggerKey(DIK_D)) {
        if (nSquarePos_.x < (stage_->squareLengthX_ - 2) && nSquarePos_.y > 1) { // マス目単位：x < (200 - 2) & y > 1 
            if (stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y][nSquarePos_.x + 1] == false &&
                stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y - 1][nSquarePos_.x + 1] == true) { // 右のマスにブロックがない & 右のマスの1マス下にブロックがある（床がある）

                // 1マス右に動く
                pos = {
                    (nSquarePos_.x + 1) * stage_->blockSideLength_,
                    nSquarePos_.y * stage_->blockSideLength_,
                    nSquarePos_.z * stage_->blockSideLength_
                };
                railCamera_->SetWorldPosition(pos);
            }
            else if (stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y][nSquarePos_.x + 1] == true) { // 右のマスにブロックがある

                // 座標は変わらない
                // TODO: 床に対して向きが変わるようにする
                // guess: rotation { 0,0,CTR(-90) }
            }
            else if (stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y][nSquarePos_.x + 1] == false &&
                     stage_->isFillCubes_[nSquarePos_.z][nSquarePos_.y - 1][nSquarePos_.x + 1] == false) { // 右のマスにブロックがない & 右のマスの1マス下にもブロックがない（床がない）

                // 左下のマスに下がる（足元の同じブロックの違う面の上に立つイメージ）
                pos = {
                    (nSquarePos_.x + 1) * stage_->blockSideLength_,
                    (nSquarePos_.y - 1) * stage_->blockSideLength_,
                    nSquarePos_.z * stage_->blockSideLength_
                };
                railCamera_->SetWorldPosition(pos);

                // TODO: 床に対して向きが変わるようにする
                // guess: rotation { 0,0,CTR(-90)}
            }
        }
    }
#pragma endregion

#ifdef _DEBUG
    if (input_->TriggerKey(DIK_SPACE)) {
        pos = {
            nSquarePos_.x * stage_->blockSideLength_,
            (nSquarePos_.y + 1) * stage_->blockSideLength_,
            nSquarePos_.z * stage_->blockSideLength_
        };
        railCamera_->SetWorldPosition(pos);
    }
    if (input_->TriggerKey(DIK_LSHIFT)) {
        pos = {
            nSquarePos_.x * stage_->blockSideLength_,
            (nSquarePos_.y - 1) * stage_->blockSideLength_,
            nSquarePos_.z * stage_->blockSideLength_
        };
        railCamera_->SetWorldPosition(pos);
    }
#endif
}
