//=============================================================================
//
// 衝突判定処理 [checkhit.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef ___CHECKHIT_H___
#define ___CHECKHIT_H___



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);
bool CheckHitRay(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vec, float size);
bool CheckHitPolygon(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);
D3DXVECTOR3 WallShear(D3DXVECTOR3 pos, D3DXVECTOR3 normal, int index);
D3DXVECTOR3 GetP3(void);
D3DXVECTOR3 GetNormal(void);
D3DXVECTOR3 ReflectVector(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 normal);

#endif