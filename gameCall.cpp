//=============================================================================
//
// タイトル画面処理 [gameCall.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "gameCall.h"
#include "input.h"
#include "player.h"
#include "ai.h"
#include "child.h"
#include "time.h"
#include "result.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGOSPACE_ON			(1)		//
#define LOGOSPACE_OFF			(0)		//
#define ON_COUNT				(70)	//
#define OFF_COUNT				(30)	//
#define READY_SHOW_FRAME		(120)	// READYテロップ表示フレーム数
#define GO_SHOW_FRAME			(60)	// GOテッロプ表示フレーム数
#define FINISH_SHOW_FRAME		(120)	// FINISHテロップ表示フレーム数


#define	TEXTURE_READY			("data/TEXTURE/ready_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_GO				("data/TEXTURE/go_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_FINISH			("data/TEXTURE/finish_logo.png")	// 読み込むテクスチャファイル名
#define TEXTURE_MAX				(3)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGameCall(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureGameCall[TEXTURE_MAX];	// テクスチャへのポインタ


VERTEX_2D				vertexWkGameCall[NUM_VERTEX];					// 頂点情報格納ワーク
static int				CntFrame;
static int				TextureNum;
LPDIRECTSOUNDBUFFER8	ReadySE = NULL;	// 選択ロゴのSE
LPDIRECTSOUNDBUFFER8	FinishSE = NULL;	// 選択ロゴのSE

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGameCall(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	CntFrame = 0;			// 状態変化のフレームカウントを初期化
	TextureNum = READY;		// テクスチャー番号を初期化

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
	FinishSE = LoadSound(SE_FINISH);
	ReadySE = LoadSound(SE_STARTCALL);

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
	{
		case READY:
			if (CntFrame % READY_SHOW_FRAME == 0)
			{
				TextureNum = GO;
				CntFrame = 0;
			}
			break;

		case GO:
			if (CntFrame % GO_SHOW_FRAME == 0)
			{
				SetStage(START);
				TextureNum = FINISH;
				CntFrame = 0;
			}
			break;

		case FINISH:
			if (CntFrame % FINISH_SHOW_FRAME == 0)
			{
				ComprareChild();

				CntFrame = 0;
			}

			break;

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
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAMECALL, vertexWkGameCall, sizeof(VERTEX_2D));
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
	vertexWkGameCall[0].vtx = D3DXVECTOR3(GAMECALL_POS_X, GAMECALL_POS_Y, 0.0f);
	vertexWkGameCall[1].vtx = D3DXVECTOR3(GAMECALL_POS_X + GAMECALL_SIZE_X, GAMECALL_POS_Y, 0.0f);
	vertexWkGameCall[2].vtx = D3DXVECTOR3(GAMECALL_POS_X, GAMECALL_POS_Y + GAMECALL_SIZE_Y, 0.0f);
	vertexWkGameCall[3].vtx = D3DXVECTOR3(GAMECALL_POS_X + GAMECALL_SIZE_X, GAMECALL_POS_Y + GAMECALL_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWkGameCall[0].rhw =
		vertexWkGameCall[1].rhw =
		vertexWkGameCall[2].rhw =
		vertexWkGameCall[3].rhw = 1.0f;

	// 反射光の設定
	vertexWkGameCall[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkGameCall[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkGameCall[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkGameCall[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWkGameCall[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkGameCall[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkGameCall[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkGameCall[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


void SetGameCallTextureNum(int num)
{
	TextureNum = num;
}