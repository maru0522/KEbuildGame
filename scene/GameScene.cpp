#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
#include "Calc.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
    delete model_;
    delete debugCamera_;
    delete modelSkyDome_;
    delete pModel_;
    delete arrowModel_;
    delete gear8Model_;
    delete gear12Model_;
    delete gear16Model_;

    // 文字
    delete RIPTAModel_;
    delete PRESSSPACEModel_;
    delete YESModel_;
    delete NOModel_;
    delete PLAYTUTORIALModel_;
    delete WModel_;
}

void GameScene::Initialize() {

    dxCommon_ = DirectXCommon::GetInstance();
    input_ = Input::GetInstance();
    audio_ = Audio::GetInstance();
    debugText_ = DebugText::GetInstance();
    model_ = Model::Create();
    stage_ = GetInstanceStage();

    // playerModel
    pModel_ = Model::CreateFromOBJ("player");
    // arrow
    arrowModel_ = Model::CreateFromOBJ("arrow");
    // gear8
    gear8Model_ = Model::CreateFromOBJ("Gear");
    // gear12
    gear12Model_ = Model::CreateFromOBJ("Gear_12");
    // gear16
    gear16Model_ = Model::CreateFromOBJ("Gear_16");

    // 文字Model
    RIPTAModel_ = Model::CreateFromOBJ("RIPTA");
    PRESSSPACEModel_ = Model::CreateFromOBJ("PRESSSPACE");
    YESModel_ = Model::CreateFromOBJ("YES");
    NOModel_ = Model::CreateFromOBJ("NO");
    PLAYTUTORIALModel_ = Model::CreateFromOBJ("PLAYTUTORIAL");
    WModel_ = Model::CreateFromOBJ("W");
    SModel_ = Model::CreateFromOBJ("S");
    AModel_ = Model::CreateFromOBJ("A");
    DModel_ = Model::CreateFromOBJ("D");



    //ファイル名を指定してテクスチャを読み込む
    textureHandle_ = TextureManager::Load("Task1_2Resources/player.png");
    testPlayerHandle_ = TextureManager::Load("player/Player_BakeTexture_Green.png");
    testPlayerHandle2_ = TextureManager::Load("player/Player_BakeTexture_Red.png");
    testBlockHandle_ = TextureManager::Load("Task1_2Resources/bullet.png");
    texArrowHandle_ = TextureManager::Load("W/WASD.png");
    texGear8Handle_ = TextureManager::Load("Gear/Gear.png");
    texGear12Handle_ = TextureManager::Load("Gear_12/Gear_12.png");
    texGear16Handle_ = TextureManager::Load("Gear_16/Gear_16.png");

    // 文字テクスチャハンドル
    texRIPTAHandle_ = TextureManager::Load("RIPTA/RIPTA.png");
    texPRESSSPACEHandle_ = TextureManager::Load("PRESSSPACE/PRESSSPACE.png");
    texYESHandle_ = TextureManager::Load("YES/YES.png");
    texNOHandle_ = TextureManager::Load("NO/NO.png");
    texPLAYTUTORIALHandle_ = TextureManager::Load("PLAYTUTORIAL/PLAYTUTORIAL.png");
    texWASDHandle_ = TextureManager::Load("W/WASD.png");


    //デバックカメラの生成
    debugCamera_ = new DebugCamera(1280, 720);

    viewProjection_.Initialize();

    //軸方向表示の有効化
    AxisIndicator::GetInstance()->SetVisible(true);
    //軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
    AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
    //ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
    PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

    // 自キャラの生成
    player_ = std::make_unique<Player>();
    // 自キャラの初期化
    player_->Initialize(pModel_, testPlayerHandle_, testPlayerHandle2_);
    player_->SetActionRemain(100);
    //player_->SetParent(railCamera_->GetWorldTransform());

    // レールカメラの生成
    railCamera_ = std::make_unique<RailCamera>();
    Vector3 pos(90, 90, 90);
    Vector3 rad(0, Calc::ConvertToRadian(0), 0);
    // レールカメラの初期化
    railCamera_->Initialize(pos, rad);

    // fpsカメラの生成
    fpsCamera_ = std::make_unique<FPSCamera>();
    // fpsカメラの初期化
    fpsCamera_->Initialize(player_->GetWorldTransform().translation_, player_->GetWorldTransform().rotation_);

    

    // 3Dモデルの生成
    modelSkyDome_ = Model::CreateFromOBJ("skydome", true);

    // 天球の生成
    skydome_ = std::make_unique<Skydome>();
    // 天球の初期化
    skydome_->Initialize(modelSkyDome_);
    //skydome_->SetParent(railCamera_->GetWorldTransform());

    // オブジェクトたちの生成
    arrowUp_ = std::make_unique<Object>();
    arrowDown_ = std::make_unique<Object>();
    gear8_ = std::make_unique<Object>();
    gear12_ = std::make_unique<Object>();
    gear16_ = std::make_unique<Object>();
    // オブジェクトたちの初期化
    arrowUp_->Initialize(arrowModel_, texArrowHandle_, { 94.4f, 89.2f, 110 }, { Calc::ConvertToRadian(-90),Calc::ConvertToRadian(0),0 });
    arrowUp_->SetScale({ 0.7,0.7,0.7 });
    arrowDown_->Initialize(arrowModel_, texArrowHandle_, { 94.4f, 87.7f, 110 }, { Calc::ConvertToRadian(90),Calc::ConvertToRadian(0),0 });
    arrowDown_->SetScale({ 0.7,0.7,0.7 });
    gear8_->Initialize(gear8Model_, texGear8Handle_, { 86, 90, 110 }, { 0,Calc::ConvertToRadian(90),0 });
    gear8_->SetIsRolling(true);
    gear8_->SetIsLeftRoll(true);
    gear12_->Initialize(gear12Model_, texGear12Handle_, { 0, 0, 0 }, { 0,Calc::ConvertToRadian(90),0 });
    gear12_->SetIsRolling(true);
    gear16_->Initialize(gear16Model_, texGear16Handle_, { 0,0, 0 }, { 0,Calc::ConvertToRadian(90),0 });
    gear16_->SetIsRolling(true);

    // 文字オブジェクト生成
    RIPTA_ = std::make_unique<Object>();
    RIPTAGear16L_ = std::make_unique<Object>();
    RIPTAGear16R_ = std::make_unique<Object>();
    PRESSSPACE_ = std::make_unique<Object>();
    YES_ = std::make_unique<Object>();
    NO_ = std::make_unique<Object>();
    YESGear12_ = std::make_unique<Object>();
    NOGear12_ = std::make_unique<Object>();
    PLAYTUTORIAL_ = std::make_unique<Object>();
    W_ = std::make_unique<Object>();
    S_ = std::make_unique<Object>();
    A_ = std::make_unique<Object>();
    D_ = std::make_unique<Object>();
    // 文字オブジェクト初期化
    RIPTA_->Initialize(RIPTAModel_, texRIPTAHandle_, { 90,94,110 }, { 0,Calc::ConvertToRadian(90),0 });
    RIPTAGear16L_->Initialize(gear16Model_, texGear16Handle_, { 84,94,110 }, { 0,Calc::ConvertToRadian(90),0 });
    RIPTAGear16L_->SetIsRolling(true);
    RIPTAGear16R_->Initialize(gear16Model_, texGear16Handle_, { 95.8f,94,110 }, { 0,Calc::ConvertToRadian(90),0 });
    RIPTAGear16R_->SetIsRolling(true);
    PRESSSPACE_->Initialize(PRESSSPACEModel_, texPRESSSPACEHandle_, { 90,88,110 }, { 0,Calc::ConvertToRadian(90),0 });
    PRESSSPACE_->SetScale({ 0.6,0.6,0.6 });
    YES_->Initialize(YESModel_, texYESHandle_, { 90,90,110 }, { 0,Calc::ConvertToRadian(90),0 });
    YES_->SetScale({ 0.7,0.7,0.7 });
    NO_->Initialize(NOModel_, texNOHandle_, { 90, 87, 110 }, { 0,Calc::ConvertToRadian(90),0 });
    NO_->SetScale({ 0.7,0.7,0.7 });
    YESGear12_->Initialize(gear12Model_, texGear12Handle_, { 87,91,113 }, { 0,Calc::ConvertToRadian(90),0 });
    YESGear12_->SetIsRolling(true);
    NOGear12_->Initialize(gear12Model_, texGear12Handle_, { 87.2f, 87.5f, 113 }, { 0,Calc::ConvertToRadian(90),0 });
    NOGear12_->SetIsRolling(true);
    PLAYTUTORIAL_->Initialize(PLAYTUTORIALModel_, texPLAYTUTORIALHandle_, { 91,98,135 }, { 0,Calc::ConvertToRadian(90),0 });
    PLAYTUTORIAL_->SetScale({ 1.5,1.5,1.5 });
    W_->Initialize(WModel_, texWASDHandle_, { 95,91,113 }, { 0,Calc::ConvertToRadian(90),0 });
    W_->SetScale({ 0.7,0.7,0.7 });
    S_->Initialize(SModel_, texWASDHandle_, { 95,85.5f,113 }, { 0,Calc::ConvertToRadian(90),0 });
    S_->SetScale({ 0.7,0.7,0.7 });
    A_->Initialize(WModel_, texWASDHandle_, { 95,91,113 }, { 0,Calc::ConvertToRadian(90),0 });
    D_->Initialize(WModel_, texWASDHandle_, { 95,91,113 }, { 0,Calc::ConvertToRadian(90),0 });



    stage_->InitStage(model_, textureHandle_);
    stage_->LoadStage("Resources/Task1_2Resources/debugRoom.csv");
}

