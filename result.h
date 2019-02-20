//=============================================================================
//
// ���U���g���� [result.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_


// �}�N����`
#define TEXTURE_WINNER_SIZE_X	(300) // �e�N�X�`���T�C�Y
#define TEXTURE_WINNER_SIZE_Y	(150) // ����

#define TEXTURE_ICON_SIZE_X		(200) // �e�N�X�`���T�C�Y
#define TEXTURE_ICON_SIZE_Y		(200) // ����

#define TEXTURE_WINNER_POS_X	(690)
#define TEXTURE_WINNER_POS_Y	(450)
#define TEXTURE_ICON_POS_X		(970)
#define TEXTURE_ICON_POS_Y		(425)

#define	NUM_RESULT						(2)		// �|���S����
#define RESULT_MAX						(2)		// ���U���g�̎��

enum {
	CLEAR,
	GAMEOVER
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(int type);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(int pno0, int pno1);


#endif
