//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGOSPACE_ON				(1)		//
#define LOGOSPACE_OFF				(0)		//
#define ON_COUNT					(70)	//
#define OFF_COUNT					(30)	//
#define	TEXTURE_TITLE		("data/TEXTURE/title_logo01.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_SPACE		("data/TEXTURE/space.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_GAMESTART	("data/TEXTURE/gamestart.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_OPTION		("data/TEXTURE/option.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_EXIT		("data/TEXTURE/exit.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_SELECT		("data/TEXTURE/yajirusi.png")	// 読み込むテクスチャファイル名


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);
//void SetVertexSelect(int move);
void SetLogoTextureDiffuse(int ver);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureTitle = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureLogo[LOGO_MAX];	// テクスチャへのポインタ
//LPDIRECT3DTEXTURE9		D3DTextureSelect;			// テクスチャへのポインタ


VERTEX_2D				vertexWkTitle[NUM_VERTEX];					// 頂点情報格納ワーク
VERTEX_2D				vertexWkLogo[LOGO_MAX][NUM_VERTEX];		// 頂点情報格納ワーク
//VERTEX_2D				vertexWkSelect[NUM_VERTEX];				// 頂点情報格納ワーク
//static float			y_move;
//static int				SelectNum;
static int				cnt_frame;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// セレクトテクスチャの移動量
	//y_move = 0.0;
	cnt_frame = 0;

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_TITLE,					// ファイルの名前
			&D3DTextureTitle);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_SPACE,					// ファイルの名前
			&D3DTextureLogo[SPACE]);		// 読み込むメモリー

		//D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		//	TEXTURE_GAMESTART,				// ファイルの名前
		//	&D3DTextureLogo[GAMESTART]);	// 読み込むメモリー

		//D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		//	TEXTURE_OPTION,					// ファイルの名前
		//	&D3DTextureLogo[OPTION]);		// 読み込むメモリー

		//D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		//	TEXTURE_EXIT,					// ファイルの名前
		//	&D3DTextureLogo[EXIT]);			// 読み込むメモリー

		//D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		//	TEXTURE_SELECT,					// ファイルの名前
		//	&D3DTextureSelect);				// 読み込むメモリー
	}
	
	// 頂点情報の作成
	MakeVertexTitle();
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if(D3DTextureTitle != NULL)
	{// テクスチャの開放
		D3DTextureTitle->Release();
		D3DTextureTitle = NULL;
	}

	for (int i = 0; i < LOGO_MAX; i++)
	{
		if (D3DTextureLogo[i] != NULL)
		{// テクスチャの開放
			D3DTextureLogo[i]->Release();
			D3DTextureLogo[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{

	cnt_frame++;

	if (cnt_frame % OFF_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_OFF);
	}
	if (cnt_frame % ON_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_ON);
		cnt_frame = 0;
	}


	//// 下方向へセレクトを移動
	//if (GetKeyboardTrigger(DIK_DOWN)) 
	//{
	//	SelectNum = (SelectNum + 1) % 3;
	//	SetVertexSelect(SelectNum);
	//}
	//// 上方向のセレクトを移動
	//if (GetKeyboardTrigger(DIK_UP))
	//{
	//	SelectNum = (SelectNum + 2) % 3;
	//	SetVertexSelect(SelectNum);
	//}

	// ゲームへ遷移
	//if ((GetKeyboardTrigger(DIK_SPACE)) && (SelectNum == SELECT_START))
	if ((GetKeyboardTrigger(DIK_SPACE)))
	{
		SetStage(TUTORIAL);
	}
	else if (IsButtonPressed(0, BUTTON_A))
	{
		SetStage(TUTORIAL);
	}
	//// コンフィングへ遷移
	//else if ((GetKeyboardTrigger(DIK_SPACE)) && (SelectNum == SELECT_OPTION))
	//{
	//		// コンフィング
	//}
	//// 強制終了
	//else if ((GetKeyboardTrigger(DIK_SPACE)) && (SelectNum == SELECT_EXIT))
	//{
	//	exit(0);
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// タイトルの背景描画
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureTitle);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkTitle, sizeof(VERTEX_2D));
	}

	//// セレクトの描画
	//{
	//	// テクスチャの設定
	//	pDevice->SetTexture(0, D3DTextureSelect);

	//	// ポリゴンの描画
	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkSelect, sizeof(VERTEX_2D));
	//}

	// セレクトロゴの描画
	//for (int i = 0; i < LOGO_MAX; i++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureLogo[SPACE]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkLogo[SPACE], sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	float set = 0.0;
	// タイトル背景
	{
		// 頂点座標の設定
		vertexWkTitle[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
		vertexWkTitle[1].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y, 0.0f);
		vertexWkTitle[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);
		vertexWkTitle[3].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWkTitle[0].rhw =
		vertexWkTitle[1].rhw =
		vertexWkTitle[2].rhw =
		vertexWkTitle[3].rhw = 1.0f;

		// 反射光の設定
		vertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// セレクトロゴ
	//for (int i = 0; i < LOGO_MAX; i++, set += SET_LOGO)
	{
		// 頂点座標の設定
		vertexWkLogo[SPACE][0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + set, 0.0f);
		vertexWkLogo[SPACE][1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_SIZE_X, TITLE_LOGO_POS_Y + set, 0.0f);
		vertexWkLogo[SPACE][2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + TITLE_LOGO_SIZE_Y + set, 0.0f);
		vertexWkLogo[SPACE][3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_SIZE_X, TITLE_LOGO_POS_Y + TITLE_LOGO_SIZE_Y + set, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWkLogo[SPACE][0].rhw =
		vertexWkLogo[SPACE][1].rhw =
		vertexWkLogo[SPACE][2].rhw =
		vertexWkLogo[SPACE][3].rhw = 1.0f;

		// 反射光の設定
		vertexWkLogo[SPACE][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[SPACE][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[SPACE][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[SPACE][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWkLogo[SPACE][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkLogo[SPACE][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkLogo[SPACE][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkLogo[SPACE][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	//// セレクト
	//{
	//	// 頂点座標の設定
	//	vertexWkSelect[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y, 0.0f);
	//	vertexWkSelect[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y, 0.0f);
	//	vertexWkSelect[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y, 0.0f);
	//	vertexWkSelect[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y, 0.0f);

	//	// テクスチャのパースペクティブコレクト用
	//	vertexWkSelect[0].rhw =
	//	vertexWkSelect[1].rhw =
	//	vertexWkSelect[2].rhw =
	//	vertexWkSelect[3].rhw = 1.0f;

	//	// 反射光の設定
	//	vertexWkSelect[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	vertexWkSelect[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	vertexWkSelect[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	vertexWkSelect[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	// テクスチャ座標の設定
	//	vertexWkSelect[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	vertexWkSelect[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	vertexWkSelect[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	vertexWkSelect[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//}

	return S_OK;
}

//============================================================================
// セレクトの設置
// 引数：move（セレクトロゴの種類）
//============================================================================
//void SetVertexSelect(int move)
//{
//	// セレクトテクスチャの移動値
//	y_move = (SELECT_MOVE * (float) move);
//
//	// 頂点座標の設定
//	vertexWkSelect[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + y_move, 0.0f);
//	vertexWkSelect[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + y_move, 0.0f);
//	vertexWkSelect[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y + y_move, 0.0f);
//	vertexWkSelect[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y + y_move, 0.0f);
//}

void SetLogoTextureDiffuse(int ver) 
{
	// 反射光の設定
	vertexWkLogo[SPACE][0].diffuse = 
	vertexWkLogo[SPACE][1].diffuse = 
	vertexWkLogo[SPACE][2].diffuse = 
	vertexWkLogo[SPACE][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);
}