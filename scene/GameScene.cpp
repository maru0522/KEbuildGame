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
    delete SModel_;
    delete AModel_;
    delete DModel_;
    delete CLEARModel_;
    delete GAMEOVERModel_;
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
    CLEARModel_ = Model::CreateFromOBJ("CLEAR");
    GAMEOVERModel_ = Model::CreateFromOBJ("GAMEOVER");


    // スプライトテクスチャハンドル
    texTutorialMessageBackHandle_ = TextureManager::Load("Task1_2Resources/messageBackGray.png");
    texTutorialMessages_[0] = TextureManager::Load("Task1_2Resources/TutorialMessage1.png");
    texTutorialMessages_[1] = TextureManager::Load("Task1_2Resources/TutorialMessage2.png");
    texTutorialMessages_[2] = TextureManager::Load("Task1_2Resources/TutorialMessage3.png");
    texTutorialMessages_[3] = TextureManager::Load("Task1_2Resources/TutorialMessage4.png");
    texTutorialMessages_[4] = TextureManager::Load("Task1_2Resources/TutorialMessage5.png");
    texTutorialMessages_[5] = TextureManager::Load("Task1_2Resources/TutorialMessage6.png");
    texTutorialMessages_[6] = TextureManager::Load("Task1_2Resources/TutorialMessage7.png");
    texTutorialMessages_[7] = TextureManager::Load("Task1_2Resources/TutorialMessage8.png");
    texTutorialMessages_[8] = TextureManager::Load("Task1_2Resources/TutorialMessage9.png");
    texTutorialMessages_[9] = TextureManager::Load("Task1_2Resources/TutorialMessage10.png");
    texTutorialMessages_[10] = TextureManager::Load("Task1_2Resources/TutorialMessage11.png");
    texTutorialMessages_[11] = TextureManager::Load("Task1_2Resources/TutorialMessage12.png");
    texTutorialMessages_[12] = TextureManager::Load("Task1_2Resources/TutorialMessage13.png");
    texTutorialMessages_[13] = TextureManager::Load("Task1_2Resources/TutorialMessage14.png");
    texTutorialMessages_[14] = TextureManager::Load("Task1_2Resources/TutorialMessage15.png");
    texSpriteW_ = TextureManager::Load("Task1_2Resources/SpriteW.png");
    texSpriteA_ = TextureManager::Load("Task1_2Resources/SpriteA.png");
    texSpriteS_ = TextureManager::Load("Task1_2Resources/SpriteS.png");
    texSpriteD_ = TextureManager::Load("Task1_2Resources/SpriteD.png");
    texSpriteWPush_ = TextureManager::Load("Task1_2Resources/SpriteWPush.png");
    texSpriteAPush_ = TextureManager::Load("Task1_2Resources/SpriteAPush.png");
    texSpriteSPush_ = TextureManager::Load("Task1_2Resources/SpriteSPush.png");
    texSpriteDPush_ = TextureManager::Load("Task1_2Resources/SpriteDPush.png");
    texSpriteWASDBack_ = TextureManager::Load("Task1_2Resources/WASDBack.png");
    texSpriteC_ = TextureManager::Load("Task1_2Resources/SpriteC.png");
    texSpriteCPush_ = TextureManager::Load("Task1_2Resources/SpriteCPush.png");
    texSpriteCameraRIPTA_ = TextureManager::Load("Task1_2Resources/SpriteCameraRIPTA.png");
    texSpriteCameraMIWATASHI_ = TextureManager::Load("Task1_2Resources/SpriteCameraMIWATASHI.png");
    texSpriteScores_[0] = TextureManager::Load("numbers/Sprite0.png");
    texSpriteScores_[1] = TextureManager::Load("numbers/Sprite1.png");
    texSpriteScores_[2] = TextureManager::Load("numbers/Sprite2.png");
    texSpriteScores_[3] = TextureManager::Load("numbers/Sprite3.png");
    texSpriteScores_[4] = TextureManager::Load("numbers/Sprite4.png");
    texSpriteScores_[5] = TextureManager::Load("numbers/Sprite5.png");
    texSpriteScores_[6] = TextureManager::Load("numbers/Sprite6.png");
    texSpriteScores_[7] = TextureManager::Load("numbers/Sprite7.png");
    texSpriteScores_[8] = TextureManager::Load("numbers/Sprite8.png");
    texSpriteScores_[9] = TextureManager::Load("numbers/Sprite9.png");
    texSpriteStringRemain_ = TextureManager::Load("numbers/SpriteRemain.png");
    texTutorialDisplayMemoryBlock_ = TextureManager::Load("blockTextures/Memory.png");
    texTutorialDisplayGoalBlock_ = TextureManager::Load("blockTextures/Carpet.png");


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
    texCLEARHandle_ = TextureManager::Load("CLEAR/CLEAR.png");
    texGAMEOVERHandle_ = TextureManager::Load("GAMEOVER/GAMEOVER.png");

    // 音
    clickSound_ = audio_->LoadWave("sounds/clickSound.wav");
    moveCursorSound_ = audio_->LoadWave("sounds/cursorMoveSound.wav");
    clearSound_ = audio_->LoadWave("sounds/Clear.wav");
    gameOverSound_ = audio_->LoadWave("sounds/gameOver.wav");
    BGM_ = audio_->LoadWave("sounds/BGM.wav");

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

    // スプライト初期化
    tutorialMessageBack_.reset(Sprite::Create(texTutorialMessageBackHandle_, { 140,50 }));
    tutorialMessages_[0].reset(Sprite::Create(texTutorialMessages_[0], { 140,50 }));
    tutorialMessages_[1].reset(Sprite::Create(texTutorialMessages_[1], { 140,50 }));
    tutorialMessages_[2].reset(Sprite::Create(texTutorialMessages_[2], { 140,50 }));
    tutorialMessages_[3].reset(Sprite::Create(texTutorialMessages_[3], { 140,50 }));
    tutorialMessages_[4].reset(Sprite::Create(texTutorialMessages_[4], { 140,50 }));
    tutorialMessages_[5].reset(Sprite::Create(texTutorialMessages_[5], { 140,50 }));
    tutorialMessages_[6].reset(Sprite::Create(texTutorialMessages_[6], { 140,50 }));
    tutorialMessages_[7].reset(Sprite::Create(texTutorialMessages_[7], { 140,50 }));
    tutorialMessages_[8].reset(Sprite::Create(texTutorialMessages_[8], { 140,50 }));
    tutorialMessages_[9].reset(Sprite::Create(texTutorialMessages_[9], { 140,50 }));
    tutorialMessages_[10].reset(Sprite::Create(texTutorialMessages_[10], { 140,50 }));
    tutorialMessages_[11].reset(Sprite::Create(texTutorialMessages_[11], { 140,50 }));
    tutorialMessages_[12].reset(Sprite::Create(texTutorialMessages_[12], { 140,50 }));
    tutorialMessages_[13].reset(Sprite::Create(texTutorialMessages_[13], { 140,50 }));
    tutorialMessages_[14].reset(Sprite::Create(texTutorialMessages_[14], { 140,50 }));
    spriteW_.reset(Sprite::Create(texSpriteW_, { 120,590 }));
    spriteA_.reset(Sprite::Create(texSpriteA_, { 72,640 }));
    spriteS_.reset(Sprite::Create(texSpriteS_, { 120,640 }));
    spriteD_.reset(Sprite::Create(texSpriteD_, { 170,640 }));
    spriteWPush_.reset(Sprite::Create(texSpriteWPush_, { 120,590 }));
    spriteAPush_.reset(Sprite::Create(texSpriteAPush_, { 72,640 }));
    spriteSPush_.reset(Sprite::Create(texSpriteSPush_, { 120,640 }));
    spriteDPush_.reset(Sprite::Create(texSpriteDPush_, { 170,640 }));
    spriteWASDBack_.reset(Sprite::Create(texSpriteWASDBack_, { 69,590 }));
    spriteC_.reset(Sprite::Create(texSpriteC_, { 1040,640 }));
    spriteCPush_.reset(Sprite::Create(texSpriteC_, { 1040,640 }));
    spriteCameraRIPTA_.reset(Sprite::Create(texSpriteCameraRIPTA_, { 1085,622 }));
    spriteCameraMIWATASHI_.reset(Sprite::Create(texSpriteCameraMIWATASHI_, { 1085,622 }));
    spriteCCameraBack_.reset(Sprite::Create(texSpriteWASDBack_, { 1040,590 }));
    for (int i = 0; i < 3; i++) {
        spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[0], { 95.0f + 50.0f * i, 70 }));
    }
    spriteStringRemain_.reset(Sprite::Create(texSpriteStringRemain_, { 100,50 }));
    spriteStringRemainBack_.reset(Sprite::Create(texSpriteWASDBack_, { 95,35 }));
    tutorialDisplayMemoryBlock_.reset(Sprite::Create(texTutorialDisplayMemoryBlock_, { 890,51 }));
    tutorialDisplayMemoryBlock_->SetSize({ 32,32 });
    tutorialDisplayGoalBlock_.reset(Sprite::Create(texTutorialDisplayGoalBlock_, { 335,51 }));
    tutorialDisplayGoalBlock_->SetSize({ 32,32 });



    // オブジェクトたちの生成
    arrowUp_ = std::make_unique<Object>();
    arrowDown_ = std::make_unique<Object>();
    gear8_ = std::make_unique<Object>();
    gear12_ = std::make_unique<Object>();
    gear16_ = std::make_unique<Object>();
    selectStageGear8_ = std::make_unique<Object>();
    for (int i = 0; i < 4; i++) {
        stage1_4Gears8_[i] = std::make_unique<Object>();
        stage5_8Gears12_[i] = std::make_unique<Object>();
    }
    stage9_10Gears16_[0] = std::make_unique<Object>();
    stage9_10Gears16_[1] = std::make_unique<Object>();

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
    selectStageGear8_->Initialize(gear8Model_, texGear8Handle_, { 85.2f, 85, 110 }, { 0,Calc::ConvertToRadian(90),0 });
    selectStageGear8_->SetIsRolling(true);
    selectStageGear8_->SetIsLeftRoll(true);
    stage1_4Gears8_[0]->Initialize(gear8Model_, texGear8Handle_, { 85.0f, 88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage1_4Gears8_[0]->SetIsRolling(true);
    stage1_4Gears8_[1]->Initialize(gear8Model_, texGear8Handle_, { 90.0f, 88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage1_4Gears8_[1]->SetIsRolling(true);
    stage1_4Gears8_[2]->Initialize(gear8Model_, texGear8Handle_, { 95.0f, 88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage1_4Gears8_[2]->SetIsRolling(true);
    stage1_4Gears8_[3]->Initialize(gear8Model_, texGear8Handle_, { 100.0f, 88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage1_4Gears8_[3]->SetIsRolling(true);
    stage5_8Gears12_[0]->Initialize(gear12Model_, texGear12Handle_, { 105.0f,88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage5_8Gears12_[0]->SetIsRolling(true);
    stage5_8Gears12_[1]->Initialize(gear12Model_, texGear12Handle_, { 110.0f,88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage5_8Gears12_[1]->SetIsRolling(true);
    stage5_8Gears12_[2]->Initialize(gear12Model_, texGear12Handle_, { 115.0f,88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage5_8Gears12_[2]->SetIsRolling(true);
    stage5_8Gears12_[3]->Initialize(gear12Model_, texGear12Handle_, { 120.0f,88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage5_8Gears12_[3]->SetIsRolling(true);
    stage9_10Gears16_[0]->Initialize(gear16Model_, texGear16Handle_, { 125.0f, 88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage9_10Gears16_[0]->SetIsRolling(true);
    stage9_10Gears16_[1]->Initialize(gear16Model_, texGear16Handle_, { 130.0f, 88.9f, 100 }, { 0,Calc::ConvertToRadian(90),0 });
    stage9_10Gears16_[1]->SetIsRolling(true);


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
    CLEAR_ = std::make_unique<Object>();
    GAMEOVER_ = std::make_unique<Object>();
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
    YESGear12_->Initialize(gear12Model_, texGear12Handle_, { 87.1f,91,113 }, { 0,Calc::ConvertToRadian(90),0 });
    YESGear12_->SetIsRolling(true);
    NOGear12_->Initialize(gear12Model_, texGear12Handle_, { 87.1f, 87.5f, 113 }, { 0,Calc::ConvertToRadian(90),0 });
    NOGear12_->SetIsRolling(true);
    PLAYTUTORIAL_->Initialize(PLAYTUTORIALModel_, texPLAYTUTORIALHandle_, { 91,98,135 }, { 0,Calc::ConvertToRadian(90),0 });
    PLAYTUTORIAL_->SetScale({ 1.5,1.5,1.5 });
    W_->Initialize(WModel_, texWASDHandle_, { 95,91,113 }, { 0,Calc::ConvertToRadian(90),0 });
    W_->SetScale({ 0.7,0.7,0.7 });
    S_->Initialize(SModel_, texWASDHandle_, { 95,85.5f,113 }, { 0,Calc::ConvertToRadian(90),0 });
    S_->SetScale({ 0.7,0.7,0.7 });
    A_->Initialize(AModel_, texWASDHandle_, { 78.5f,83,113 }, { 0,Calc::ConvertToRadian(90),0 });
    A_->SetScale({ 0.7,0.7,0.7 });
    D_->Initialize(DModel_, texWASDHandle_, { 92,83,113 }, { 0,Calc::ConvertToRadian(90),0 });
    D_->SetScale({ 0.7,0.7,0.7 });
    CLEAR_->Initialize(CLEARModel_, texCLEARHandle_, { 92,98,120 }, { 0,Calc::ConvertToRadian(90),0 });
    GAMEOVER_->Initialize(GAMEOVERModel_, texGAMEOVERHandle_, { 95,98,120 }, { 0,Calc::ConvertToRadian(90),0 });



    stage_->InitStage(model_, textureHandle_);
    stage_->LoadStage("Resources/Task1_2Resources/debugRoom.csv");
}

void GameScene::Update() {

    if (gameScene_ == Title) {
#pragma region キーボード操作
        if (input_->TriggerKey(DIK_SPACE)) {
            gameScene_ = GuidQuestion;
            audio_->PlayWave(clickSound_, false, 0.2);
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
            audio_->PlayWave(moveCursorSound_, false, 0.3);
        }
        if (input_->TriggerKey(DIK_S)) {
            isPlayTutorial = false;
            gear8_->SetPos({ 86, 87, 110 });
            audio_->PlayWave(moveCursorSound_, false, 0.3);
        }

        if (input_->TriggerKey(DIK_SPACE)) {
            if (isPlayTutorial == true) {
                gameScene_ = Tutorial;
                stage_->Reset();
                stage_->LoadStage("Resources/Task1_2Resources/tutorial.csv");
                player_->SetPos({ 92,96,92 });
                player_->SetActionRemain(100);
                railCamera_->SetWorldPosition({ 92, 98, 82 });
                arrowUp_->SetPos({ 83,83,113 });
                arrowUp_->SetRot({ 0,Calc::ConvertToRadian(-90),0 });
                arrowDown_->SetPos({ 87,83,113 });
                arrowDown_->SetRot({ 0,Calc::ConvertToRadian(90),0 });
                A_->SetPos({ 73.5f + indexSelectStage * 5.0f,83,113 });
                D_->SetPos({ 87 + indexSelectStage * 5.0f,83,113 });
                selectStageGear8_->SetPos({ 80.2f + indexSelectStage * 5.0f,85,110 });
                player_->SetRot({ 0,Calc::ConvertToRadian(0),0 });
            }
            else {
                gameScene_ = SelectStage;
                stage_->Reset();
                railCamera_->SetWorldPosition({ 85,90,90 });
                arrowUp_->SetPos({ 83,83,113 });
                arrowUp_->SetRot({ 0,Calc::ConvertToRadian(-90),0 });
                arrowDown_->SetPos({ 87,83,113 });
                arrowDown_->SetRot({ 0,Calc::ConvertToRadian(90),0 });
                indexSelectStage = 1;

                A_->SetPos({ 73.5f + indexSelectStage * 5.0f,83,113 });
                D_->SetPos({ 87 + indexSelectStage * 5.0f,83,113 });
                arrowUp_->SetPos({ 78 + indexSelectStage * 5.0f,83,113 });
                arrowDown_->SetPos({ 82 + indexSelectStage * 5.0f,83,113 });
                railCamera_->SetWorldPosition({ 80.0f + indexSelectStage * 5.0f,90,90 });
                selectStageGear8_->SetPos({ 80.2f + indexSelectStage * 5.0f,85,110 });

                player_->SetPos({ 80,88.7f,100 });
                player_->SetRot({ 0,Calc::ConvertToRadian(180),0 });
                player_->Update();
            }
            audio_->PlayWave(clickSound_, false, 0.2);
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
        if (gameStats_ == Operate) {
            if (indexMessage_ >= 10) {
                if (input_->TriggerKey(DIK_C)) {
                    if (indexCamera_ == Rail) {
                        indexCamera_ = FPS;
                    }
                    else if (indexCamera_ == FPS) {
                        indexCamera_ = Rail;
                    }
                    audio_->PlayWave(clickSound_, false, 0.2);
                }
            }
        }
        if (input_->TriggerKey(DIK_SPACE)) {
            indexMessage_++;
            if (gameStats_ == Clear || gameStats_ == Gameover) {
                gameScene_ = SelectStage;

                indexCamera_ = Rail;
                gameStats_ = Operate;
                indexMessage_ = 0;
                stage_->Reset();

                railCamera_->SetWorldPosition({ 85,90,90 });
                arrowUp_->SetPos({ 83,83,113 });
                arrowUp_->SetRot({ 0,Calc::ConvertToRadian(-90),0 });
                arrowDown_->SetPos({ 87,83,113 });
                arrowDown_->SetRot({ 0,Calc::ConvertToRadian(90),0 });

                gameStats_ = Operate;
                indexCamera_ = Rail;
                indexSelectStage = 1;

                player_->SetPos({ 80,88.7f,100 });
                player_->SetRot({ 0,Calc::ConvertToRadian(180),0 });
                player_->Update();
            }
            audio_->PlayWave(clickSound_, false, 0.2);
        }
#pragma endregion
#pragma region オブジェクト更新
        uint32_t remainCopy = player_->GetRemain();
        for (int i = 0; i < 3; i++) {
            int num = pow(10, 3 - i - 1);
            indexRemain_[i] = remainCopy / num;
            remainCopy = remainCopy % num;

            if (indexRemain_[i] == 0) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[0], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 1) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[1], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 2) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[2], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 3) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[3], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 4) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[4], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 5) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[5], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 6) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[6], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 7) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[7], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 8) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[8], { 95.0f + 50.0f * i, 70 }));
            }
            else if (indexRemain_[i] == 9) {
                spriteRemain_[i].reset(Sprite::Create(texSpriteScores_[9], { 95.0f + 50.0f * i, 70 }));
            }
        }

        arrowUp_->Update();
        arrowDown_->Update();

        // カメラの更新
        if (indexCamera_ == Rail) {
            railCamera_->Update();
            //if (!isFinishEasing4TutorialCam_) {
            //    if (fpsCount4TutorialCam_ < 200) {
            //        fpsCount4TutorialCam_++;
            //    }
            //    moveValue4TutorialCam_ = {
            //        0,
            //        static_cast<float>(Calc::EaseOutSine(fpsCount4TutorialCam_ / 200) * 8),
            //        static_cast<float>(Calc::EaseOutSine(fpsCount4TutorialCam_ / 200) * 4),
            //    };
            //    railCamera_->SetWorldPosition({
            //        railCamera_->GetWorldTransform2().translation_.x + moveValue4TutorialCam_.x,
            //        railCamera_->GetWorldTransform2().translation_.y + moveValue4TutorialCam_.y,
            //        railCamera_->GetWorldTransform2().translation_.z + moveValue4TutorialCam_.z
            //                                  });
            //    if (fpsCount4TutorialCam_ >= 200) {
            //        isFinishEasing4TutorialCam_ = true;
            //    }
            //}
        }
        else if (indexCamera_ == FPS) {
            fpsCamera_->Update(player_->GetWorldTransform());
        }

        if (indexMessage_ >= 15) {
            if (gameStats_ == Operate) {
                // 自機更新 
                player_->Update();
            }
        }
        stage_->UpdateStage();

        //input_->IsTriggerMouse

        // 天球の更新
        skydome_->Update();

        if (player_->GetUnderBlockId() == GOAL) {
            gameStats_ = Clear;
            if (!isFanfare_) {
                isFanfare_ = true;
                audio_->PlayWave(clearSound_, false, 0.05);
            }
        }
        if (player_->GetRemain() <= 0) {
            gameStats_ = Gameover;
            if (!isFanfare_) {
                isFanfare_ = true;
                audio_->PlayWave(gameOverSound_, false, 0.3);
            }
        }
#pragma endregion
    }
    else if (gameScene_ == SelectStage) {
#pragma region キーボード操作
        if (0 < indexSelectStage) {
            if (input_->TriggerKey(DIK_A)) {
                indexSelectStage--;

                A_->SetPos({ 73.5f + indexSelectStage * 5.0f,83,113 });
                D_->SetPos({ 87 + indexSelectStage * 5.0f,83,113 });
                arrowUp_->SetPos({ 78 + indexSelectStage * 5.0f,83,113 });
                arrowDown_->SetPos({ 82 + indexSelectStage * 5.0f,83,113 });
                railCamera_->SetWorldPosition({ 80.0f + indexSelectStage * 5.0f,90,90 });
                selectStageGear8_->SetPos({ 80.2f + indexSelectStage * 5.0f,85,110 });
                audio_->PlayWave(moveCursorSound_, false, 0.3);
            }
        }
        if (indexSelectStage < 10) {
            if (input_->TriggerKey(DIK_D)) {
                indexSelectStage++;
                A_->SetPos({ 73.5f + indexSelectStage * 5.0f,83,113 });
                D_->SetPos({ 87 + indexSelectStage * 5.0f,83,113 });
                arrowUp_->SetPos({ 78 + indexSelectStage * 5.0f,83,113 });
                arrowDown_->SetPos({ 82 + indexSelectStage * 5.0f,83,113 });
                railCamera_->SetWorldPosition({ 80.0f + indexSelectStage * 5.0f,90,90 });
                selectStageGear8_->SetPos({ 80.2f + indexSelectStage * 5.0f,85,110 });
                audio_->PlayWave(moveCursorSound_, false, 0.3);
            }
        }

        if (input_->TriggerKey(DIK_SPACE)) {
            if (indexSelectStage == 0) {
                gameScene_ = Title;

                arrowUp_->SetPos({ 94.4f, 89.2f, 110 });
                arrowUp_->SetRot({ Calc::ConvertToRadian(-90),Calc::ConvertToRadian(0),0 });
                arrowDown_->SetPos({ 94.4f, 87.7f, 110 });
                arrowDown_->SetRot({ Calc::ConvertToRadian(90),Calc::ConvertToRadian(0),0 });
                railCamera_->SetWorldPosition({ 90,90,90 });
                isPlayTutorial = false;
                gear8_->SetPos({ 86, 87, 110 });
                indexMessage_ = 0;
                gameStats_ = Operate;
                indexSelectStage = 1;

                player_->SetPos({ 92,96,92 });
                player_->Reset();
            }
            else if (1 <= indexSelectStage && indexSelectStage <= 10) {
                indexPlayGameStage = indexSelectStage;
            }
            indexSelectStage = 1;
            audio_->PlayWave(clickSound_, false, 0.2);
        }
#pragma endregion
#pragma region オブジェクト更新
        if (audio_->IsPlaying(clearSound_)) {
            audio_->StopWave(clearSound_);
        }
        if (audio_->IsPlaying(gameOverSound_)) {
            audio_->StopWave(gameOverSound_);
        }
        if (isFanfare_) {
            isFanfare_ = false;
        }

        // レールカメラの更新
        railCamera_->Update();
        A_->Update();
        D_->Update();
        arrowUp_->Update();
        arrowDown_->Update();

        selectStageGear8_->Update();
        if (indexSelectStage == 1) {
            stage1_4Gears8_[0]->Update();
        }
        else if (indexSelectStage == 2) {
            stage1_4Gears8_[1]->Update();
        }
        else if (indexSelectStage == 3) {
            stage1_4Gears8_[2]->Update();
        }
        else if (indexSelectStage == 4) {
            stage1_4Gears8_[3]->Update();
        }
        else if (indexSelectStage == 5) {
            stage5_8Gears12_[0]->Update();
        }
        else if (indexSelectStage == 6) {
            stage5_8Gears12_[1]->Update();
        }
        else if (indexSelectStage == 7) {
            stage5_8Gears12_[2]->Update();
        }
        else if (indexSelectStage == 8) {
            stage5_8Gears12_[3]->Update();
        }
        else if (indexSelectStage == 9) {
            stage9_10Gears16_[0]->Update();
        }
        else if (indexSelectStage == 10) {
            stage9_10Gears16_[1]->Update();
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
    debugText_->SetPos(600, 430);
    debugText_->Printf("indexSelectStage:%d", indexSelectStage);
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

            if (gameStats_ == Clear) {
                CLEAR_->Draw(railCamera_->GetViewProjection(), false);
            }
            if (gameStats_ == Gameover) {
                GAMEOVER_->Draw(railCamera_->GetViewProjection(), false);
            }
        }
        else if (indexCamera_ == FPS) {
            // 自キャラの描画
            /*player_->Draw(fpsCamera_->GetViewProjection());*/
            //player_->Draw(debugCamera_->GetViewProjection());

            // 天球の描画
            //skydome_->Draw(railCamera_->GetViewProjection());

            stage_->DrawStage(fpsCamera_->GetViewProjection());

            if (gameStats_ == Clear) {
                CLEAR_->Draw(fpsCamera_->GetViewProjection(), false);
            }
            if (gameStats_ == Gameover) {
                GAMEOVER_->Draw(fpsCamera_->GetViewProjection(), false);
            }
        }
    }
    else if (gameScene_ == SelectStage) {
        A_->Draw(railCamera_->GetViewProjection(), false);
        D_->Draw(railCamera_->GetViewProjection(), false);
        arrowUp_->Draw(railCamera_->GetViewProjection(), false);
        arrowDown_->Draw(railCamera_->GetViewProjection(), false);
        selectStageGear8_->Draw(railCamera_->GetViewProjection(), false);
        stage1_4Gears8_[0]->Draw(railCamera_->GetViewProjection(), false);
        stage1_4Gears8_[1]->Draw(railCamera_->GetViewProjection(), false);
        stage1_4Gears8_[2]->Draw(railCamera_->GetViewProjection(), false);
        stage1_4Gears8_[3]->Draw(railCamera_->GetViewProjection(), false);
        stage5_8Gears12_[0]->Draw(railCamera_->GetViewProjection(), false);
        stage5_8Gears12_[1]->Draw(railCamera_->GetViewProjection(), false);
        stage5_8Gears12_[2]->Draw(railCamera_->GetViewProjection(), false);
        stage5_8Gears12_[3]->Draw(railCamera_->GetViewProjection(), false);
        stage9_10Gears16_[0]->Draw(railCamera_->GetViewProjection(), false);
        stage9_10Gears16_[1]->Draw(railCamera_->GetViewProjection(), false);

        player_->Draw(railCamera_->GetViewProjection());
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

    if (gameScene_ == Title) {

    }
    else if (gameScene_ == GuidQuestion) {

    }
    else if (gameScene_ == Tutorial) {
        if (indexMessage_ < 15) {
            tutorialMessageBack_->Draw();
            tutorialMessages_[indexMessage_]->Draw();
        }
        if (indexMessage_ == 2) {
            tutorialDisplayMemoryBlock_->Draw();
        }
        if (indexMessage_ == 9) {
            tutorialDisplayGoalBlock_->Draw();
        }

        if (indexMessage_ >= 2) {
            spriteWASDBack_->Draw();
            if (input_->PushKey(DIK_W)) {
                spriteWPush_->Draw();
            }
            else {
                spriteW_->Draw();
            }
            if (input_->PushKey(DIK_A)) {
                spriteAPush_->Draw();
            }
            else {
                spriteA_->Draw();
            }
            if (input_->PushKey(DIK_S)) {
                spriteSPush_->Draw();
            }
            else {
                spriteS_->Draw();
            }
            if (input_->PushKey(DIK_D)) {
                spriteDPush_->Draw();
            }
            else {
                spriteD_->Draw();
            }
        }
        if (indexMessage_ >= 10) {
            spriteCCameraBack_->Draw();
            spriteC_->Draw();
            spriteCPush_->Draw();
            if (indexCamera_ == Rail) {
                spriteCameraMIWATASHI_->Draw();
            }
            else if (indexCamera_ == FPS) {
                spriteCameraRIPTA_->Draw();
            }
        }
        if (indexMessage_ >= 15) {
            spriteStringRemainBack_->Draw();
            spriteStringRemain_->Draw();
            for (int i = 0; i < 3; i++) {
                spriteRemain_[i]->Draw();
            }
        }
    }
    else if (gameScene_ == SelectStage) {

    }
    else if (gameScene_ == PlayGame) {

    }

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
