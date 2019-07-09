//=============================================================================
//
// ゲーム開始終了処理 [gameCall.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "gameCall.h"
#include "player.h"
#include "ai.h"
#include "ball.h"
#include "time.h"
#include "result.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define READY_SHOW_FRAME		(200)	// READYロゴ表示フレーム数
#define GO_SHOW_FRAME			(60)	// GOテッロプ表示フレーム数
#define FINISH_SHOW_FRAME		(120)	// FINISHロゴ表示フレーム数
#define STOP_FINISH_FRAME		(60)	// FINISHSE停止フレーム数

#define	TEXTURE_READY			("data/TEXTURE/ready_logo.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_GO				("data/TEXTURE/go_logo.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_FINISH			("data/TEXTURE/finish_logo.png")	// 読み込むテクスチャファイル名
#define TEXTURE_MAX				(3)		// テクスチャの枚数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGameCall(void);
void FinishGameCall(int frame);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureGameCall[TEXTURE_MAX];	// テクスチャへのポインタ


VERTEX_2D				VertexWkGameCall[NUM_VERTEX];	// 頂点情報格納ワーク
static int				CntFrame;						// フレームカウント
static int				TextureNum;						// 現在のテクスチャ番号
LPDIRECTSOUNDBUFFER8	ReadySE = NULL;					// 開始のSE
LPDIRECTSOUNDBUFFER8	FinishSE = NULL;				// フィニッシュのSE
LPDIRECTSOUNDBUFFER8	GameBGM01 = NULL;				// ゲーム内のBGM
LPDIRECTSOUNDBUFFER8	GameBGM03 = NULL;				// ゲーム内のBGM

//=============================================================================
// 初期化処理
// 引　数：int type(再初期化時の2数判定変数)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitGameCall(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	CntFrame = 0;			// 状態変化のフレームカウントを初期化
	TextureNum = READY;		// テクスチャー番号を初期化

	// 再初期化時は読み込まない
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_READY,					// ファイルの名前
			&D3DTextureGameCall[READY]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_GO,					// ファイルの名前
			&D3DTextureGameCall[GO]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_FINISH,					// ファイルの名前
			&D3DTextureGameCall[FINISH]);		// 読み込むメモリー
	}
	
	// 頂点情報の作成
	MakeVertexGameCall();
	
	// SEロード
	GameBGM01 = LoadSound(BGM_GAME01);	
	GameBGM03 = LoadSound(BGM_GAME03);
	ReadySE = LoadSound(SE_STARTCALL);
	FinishSE = LoadSound(SE_FINISH);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGameCall(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureGameCall[i] != NULL)
		{// テクスチャの開放
			D3DTextureGameCall[i]->Release();
			D3DTextureGameCall[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameCall(void)
{
	CntFrame++;

	// 表示時間
	switch (TextureNum)
	{	// ゲーム開始時のロゴ表示
		case READY:
			PlaySound(ReadySE, E_DS8_FLAG_NONE);
			if (CntFrame % READY_SHOW_FRAME == 0)
			{
				TextureNum = GO;
				CntFrame = 0;
			}
			break;
		case GO:
			if (CntFrame % GO_SHOW_FRAME == 0)
			{
				PlaySound(GameBGM01, E_DS8_FLAG_LOOP);
				SetStage(START);
				TextureNum = FINISH;
				CntFrame = 0;
			}
			break;

		// ゲーム終了時のロゴ表示
		case FINISH:
			FinishGameCall(CntFrame);
			break;

		// エラー警告
		default :
			assert("ゲームテッロプのエラー");
			break;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGameCall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// タイトルの背景描画
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureGameCall[TextureNum]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAMECALL, VertexWkGameCall, sizeof(VERTEX_2D));
	}

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGameCall(void)
{
	// 頂点座標の設定
	VertexWkGameCall[0].vtx = D3DXVECTOR3(GAMECALL_POS_X, GAMECALL_POS_Y, 0.0f);
	VertexWkGameCall[1].vtx = D3DXVECTOR3(GAMECALL_POS_X + GAMECALL_SIZE_X, GAMECALL_POS_Y, 0.0f);
	VertexWkGameCall[2].vtx = D3DXVECTOR3(GAMECALL_POS_X, GAMECALL_POS_Y + GAMECALL_SIZE_Y, 0.0f);
	VertexWkGameCall[3].vtx = D3DXVECTOR3(GAMECALL_POS_X + GAMECALL_SIZE_X, GAMECALL_POS_Y + GAMECALL_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	VertexWkGameCall[0].rhw =
		VertexWkGameCall[1].rhw =
		VertexWkGameCall[2].rhw =
		VertexWkGameCall[3].rhw = 1.0f;

	// 反射光の設定
	VertexWkGameCall[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGameCall[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGameCall[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkGameCall[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkGameCall[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkGameCall[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkGameCall[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkGameCall[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//===============================================================================
// フィニッシュ時のロゴ変更処理
// 引　数：int frame(現在のフレーム数)
// 戻り値：な　し
//===============================================================================
void FinishGameCall(int frame)
{
	// BGMの停止
	if (frame == 1)
	{
		StopSound(GameBGM01);
		StopSound(GetGameBGM02());
		PlaySound(FinishSE, E_DS8_FLAG_NONE);
	}

	// SEの停止
	else if (frame % STOP_FINISH_FRAME == 0)
	{
		StopSound(FinishSE);
	}

	// 120フレームで勝敗判定へ
	if (frame % FINISH_SHOW_FRAME == 0)
	{
		PLAYER *player = GetPlayer(0);
		PlaySound(GameBGM03, E_DS8_FLAG_NONE);
		
		// ライフによる判定
		if (player[P1].life <= 0)
		{
			player[P2].npc == false ? SetResult(P2,P2) : SetResult(P2,NPC);
		}
		else if (player[P2].life <= 0)
		{
			SetResult(P1,P1);
		}

		// ボール数による判定
		ComprareBall();

		// フレームを初期化
		frame = 0;
	}
}

//===============================================================================
// ロゴのテクスチャ変更処理
// 引　数：int num(テクスチャ番号)
// 戻り値：な　し
//===============================================================================
void SetGameCallTextureNum(int num)
{
	TextureNum = num;
}

//===============================================================================
// ゲーム内BGM01の状態取得
// 引　数：な　し
// 戻り値：な　し
//===============================================================================
LPDIRECTSOUNDBUFFER8 GetGameBGM01(void)
{
	return GameBGM01;
}

//===============================================================================
// ゲーム内BGM03の状態取得
// 引　数：な　し
// 戻り値：な　し
//===============================================================================
LPDIRECTSOUNDBUFFER8 GetGameBGM03(void)
{
	return GameBGM03;
}