void GameScene::Update() {

    if (gameScene_ == Title) {
#pragma region キーボード操作
        if (input_->TriggerKey(DIK_SPACE)) {
            gameScene_ = GuidQuestion;
        }
#pragma endregion
#pragma region オブジェクト更新
        // レールカメラの更新
        railCamera_->Update();
        // 文字
        RIPTA_->Update();
        RIPTAGear16L_->Update();
        RIPTAGear16R_->Update();
        PRESSSPACE_->Update();
#pragma endregion
    }
    else if (gameScene_ == GuidQuestion) {
#pragma region キーボード操作
        if (input_->TriggerKey(DIK_W)) {
            isPlayTutorial = true;
            gear8_->SetPos({ 86, 90, 110 });
        }
        if (input_->TriggerKey(DIK_S)) {
            isPlayTutorial = false;
            gear8_->SetPos({ 86, 87, 110 });
        }

        if (input_->TriggerKey(DIK_SPACE)) {
            if (isPlayTutorial == true) {
                gameScene_ = Tutorial;
                stage_->Reset();
                stage_->LoadStage("Resources/Task1_2Resources/tutorial.csv");
                player_->SetPos({ 92,96,92 });
            }
            else {
                gameScene_ = SelectStage;
                stage_->Reset();
            }
        }
#pragma endregion
#pragma region オブジェクト更新
        // レールカメラの更新
        railCamera_->Update();
        arrowUp_->Update();
        arrowDown_->Update();
        // オブジェクト
        gear8_->Update();
        if (isPlayTutorial) {
            YESGear12_->Update();
        }
        else {
            NOGear12_->Update();
        }
        // 文字
        YES_->Update();
        NO_->Update();
        PLAYTUTORIAL_->Update();
        W_->Update();
        S_->Update();
#pragma endregion
    }
    else if (gameScene_ == Tutorial) {
#pragma region キーボード操作
        if (input_->TriggerKey(DIK_C)) {
            if (indexCamera_ == Rail) {
                indexCamera_ = FPS;
            }
            else if (indexCamera_ == FPS) {
                indexCamera_ = Rail;
            }
        }
#pragma endregion
        // カメラの更新
        if (indexCamera_ == Rail) {
            railCamera_->Update();
        }
        else if (indexCamera_ == FPS) {
            fpsCamera_->Update(player_->GetWorldTransform());
        }
        

        // 自機更新 
        player_->Update();

        stage_->UpdateStage();

        //input_->IsTriggerMouse

        // 天球の更新
        skydome_->Update();

        if (player_->GetUnderBlockId() == GOAL) {
            gameScene_ = SelectStage;
        }
    }
    else if (gameScene_ == SelectStage) {
#pragma region キーボード操作
        if (input_->TriggerKey(DIK_A)) {
        }
        if (input_->TriggerKey(DIK_D)) {
        }

        if (input_->TriggerKey(DIK_SPACE)) {

        }
#pragma endregion
    }
    else if (gameScene_ == PlayGame) {
        // レールカメラの更新
        railCamera_->Update();

        // 自機更新 *座標更新は->RailCamera->GameSceneの順に委託している。
        player_->Update();

        stage_->UpdateStage();

        //input_->IsTriggerMouse

        // 天球の更新
        skydome_->Update();
    }

#ifdef _DEBUG
    //if (input_->TriggerKey(DIK_C)) {
    //    if (isDebugCameraActive_) {
    //        isDebugCameraActive_ = false;
    //    }
    //    else {
    //        isDebugCameraActive_ = true;
    //    }
    //}

    //// カメラの処理
    //if (isDebugCameraActive_) {
    //    //デバックカメラの更新
    //    debugCamera_->Update();
    //    debugViewProjection_ = debugCamera_->GetViewProjection();
    //    viewProjection_.matView = debugViewProjection_.matView;
    //    viewProjection_.matProjection = debugViewProjection_.matProjection;
    //}
    //else {
    //    viewProjection_.UpdateMatrix();
    //}

    debugText_->SetPos(600, 400);
    debugText_->Printf("gameScene_:%d", gameScene_);
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

    if (gameScene_ == Title) {
        RIPTA_->Draw(railCamera_->GetViewProjection(), false);
        RIPTAGear16L_->Draw(railCamera_->GetViewProjection(), false);
        RIPTAGear16R_->Draw(railCamera_->GetViewProjection(), false);
        PRESSSPACE_->Draw(railCamera_->GetViewProjection(), false);
    }
    else if (gameScene_ == GuidQuestion) {
        arrowUp_->Draw(railCamera_->GetViewProjection(), false);
        arrowDown_->Draw(railCamera_->GetViewProjection(), false);
        gear8_->Draw(railCamera_->GetViewProjection(), false);
        gear16_->Draw(railCamera_->GetViewProjection(), false);
        // 文字
        YES_->Draw(railCamera_->GetViewProjection(), false);
        NO_->Draw(railCamera_->GetViewProjection(), false);
        YESGear12_->Draw(railCamera_->GetViewProjection(), false);
        NOGear12_->Draw(railCamera_->GetViewProjection(), false);
        PLAYTUTORIAL_->Draw(railCamera_->GetViewProjection(), false);
        W_->Draw(railCamera_->GetViewProjection(), false);
        S_->Draw(railCamera_->GetViewProjection(), false);
    }
    else if (gameScene_ == Tutorial) {
        if (indexCamera_ == Rail) {
            // 自キャラの描画
            player_->Draw(railCamera_->GetViewProjection());
            //player_->Draw(debugCamera_->GetViewProjection());

            // 天球の描画
            //skydome_->Draw(railCamera_->GetViewProjection());

            stage_->DrawStage(railCamera_->GetViewProjection());
        }
        else if (indexCamera_ == FPS) {
            // 自キャラの描画
            /*player_->Draw(fpsCamera_->GetViewProjection());*/
            //player_->Draw(debugCamera_->GetViewProjection());

            // 天球の描画
            //skydome_->Draw(railCamera_->GetViewProjection());

            stage_->DrawStage(fpsCamera_->GetViewProjection());
        }
    }
    else if (gameScene_ == SelectStage) {

    }
    else if (gameScene_ == PlayGame) {
        // 自キャラの描画
        player_->Draw(railCamera_->GetViewProjection());
        //player_->Draw(debugCamera_->GetViewProjection());

        // 天球の描画
        //skydome_->Draw(railCamera_->GetViewProjection());

        stage_->DrawStage(railCamera_->GetViewProjection());
    }
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

    //#ifdef _DEBUG
    //    if (input_->TriggerKey(DIK_SPACE)) {
    //        pos = {
    //            nCubePos_.x * stage_->blockSideLength_,
    //            (nCubePos_.y + 1) * stage_->blockSideLength_,
    //            nCubePos_.z * stage_->blockSideLength_
    //        };
    //        railCamera_->SetWorldPosition(pos);
    //    }
    //    if (input_->TriggerKey(DIK_LSHIFT)) {
    //        pos = {
    //            nCubePos_.x * stage_->blockSideLength_,
    //            (nCubePos_.y - 1) * stage_->blockSideLength_,
    //            nCubePos_.z * stage_->blockSideLength_
    //        };
    //        railCamera_->SetWorldPosition(pos);
    //    }
    //    if (input_->TriggerKey(DIK_P)) {
    //        railCamera_->GetWorldTransform()->rotation_.y += 45;
    //    }
    //#endif
}

void GameScene::SearchUnderfootBlock()
{
    Vector3 checkRot;
    WorldTransform checkWT;
}
