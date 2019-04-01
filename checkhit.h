//=============================================================================
//
// �Փ˔��菈�� [checkhit.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _CHECKHIT_H_
#define _CHECKHIT_H_

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);
bool CheckHitRay(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vec, float size);
bool CheckHitPolygon(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);
D3DXVECTOR3 WallShear(D3DXVECTOR3 pos, D3DXVECTOR3 normal, int index);
D3DXVECTOR3 ReflectVector(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 normal);
D3DXVECTOR3 GetP3(void);
D3DXVECTOR3 GetNormal(void);

#endif