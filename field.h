//=============================================================================
//
// 処理 [field.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#define FIELD_SIZE_X	(400.0f)
#define FIELD_SIZE_Y	(0.0f)
#define FIELD_SIZE_Z	(150.0f)					

typedef struct {

	D3DXMATRIX				mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3				pos;				// 現在の位置
	D3DXVECTOR3				rot;				// 現在の向き
	int						scanData;			//

}FIELD;

//typedef struct
//{
//	int			nextTexType;	// テクスチャーのタイプ
//	int			scanResult;		// データの読み込み
//	int			nextIndex;		// POPするOBJのアドレス番号
//	float		nextPopObj;	// POPするの座標
//	D3DXVECTOR3 nextSize;		// POPする初期座標
//	D3DXVECTOR3 nextNor;
//
//}FIELDDATA;

enum {

	BOLCK_TEX,
	IRON_TEX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitField(int type);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
void InitMakeVertex(void);
bool HitCheckField(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);
FIELD *GetField(int fno);
void SetMakeField(void);

#endif
