//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_



//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	TEXTURE_BUTTON		("data/TEXTURE/button_a.png")	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TITLE_POS_X					(550)			// �^�C�g���̕\���ʒu
#define	TITLE_POS_Y					(380)			// �^�C�g���̕\���ʒu
#define	TITLE_SIZE_X				(750)			// �^�C�g���̕�
#define	TITLE_SIZE_Y				(310)			// �^�C�g���̍���

#define	TITLE_LOGO_POS_X			(840)			// �^�C�g�����S�̕\���ʒu
#define	TITLE_LOGO_POS_Y			(700)			// �^�C�g�����S�̕\���ʒu
#define	TITLE_LOGO_SIZE_X			(200)			// �^�C�g�����S�̕�
#define	TITLE_LOGO_SIZE_Y			(150)			// �^�C�g�����S�̍���
#define SET_LOGO					(75)

#define TITLE_LOGO_PUSHKEY_POS_X01	(710)			// �J�ڃL�[�̕\���ʒu
#define TITLE_LOGO_PUSHKEY_POS_Y01	(625)			//
#define TITLE_LOGO_PUSHKEY_SIZE_X01	(300)			// �傫��
#define TITLE_LOGO_PUSHKEY_SIZE_Y01	(100)			//

#define TITLE_LOGO_PUSHKEY_POS_X02	(1055)			// �J�ڃL�[�̕\���ʒu
#define TITLE_LOGO_PUSHKEY_POS_Y02	(645)			//
#define TITLE_LOGO_PUSHKEY_SIZE_X02	(70)			// �傫��
#define TITLE_LOGO_PUSHKEY_SIZE_Y02	(70)			//

#define PUSHKEY_MAX					(2)				// �J�ڃL�[�̍ő�\����

#define	TITLE_SELECT_POS_X	(770)					// �^�C�g�����S�̕\���ʒu
#define	TITLE_SELECT_POS_Y	(745)					// �^�C�g�����S�̕\���ʒu
#define	TITLE_SELECT_SIZE_X	(60)					// �^�C�g�����S�̕�
#define	TITLE_SELECT_SIZE_Y	(60)					// �^�C�g�����S�̍���
#define SELECT_MOVE			(75)					// �Z���N�g�̈ړ��l

#define NUM_TITLE			(2)
#define LOGO_MAX			(5)
#define SELECT_NPC			(0)
#define SELECT_PVP			(1)
#define SELECT_EXIT			(2)

enum {

	ENTHER,
	BUTTON,
	NCP,
	PVP,
	EXIT,
	SELECT,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(int type);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
LPDIRECTSOUNDBUFFER8 *GetTitleSound(void);

#endif
