//=============================================================================
//
// スコア処理 [score.cpp]
// Author：GP11A341_22_田中太陽 
//=============================================================================
#include "main.h"
#include "score.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(void);
void SetTextureScore(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;		// テクスチャへのポリゴン
VERTEX_2D				VertexWkScore[SCORE_DIGIT][NUM_VERTEX];	// 頂点情報格納ワーク

D3DXVECTOR3				PosScore;					// ポリゴンの移動量

int						Score;						// スコア

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_SCORE,				// ファイルの名前
			&g_pD3DTextureScore);				// 読み込むメモリのポインタ
	}
	for (int i = 0; i < SCORE_DIGIT; i++)
	{

		PosScore = D3DXVECTOR3((float)SCORE_POS_X - 16.0f, (float)SCORE_POS_Y, 0.0f);
		Score = 0;

		// 頂点情報の作成
		MakeVertexScore();
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	if( g_pD3DTextureScore != NULL )
	{	// テクスチャの開放
		g_pD3DTextureScore->Release();
		g_pD3DTextureScore = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	// 毎フレーム実行される処理を記述する
	SetTextureScore();

	if (Score == 100) 
	{
		SetStage(RESULT);
	}

	if (GetKeyboardPress(DIK_L))
	{
		Score += 1;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i, DigitMax = 1;


	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pD3DTextureScore );
	// スコア
	for (int j = 1; j < SCORE_DIGIT; j++, DigitMax++)
	{
		if (Score < pow(10, j))
		{
			break;
		}
	}

	for( i = 0; i < DigitMax; i++ )
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SCORE, VertexWkScore[i], sizeof(VERTEX_2D));
	}

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(void)
{
	int i;
	float haba_x = 16.0f;	// 数字の横幅
	
	// 桁数分処理する
	for( i = 0; i < SCORE_DIGIT; i++ )
	{
		// 頂点座標の設定
		VertexWkScore[i][0].vtx.x = -haba_x * i + PosScore.x;
		VertexWkScore[i][0].vtx.y = PosScore.y;
		VertexWkScore[i][0].vtx.z = 0.0f;
		VertexWkScore[i][1].vtx.x = -haba_x * i + PosScore.x + TEXTURE_SCORE00_SIZE_X;
		VertexWkScore[i][1].vtx.y = PosScore.y;
		VertexWkScore[i][1].vtx.z = 0.0f;
		VertexWkScore[i][2].vtx.x = -haba_x * i + PosScore.x;
		VertexWkScore[i][2].vtx.y = PosScore.y + TEXTURE_SCORE00_SIZE_Y;
		VertexWkScore[i][2].vtx.z = 0.0f;
		VertexWkScore[i][3].vtx.x = -haba_x * i + PosScore.x + TEXTURE_SCORE00_SIZE_X;
		VertexWkScore[i][3].vtx.y = PosScore.y + TEXTURE_SCORE00_SIZE_Y;
		VertexWkScore[i][3].vtx.z = 0.0f;

		// rhwの設定
		VertexWkScore[i][0].rhw =
		VertexWkScore[i][1].rhw =
		VertexWkScore[i][2].rhw =
		VertexWkScore[i][3].rhw = 1.0f;

		// 反射光の設定
		VertexWkScore[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkScore[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkScore[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkScore[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWkScore[i][0].tex = D3DXVECTOR2( 0.0f, 0.0f );
		VertexWkScore[i][1].tex = D3DXVECTOR2( 1.0f, 0.0f );
		VertexWkScore[i][2].tex = D3DXVECTOR2( 0.0f, 1.0f );
		VertexWkScore[i][3].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureScore(void)
{
	int i;
	int number = Score;
	
	for( i = 0; i < SCORE_DIGIT; i++ )
	{
		// テクスチャ座標の設定
		float x = (float)(number % 10);
		VertexWkScore[i][0].tex = D3DXVECTOR2( 0.1f * x, 0.0f );
		VertexWkScore[i][1].tex = D3DXVECTOR2( 0.1f * ( x + 1 ),	 0.0f );
		VertexWkScore[i][2].tex = D3DXVECTOR2( 0.1f * x, 1.0f );
		VertexWkScore[i][3].tex = D3DXVECTOR2( 0.1f * ( x + 1 ),	 1.0f );
		number /= 10;
	}

}
//=============================================================================
// スコアデータをセットする
// 引数:add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddScore( int add )
{
	Score += add;
	if( Score > SCORE_MAX )
	{
		Score = SCORE_MAX;
	}
	else if( Score < 0 )
	{
		Score = 0;
	}

}
