//=============================================================================
//
// 制限時間処理 [time.cpp]
// Author：GP11A341_22_田中太陽 
//=============================================================================
#include "main.h"
#include "time.h"
#include "input.h"
#include "child.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_TIME		("data/TEXTURE/number16x32.png")	// サンプル用画像
#define TEXTURE_TIME00_SIZE_X	(50)	// テクスチャサイズ
#define TEXTURE_TIME00_SIZE_Y	(70)	// 同上
#define TIME_POS_X				(630)		// ポリゴンの初期位置X
#define TIME_POS_Y				(90)		// 同上
#define TIME_MAX				(60)		// スコアの最大値
#define TIME_DIGIT				(2)			// 桁数
#define NUM_TIME				(2)
#define TIME_COUNT_FRAME		(60)		// 時間経過のフレーム数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTime(void);
void SetTextureTime(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTime = NULL;				// テクスチャへのポリゴン
VERTEX_2D				VertexWkTime[TIME_DIGIT][NUM_VERTEX];	// 頂点情報格納ワーク

D3DXVECTOR3				PosTime;					// ポリゴンの移動量
static int				CntFrame;
int						Time;						// 制限時間
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTime(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TIME,				// ファイルの名前
			&g_pD3DTextureTime);				// 読み込むメモリのポインタ
	}

	for (int i = 0; i < TIME_DIGIT; i++)
	{

		PosTime = D3DXVECTOR3((float)TIME_POS_X - 16.0f, (float)TIME_POS_Y, 0.0f);
		Time = TIME_MAX;
		CntFrame = 0;

		// 頂点情報の作成
		MakeVertexTime();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTime(void)
{
	if( g_pD3DTextureTime != NULL )
	{	// テクスチャの開放
		g_pD3DTextureTime->Release();
		g_pD3DTextureTime = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(void)
{
	// 毎フレーム実行される処理を記述する
	SetTextureTime();
	CntFrame++;

	if (Time == NULL) 
	{
		ComprareChild();
	}
	
	if(CntFrame % TIME_COUNT_FRAME == 0)
	{
		Time--;
		CntFrame = 0;
	}

	if (GetKeyboardTrigger(DIK_T))
	{
		Time = 3;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i, DigitMax = 1;


	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pD3DTextureTime );
	// 制限時間
	for (int j = 1; j < TIME_DIGIT; j++, DigitMax++)
	{
		if (Time < pow(10, j))
		{
			break;
		}
	}

	for( i = 0; i < DigitMax; i++ )
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TIME, VertexWkTime[i], sizeof(VERTEX_2D));
	}

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTime(void)
{
	int i;
	float haba_x = 30.0f;	// 数字の横幅
	
	// 桁数分処理する
	for( i = 0; i < TIME_DIGIT; i++ )
	{
		// 頂点座標の設定
		VertexWkTime[i][0].vtx.x = -haba_x * i + PosTime.x;
		VertexWkTime[i][0].vtx.y = PosTime.y;
		VertexWkTime[i][0].vtx.z = 0.0f;
		VertexWkTime[i][1].vtx.x = -haba_x * i + PosTime.x + TEXTURE_TIME00_SIZE_X;
		VertexWkTime[i][1].vtx.y = PosTime.y;
		VertexWkTime[i][1].vtx.z = 0.0f;
		VertexWkTime[i][2].vtx.x = -haba_x * i + PosTime.x;
		VertexWkTime[i][2].vtx.y = PosTime.y + TEXTURE_TIME00_SIZE_Y;
		VertexWkTime[i][2].vtx.z = 0.0f;
		VertexWkTime[i][3].vtx.x = -haba_x * i + PosTime.x + TEXTURE_TIME00_SIZE_X;
		VertexWkTime[i][3].vtx.y = PosTime.y + TEXTURE_TIME00_SIZE_Y;
		VertexWkTime[i][3].vtx.z = 0.0f;

		// rhwの設定
		VertexWkTime[i][0].rhw =
		VertexWkTime[i][1].rhw =
		VertexWkTime[i][2].rhw =
		VertexWkTime[i][3].rhw = 1.0f;

		// 反射光の設定
		VertexWkTime[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTime[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTime[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTime[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWkTime[i][0].tex = D3DXVECTOR2( 0.0f, 0.0f );
		VertexWkTime[i][1].tex = D3DXVECTOR2( 1.0f, 0.0f );
		VertexWkTime[i][2].tex = D3DXVECTOR2( 0.0f, 1.0f );
		VertexWkTime[i][3].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureTime(void)
{
	int i;
	int number = Time;
	
	for( i = 0; i < TIME_DIGIT; i++ )
	{
		// テクスチャ座標の設定
		float x = (float)(number % 10);
		VertexWkTime[i][0].tex = D3DXVECTOR2( 0.1f * x, 0.0f );
		VertexWkTime[i][1].tex = D3DXVECTOR2( 0.1f * ( x + 1 ),	 0.0f );
		VertexWkTime[i][2].tex = D3DXVECTOR2( 0.1f * x, 1.0f );
		VertexWkTime[i][3].tex = D3DXVECTOR2( 0.1f * ( x + 1 ),	 1.0f );
		number /= 10;
	}

}
//=============================================================================
// 制限時間データをセットする
// 引数:add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddTime( int add )
{
	Time += add;
	if( Time > TIME_MAX )
	{
		Time = TIME_MAX;
	}
	else if( Time < 0 )
	{
		Time = 0;
	}

}
