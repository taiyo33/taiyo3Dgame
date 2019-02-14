//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _GAMECALL_H_
#define _GAMECALL_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************


#define	GAMECALL_POS_X		(400)			// �^�C�g���̕\���ʒu
#define	GAMECALL_POS_Y		(250)			// �^�C�g���̕\���ʒu
#define	GAMECALL_SIZE_X		(500)			// �^�C�g���̕�
#define	GAMECALL_SIZE_Y		(300)			// �^�C�g���̍���

#define NUM_GAMECALL		(2)

enum {

	READY,
	GO,
	FINISH,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGameCall(int type);
void UninitGameCall(void);
void UpdateGameCall(void);
void DrawGameCall(void);
void SetGameCallTextureNum(int num);

#endif
