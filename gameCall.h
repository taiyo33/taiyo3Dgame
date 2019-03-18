//=============================================================================
//
// �Q�[���̊J�n�I������ [gameCall.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _GAMECALL_H_
#define _GAMECALL_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	GAMECALL_POS_X		(690)			// �\��X�ʒu
#define	GAMECALL_POS_Y		(350)			// �\��Y�ʒu
#define	GAMECALL_SIZE_X		(500)			// ��
#define	GAMECALL_SIZE_Y		(350)			// ����
#define NUM_GAMECALL		(2)				// �|���S����

enum GAMECALL{

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
LPDIRECTSOUNDBUFFER8 GetGameBGM01(void);
LPDIRECTSOUNDBUFFER8 GetGameBGM03(void);

#endif
