//=============================================================================
//
// �|�[�Y��ʏ��� [pause.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PAUSE_POS_X					(370)	// �|�[�Y�̕\���ʒu
#define	PAUSE_POS_Y					(300)	// �|�[�Y�̕\���ʒu
#define	PAUSE_SIZE_X				(1120)	// �|�[�Y�̕�
#define	PAUSE_SIZE_Y				(520)	// �|�[�Y�̍���

#define	PAUSE_LOGO_POS_X			(840)	// �|�[�Y���S�̕\���ʒu
#define	PAUSE_LOGO_POS_Y			(400)	// �|�[�Y���S�̕\���ʒu
#define	PAUSE_LOGO_SIZE_X			(200)	// �|�[�Y���S�̕�
#define	PAUSE_LOGO_SIZE_Y			(150)	// �|�[�Y���S�̍���
#define SET_LOGO					(75)

#define PUSHKEY_MAX					(2)		// �J�ڃL�[�̍ő�\����

#define	PAUSE_SELECT_POS_X			(770)	// �|�[�Y���S�̕\���ʒu
#define	PAUSE_SELECT_POS_Y			(445)	// �|�[�Y���S�̕\���ʒu
#define	PAUSE_SELECT_SIZE_X			(60)	// �|�[�Y���S�̕�
#define	PAUSE_SELECT_SIZE_Y			(60)	// �|�[�Y���S�̍���
#define SELECT_MOVE					(75)	// �Z���N�g�̈ړ��l

#define SELECT_RESTART				(0)		// �ĊJ
#define SELECT_REOPNE				(1)		// ��蒼��
#define SELECT_RETITLE				(2)		// �^�C�g����

enum PAUSE_TEX{

	PAUSE,
	RESTART,
	REOPNE,
	RETITLE,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPause(int type);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
//bool GetPause(void);
//bool SetPause(bool on);

#endif
