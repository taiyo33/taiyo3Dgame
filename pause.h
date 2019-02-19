//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_



//*****************************************************************************
// �}�N����`
//*****************************************************************************


#define	PAUSE_POS_X					(370)			// �^�C�g���̕\���ʒu
#define	PAUSE_POS_Y					(300)			// �^�C�g���̕\���ʒu
#define	PAUSE_SIZE_X				(1120)			// �^�C�g���̕�
#define	PAUSE_SIZE_Y				(520)			// �^�C�g���̍���

#define	PAUSE_LOGO_POS_X			(840)			// �^�C�g�����S�̕\���ʒu
#define	PAUSE_LOGO_POS_Y			(400)			// �^�C�g�����S�̕\���ʒu
#define	PAUSE_LOGO_SIZE_X			(200)			// �^�C�g�����S�̕�
#define	PAUSE_LOGO_SIZE_Y			(150)			// �^�C�g�����S�̍���
#define SET_LOGO					(75)

#define PAUSE_LOGO_PUSHKEY_POS_X01	(710)			// �J�ڃL�[�̕\���ʒu
#define PAUSE_LOGO_PUSHKEY_POS_Y01	(625)			//
#define PAUSE_LOGO_PUSHKEY_SIZE_X01	(300)			// �傫��
#define PAUSE_LOGO_PUSHKEY_SIZE_Y01	(100)			//

#define PAUSE_LOGO_PUSHKEY_POS_X02	(1055)			// �J�ڃL�[�̕\���ʒu
#define PAUSE_LOGO_PUSHKEY_POS_Y02	(645)			//
#define PAUSE_LOGO_PUSHKEY_SIZE_X02	(70)			// �傫��
#define PAUSE_LOGO_PUSHKEY_SIZE_Y02	(70)			//

#define PUSHKEY_MAX					(2)				// �J�ڃL�[�̍ő�\����

#define	PAUSE_SELECT_POS_X	(770)					// �^�C�g�����S�̕\���ʒu
#define	PAUSE_SELECT_POS_Y	(445)					// �^�C�g�����S�̕\���ʒu
#define	PAUSE_SELECT_SIZE_X	(60)					// �^�C�g�����S�̕�
#define	PAUSE_SELECT_SIZE_Y	(60)					// �^�C�g�����S�̍���
#define SELECT_MOVE			(75)					// �Z���N�g�̈ړ��l

#define NUM_PAUSE			(2)
#define SELECT_RESTART		(0)
#define SELECT_REOPNE		(1)
#define SELECT_RETITLE		(2)

enum {

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
