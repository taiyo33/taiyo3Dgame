//=============================================================================
//
// �u���b�N���� [block.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define	BLOCK_MAX						(146)	// �u���b�N�̍ő吔
#define BLOCK_VTX_MAX					(122)	// �|���S������
#define BLOCK_NUM_WALL					(106)	// �O�g�u���b�N�̐�


#define BLOCK_NUM_CORNER				(4)		// �S�p
#define BLOCK_NUM_LEFTSIDE				(24)	// �|���S���̃i���o�����O
#define BLOCK_NUM_NEARSIDE				(53)	// 
#define BLOCK_NUM_RIGHTSIDE				(77)	// 
#define BLOCK_NUM_FEARSIDE				(106)	// 
#define BLOCK_NUM_LEFTSIDE_OBJ			(116)	// ��Q���u���b�N�̃i���o�����O
#define BLOCK_NUM_NEARSIDE_OBJ			(126)	// 
#define BLOCK_NUM_RIGHTSIDE_OBJ			(136)	// 
#define BLOCK_NUM_FEARSIDE_OBJ			(146)	// 
#define BLOCK_LEFT_OUTSIDE_OBJ_VTX		(108)	// �|���S���̃i���o�����O
#define BLOCK_NEAR_OUTSIDE_OBJ_VTX		(110)	// �|���S���̃i���o�����O
#define BLOCK_RIGHT_OUTSIDE_OBJ_VTX		(112)	// �|���S���̃i���o�����O
#define BLOCK_FEAR_OUTSIDE_OBJ_VTX		(114)	// �|���S���̃i���o�����O
#define BLOCK_LEFT_INSIDE_OBJ_VTX		(116)	// �|���S���̃i���o�����O
#define BLOCK_NEAR_INSIDE_OBJ_VTX		(118)	// �|���S���̃i���o�����O
#define BLOCK_RIGHT_INSIDE_OBJ_VTX		(120)	// �|���S���̃i���o�����O
#define BLOCK_FEAR_INSIDE_OBJ_VTX		(122)	// �|���S���̃i���o�����O



typedef struct {
	
	D3DXVECTOR3					pos;		// ���݂̈ʒu
	int							life;		// �u���b�N�̎���
	int							textureNum; // �e�N�X�`���[�̔ԍ�
	bool						use;		// �g�p���

}BLOCK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBlock(int type);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

BLOCK *GetBlock(int bno);
void BlockDamageManager(int bno);
bool HitCheckBlock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, int max);
bool HitCheckCornerBlock(D3DXVECTOR3 pos0);

#endif
