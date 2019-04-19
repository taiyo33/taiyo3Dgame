//=============================================================================
//
// メイン処理 [main.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "light.h"
#include "player.h"
#include "hitEffect.h"
#include "bullet.h"
#include "time.h"
#include "field.h"
#include "checkhit.h"
#include "explosion.h"
#include "bulletEffect.h"
#include "block.h"
#include "bulletGauge.h"
#include "lifeGauge.h"
#include "ball.h"
#include "ai.h"
#include "ballGauge.h"
#include "gaugeEffect.h"
#include "gameCall.h"
#include "chargeEffect.h"
#include "pause.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"					// ウインドウのクラス名
#define WINDOW_NAME		"3Dゲーム「Re:Plection」"	// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Init(HINSTANCE hInstance, HWND hWnd);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;			// Direct3D オブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
#ifdef _DEBUG
int						g_nCountFPS;			// FPSカウンタ
#endif
bool					g_bDispDebug = true;	// デバッグ表示ON/OFF
int						StageSelect;
bool					screen = true;
bool					pause = false;
//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif

				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);	// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージ加算合成処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// 各初期化
	Init(hInstance, hWnd);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// 入力処理の終了処理
	UninitInput();

	// デバッグ表示処理の終了処理
	UninitDebugProc();

	// バレットの終了処理
	UninitBullet();

	// プレイヤーの終了処理
	UninitPlayer();

	// エフェクトの終了処理
	UninitHitEffect();

	// タイトルの終了処理
	UninitTitle();

	// リザルトの終了処理
	UninitResult();

	// チュートリアルの終了処理
	UninitTutorial();

	// フィールドの終了処理
	UninitField();

	// 爆発エフェクトの終了処理
	UninitExplosion();

	// バレットエフェクトの終了処理
	UninitBulletEffect();

	// 左腕モデルの終了処理
	UninitLeftArm();

	// 右腕モデルの終了処理
	UninitRightArm();

	// 頭モデルの終了処理
	UninitHead();

	// ブロックの終了処理
	UninitBlock();

	// バレットゲージの終了処理
	UninitBulletGauge();

	// ライフゲージの終了処理
	UninitLifeGauge();

	// 子供モデルの終了処理
	UninitBall();

	// バトルゲージの終了処理
	UninitBallGauge();

	//
	UninitGaugeEffect();


	// ゲーム遷移テロップの終了処理
	UninitGameCall();

	// チャージエフェクトの終了処理
	UninitChargeEffect();

	// メニューの終了処理
	UninitPause();

	// サウンドの終了処理
	UninitSound();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	if(GetKeyboardTrigger(DIK_F1))
	{// デバッグ表示ON/OFF
		g_bDispDebug = g_bDispDebug ? false: true;
	}

	// 入力更新
	UpdateInput();

	switch (StageSelect)
	{
		case TITLE:
		{
			// タイトルの終了処理
			UpdateTitle();

			// モデル処理の更新
			UpdatePlayer();

			// 左腕の更新
			UpdateLeftArm();

			// 右腕の更新
			UpdateRightArm();

			// 頭の更新
			UpdateHead();

			// 子供の更新
			UpdateBall();

			break;
		}
		case TUTORIAL:
		{
			// チュートリアルの更新処理
			UpdateTutorial();

			break;
		}
		case STARTCALL:
		{
			// ゲーム遷移テロップの更新処理
			UpdateGameCall();

			// 子供の更新
			UpdateBall();

			break;
		}
		case START:
		{	
			if (!pause)
			{
				// ブロックの更新
				UpdateBlock();

				// フィールドの更新
				UpdateField();

				// カメラ更新
				UpdateCamera();

				// モデル処理の更新
				UpdatePlayer();

				// 左腕の更新
				UpdateLeftArm();

				// 右腕の更新
				UpdateRightArm();

				// 頭の更新
				UpdateHead();

				// キャラクターアイコンの更新
				//UpdateIcon();

				// 子供の更新
				UpdateBall();

				// チャージエフェクトの更新
				UpdateChargeEffect();

				// エフェクトの更新
				UpdateHitEffect();

				// 弾痕の更新
				UpdateExplosion();

				// 煙の更新
				UpdateBulletEffect();

				// バレットの更新
				UpdateBullet();

				// 時間の更新
				UpdateTime();

				// バレットゲージの更新
				UpdateBulletGauge();

				// ライフゲージの更新
				UpdateLifeGauge();

				// バトルゲージ更新
				UpdateBallGauge();

				// ゲージエフェクトの更新
				UpdateGaugeEffect();

				// 当たり判定
				CheckHit();
			}
			// ポーズ画面
			else if(pause)
			{
				// ポーズ画面の更新
				UpdatePause();
				// ゲージエフェクトの更新
				UpdateGaugeEffect();
			}

			break;
		}
		case FINISHCALL:
		{
			// 子供の更新
			UpdateBall();

			// ゲージエフェクトの更新
			UpdateGaugeEffect();

			// ゲーム遷移テロップの更新処理
			UpdateGameCall();

			break;
		}
		case RESULT:
		{
			// リザルトの更新
			UpdateResult();

			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(75, 144, 205, 180), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (StageSelect)
		{
			case TITLE:
			{
				// カメラの設定
				SetCamera();

				// フィールドの描画
				DrawField();

				// ブロックの描画
				DrawBlock();

				// プレイヤー処理の描画
				DrawPlayer();

				// 頭の描画
				DrawHead();

				// 左腕の描画
				DrawLeftArm();

				// 右腕の描画
				DrawRightArm();

				// 子供モデルの描画
				DrawBall();

				// タイトルの終了処理
				DrawTitle();

				break;
			}
			case TUTORIAL:
			{
				// チュートリアルの終了処理
				DrawTutorial();

				break;
			}
			case STARTCALL:
			{
				// カメラの設定
				SetCamera();

				// フィールドの描画
				DrawField();

				// ブロックの描画
				DrawBlock();

				// プレイヤー処理の描画
				DrawPlayer();

				// 頭の描画
				DrawHead();

				// 左腕の描画
				DrawLeftArm();

				// 右腕の描画
				DrawRightArm();

				// 子供モデルの描画
				DrawBall();

				// ゲーム遷移テロップの描画
				DrawGameCall();

				break;
			}
			case START:
			{
				// カメラの設定
				SetCamera();
				
				// フィールドの描画
				DrawField();

				// ブロックの描画
				DrawBlock();

				// バレットの描画
				DrawBullet();

				// 爆発エフェクトの描画
				DrawExplosion();

				// バレットエフェクトの描画
				DrawBulletEffect();
				
				// 子供モデルの描画
				DrawBall();

				// チャージエフェクトの描画
				DrawChargeEffect();

				// プレイヤー処理の描画
				DrawPlayer();

				// 頭の描画
				DrawHead();

				// 左腕の描画
				DrawLeftArm();

				// 右腕の描画
				DrawRightArm();

				// 被弾エフェクトの描画
				DrawHitEffect();

				// キャラクターアイコンの描画
				//DrawIcon();

				// 時間の描画
				DrawTime();

				// ライフゲージの描画
				DrawLifeGauge();

				// バレットゲージの描画
				DrawBulletGauge();

				// バトルゲージの描画
				DrawBallGauge();

				// ゲージエフェクトの描画
				DrawGaugeEffect();

				// ポーズメニューの描画
				if (pause)
				{
					DrawPause();
				}

#ifdef _DEBUG
				// デバッグ表示処理の描画
				if (g_bDispDebug)
				{
					DrawDebugProc();
				}

#endif

				break;
			}
			case FINISHCALL:
			{
				// カメラの設定
				SetCamera();

				// フィールドの描画
				DrawField();

				// ブロックの描画
				DrawBlock();

				// プレイヤー処理の描画
				DrawPlayer();

				// 頭の描画
				DrawHead();

				// 左腕の描画
				DrawLeftArm();

				// 右腕の描画
				DrawRightArm();

				// 子供モデルの描画
				DrawBall();

				// ライフゲージの描画
				DrawLifeGauge();

				// バレットゲージの描画
				DrawBulletGauge();

				// バトルゲージの描画
				DrawBallGauge();

				// ゲージエフェクトの描画
				DrawGaugeEffect();

				// ゲーム遷移テロップの描画
				DrawGameCall();

				break;
			}
			case RESULT:
			{

				// カメラの設定
				SetCamera();

				// フィールドの描画
				DrawField();

				// ブロックの描画
				DrawBlock();

				// プレイヤー処理の描画
				DrawPlayer();

				// 頭の描画
				DrawHead();

				// 左腕の描画
				DrawLeftArm();

				// 右腕の描画
				DrawRightArm();

				// 子供モデルの描画
				DrawBall();

				// ライフゲージの描画
				DrawLifeGauge();

				// バレットゲージの描画
				DrawBulletGauge();

				// バトルゲージの描画
				DrawBallGauge();

				// ゲージエフェクトの描画
				DrawGaugeEffect();


				// リザルトの終了処理
				DrawResult();

				break;
			}
		}

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//============================================================================
// ゲーム遷移の設置
// 引数： int Stage(遷移先の番号)
// 戻り値：なし
//============================================================================
void SetStage(int Stage)
{
	StageSelect = Stage;
}

//============================================================================
// ゲーム遷移の取得
// 引数： int Stage(遷移先の番号)
// 戻り値：なし
//============================================================================
int GetStage(void)
{
	return StageSelect;
}

//============================================================================
// ポーズ画面の設置
// 引　数：bool on(ポーズの使用状態)
// 戻り値：bool型
//============================================================================
bool SetPause(bool on)
{
	return pause = on;
}

//============================================================================
// ポーズ画面の取得
// 引　数：bool on(ポーズの使用状態)
// 戻り値：bool型
//============================================================================
bool GetPause(void)
{
	return &pause;
}

//============================================================================
// 各ファイルの初期化
//============================================================================
void Init(HINSTANCE hInstance, HWND hWnd)
{
	// サウンド初期化
	InitSound(hWnd);

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// デバッグ表示処理の初期化
	InitDebugProc();

	// ライトの初期化
	InitLight();

	// カメラの初期化
	InitCamera();

	// プレイヤーの初期化
	InitPlayer(0);

	// バレットの初期化
	InitBullet(0);

	// エフェクトの初期化
	InitHitEffect(0);

	// 爆発エフェクトの初期化
	InitExplosion(0);

	// 時間の初期化
	InitTime(0);

	// タイトルの初期化
	InitTitle(0);

	// ブロックの初期化
	InitBlock(0);

	// リザルトの初期化
	InitResult(0);

	// チュートリアルの初期化
	InitTutorial(0);

	// フィールドの初期化
	InitField(0);

	// バレットエフェクトの初期化
	InitBulletEffect(0);
	
	// 左腕モデルの初期化
	InitLeftArm();

	// 右腕モデルの初期化
	InitRightArm();

	// 頭モデルの初期化
	InitHead();

	// バレットゲージの初期化
	InitBulletGauge(0);

	// ライフゲージの初期化
	InitLifeGauge(0);

	// 子供モデルの初期化
	InitBall();

	// AIの初期化
	InitAi();

	// バトルゲージの初期化
	InitBallGauge(0);

	// ゲージエフェクトの初期化
	InitGaugeEffect(0);

	// ゲーム遷移テロップの初期化
	InitGameCall(0);

	// チャージエフェクトの初期化
	InitChargeEffect(0);

	// ポーズ画面の初期化
	InitPause(0);

}

//============================================================================
// ゲームの再初期化
// 引　数：な　し
// 戻り値：な　し
//============================================================================
void InitGame(void)
{
	// ライトの初期化
	InitLight();

	// カメラの初期化
	InitCamera();

	// プレイヤーの初期化
	InitPlayer(INIT_GAME);

	// バレットの初期化
	InitBullet(INIT_GAME);

	// エフェクトの初期化
	InitHitEffect(INIT_GAME);

	// 爆発エフェクトの初期化
	InitExplosion(INIT_GAME);

	// 時間の初期化
	InitTime(INIT_GAME);

	// タイトルの初期化
	InitTitle(INIT_GAME);

	// ブロックの初期化
	InitBlock(INIT_GAME);

	// リザルトの初期化
	InitResult(INIT_GAME);

	// チュートリアルの初期化
	InitTutorial(INIT_GAME);

	// フィールドの初期化
	InitField(INIT_GAME);

	// バレットエフェクトの初期化
	InitBulletEffect(INIT_GAME);

	// 左腕モデルの初期化
	InitLeftArm();

	// 右腕モデルの初期化
	InitRightArm();

	// 頭モデルの初期化
	InitHead();

	// バレットゲージの初期化
	InitBulletGauge(INIT_GAME);

	// ライフゲージの初期化
	InitLifeGauge(INIT_GAME);

	// 子供モデルの初期化
	InitBall();

	// AIの初期化
	InitAi();
	
	// ポーズ画面の初期化
	InitPause(INIT_GAME);

	// バトルゲージの初期化
	InitBallGauge(INIT_GAME);

	// ゲージエフェクトの初期化
	InitGaugeEffect(INIT_GAME);

	// ゲーム遷移テロップの初期化
	InitGameCall(INIT_GAME);

	// チャージエフェクトの初期化
	InitChargeEffect(INIT_GAME);
}

//============================================================================
// ゲームポーズ処理後の初期化
// 引　数：な　し
// 戻り値：な　し
//============================================================================
void InitPauseGame(void)
{
	// プレイヤーの初期化
	InitPlayer(INIT_GAME);

	// バレットの初期化
	InitBullet(INIT_GAME);

	// エフェクトの初期化
	InitHitEffect(INIT_GAME);

	// 爆発エフェクトの初期化
	InitExplosion(INIT_GAME);

	// 時間の初期化
	InitTime(INIT_GAME);

	// ブロックの初期化
	InitBlock(INIT_GAME);

	// リザルトの初期化
	InitResult(INIT_GAME);

	// バレットエフェクトの初期化
	InitBulletEffect(INIT_GAME);

	// 左腕モデルの初期化
	InitLeftArm();

	// 右腕モデルの初期化
	InitRightArm();

	// 頭モデルの初期化
	InitHead();

	// バレットゲージの初期化
	InitBulletGauge(INIT_GAME);

	// ライフゲージの初期化
	InitLifeGauge(INIT_GAME);

	// 子供モデルの初期化
	InitBall();

	// AIの初期化
	InitAi();

	// ポーズ画面の初期化
	InitPause(INIT_GAME);

	// バトルゲージの初期化
	InitBallGauge(INIT_GAME);

	// ゲージエフェクトの初期化
	InitGaugeEffect(INIT_GAME);

	// ゲーム遷移テロップの初期化
	InitGameCall(INIT_GAME);

	// チャージエフェクトの初期化
	InitChargeEffect(INIT_GAME);
}
