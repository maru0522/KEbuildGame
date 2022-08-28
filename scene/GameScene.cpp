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

    // 敵の生成コマンドの読み込み
    LoadEnemyPopData();

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

    UpdateEnemyPopCommands();

    // 敵の更新
    // デスフラグの立った敵を削除
    enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->isDead(); });
    // 敵更新
    for (std::unique_ptr<Enemy>& enemy : enemys_) {
        enemy->Update();
    }

    // 天球の更新
    skydome_->Update();
    // レールカメラの更新
    railCamera_->Update();

    CheckAllCollision();

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

    // 敵の描画
    for (std::unique_ptr<Enemy>& enemy : enemys_) {
        enemy->Draw(railCamera_->GetViewProjection());
    }

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

void GameScene::PopEnemy(Vector3& pos)
{
    // 敵の生成
    std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
    // 敵の初期化
    newEnemy->Initialize(model_, textureHandle_,pos);
    newEnemy->SetPlayer(player_.get());
    //newEnemy->SetParent(railCamera_->GetWorldTransform());
    // 敵の登録
    enemys_.push_back(std::move(newEnemy));
}

void GameScene::CheckAllCollision()
{
    // 判定対象AとBの座標
    Vector3 posA, posB;

    // 自弾リストの取得
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

    // 自キャラ半径
    const double pR = 1;

    // 敵キャラ半径
    const double eR = 1;

    // 自弾半径
    const double pBR = 1;

    // 敵弾半径
    const double eBR = 1;
#pragma region 自キャラと敵弾の当たり判定
    // 自キャラの座標
    posA = player_->GetWorldPosition();

    // 敵弾リストの取得
    for (std::unique_ptr<Enemy>& enemy : enemys_) {
        const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
        // 自キャラと敵弾すべての当たり判定
        for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
            // 敵弾の座標
            posB = bullet->GetWorldPosition();

            // 座標AとBの距離を求める
            double diffPosAB =
                std::pow(posB.x - posA.x, 2) +
                std::pow(posB.y - posA.y, 2) +
                std::pow(posB.z - posA.z, 2);

            // 球と球の交差判定
            if (diffPosAB <= std::pow(pR + eBR, 2)) {
                // 自キャラの衝突時コールバックを呼び出す
                player_->OnCollision();
                // 敵弾の衝突時コールバックを呼び出す
                bullet->OnCollision();
            }
        }
    }
#pragma endregion

#pragma region 敵キャラと自弾の当たり判定
    for (std::unique_ptr<Enemy>& enemy : enemys_) {
        // 敵キャラの座標
        posA = enemy->GetWorldPosition();

        // 敵キャラと自弾すべての当たり判定
        for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
            // 自弾の座標
            posB = bullet->GetWorldPosition();

            // 座標AとBの距離を求める
            double diffPosAB =
                std::pow(posB.x - posA.x, 2) +
                std::pow(posB.y - posA.y, 2) +
                std::pow(posB.z - posA.z, 2);

            // 球と球の交差判定
            if (diffPosAB <= std::pow(eR + pBR, 2)) {
                // 敵キャラの衝突時コールバックを呼び出す
                enemy->OnCollision();
                // 自弾弾の衝突時コールバックを呼び出す
                bullet->OnCollision();
            }
        }
    }
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
    for (std::unique_ptr<Enemy>& enemy : enemys_) {
        // 敵弾リストの取得
        const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
        // 自弾全てと敵弾すべての当たり判定
        for (const std::unique_ptr<PlayerBullet>& pBullet : playerBullets) {
            for (const std::unique_ptr<EnemyBullet>& eBullet : enemyBullets) {
                // 自弾の座標
                posA = pBullet->GetWorldPosition();

                // 敵弾の座標
                posB = eBullet->GetWorldPosition();

                // 座標AとBの距離を求める
                double diffPosAB =
                    std::pow(posB.x - posA.x, 2) +
                    std::pow(posB.y - posA.y, 2) +
                    std::pow(posB.z - posA.z, 2);

                // 球と球の交差判定
                if (diffPosAB <= std::pow(pBR + eBR, 2)) {
                    // 自キャラの衝突時コールバックを呼び出す
                    pBullet->OnCollision();
                    // 敵弾の衝突時コールバックを呼び出す
                    eBullet->OnCollision();
                }
            }
        }
    }
#pragma endregion
}

void GameScene::LoadEnemyPopData()
{
    // ファイルを開く
    std::ifstream file;
    file.open("Resources/Task1_2Resources/enemyPos.csv");
    assert(file.is_open());

    // ファイルの内容を文字列ストリームにコピー
    enemyPosCommands_ << file.rdbuf();

    // ファイルを閉じる
    file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
    // 待機処理
    if (isWaitEnemyPop_) {
        waitTime_--;
        if (waitTime_ <= 0) {
            // 待機完了
            isWaitEnemyPop_ = false;
        }
        return;
    }

    // 1行分の文字列を入れる変数
    std::string line;

    // コマンド実行ループ
    while (std::getline(enemyPosCommands_, line)) {
        // 1行分の文字列をストリームに変換して解析しやすくする
        std::istringstream line_stream(line);

        std::string word;
        // ,区切りで行の先頭文字列を取得
        std::getline(line_stream, word, ',');

        // "//"から始まる行はコメント
        if (word.find("//") == 0) {
            // コメント行を飛ばす
            continue;
        }

        // POPコマンド
        if (word.find("POP") == 0) {
            // x座標
            std::getline(line_stream, word, ',');
            float x = static_cast<float>(std::atof(word.c_str()));

            // y座標
            std::getline(line_stream, word, ',');
            float y = static_cast<float>(std::atof(word.c_str()));

            // z座標
            std::getline(line_stream, word, ',');
            float z = static_cast<float>(std::atof(word.c_str()));

            Vector3 pos(x, y, z);

            // 敵を発生させる
            PopEnemy(pos);
        }
        // WAITコマンド
        else if (word.find("WAIT") == 0) {
            std::getline(line_stream, word, ',');

            // 待ち時間
            int32_t waitTime = atoi(word.c_str());

            // 待機開始
            isWaitEnemyPop_ = true;
            waitTime_ = waitTime;

            // コマンドループを抜ける
            break;
        }
    }
}
