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

#define	TITLE_POS_X					(260)			// �^�C�g���̕\���ʒu
#define	TITLE_POS_Y					(230)			// �^�C�g���̕\���ʒu
#define	TITLE_SIZE_X				(750)			// �^�C�g���̕�
#define	TITLE_SIZE_Y				(310)			// �^�C�g���̍���

#define	TITLE_LOGO_POS_X			(550)			// �^�C�g�����S�̕\���ʒu
#define	TITLE_LOGO_POS_Y			(550)			// �^�C�g�����S�̕\���ʒu
#define	TITLE_LOGO_SIZE_X			(200)			// �^�C�g�����S�̕�
#define	TITLE_LOGO_SIZE_Y			(150)			// �^�C�g�����S�̍���
#define SET_LOGO					(50)

#define TITLE_LOGO_PUSHKEY_POS_X01	(420)			// �J�ڃL�[�̕\���ʒu
#define TITLE_LOGO_PUSHKEY_POS_Y01	(475)			//
#define TITLE_LOGO_PUSHKEY_SIZE_X01	(300)			// �傫��
#define TITLE_LOGO_PUSHKEY_SIZE_Y01	(100)			//

#define TITLE_LOGO_PUSHKEY_POS_X02	(745)			// �J�ڃL�[�̕\���ʒu
#define TITLE_LOGO_PUSHKEY_POS_Y02	(495)			//
#define TITLE_LOGO_PUSHKEY_SIZE_X02	(70)			// �傫��
#define TITLE_LOGO_PUSHKEY_SIZE_Y02	(70)			//

#define PUSHKEY_MAX					(2)				// �J�ڃL�[�̍ő�\����

#define	TITLE_SELECT_POS_X	(480)					// �^�C�g�����S�̕\���ʒu
#define	TITLE_SELECT_POS_Y	(595)					// �^�C�g�����S�̕\���ʒu
#define	TITLE_SELECT_SIZE_X	(60)					// �^�C�g�����S�̕�
#define	TITLE_SELECT_SIZE_Y	(60)					// �^�C�g�����S�̍���
#define SELECT_MOVE			(50)					// �Z���N�g�̈ړ��l

#define NUM_TITLE			(2)
#define LOGO_MAX			(5)
#define SELECT_START		(0)
#define SELECT_OPTION		(1)
#define SELECT_EXIT			(2)

enum {

	ENTHER,
	BUTTON,
	GAMESTART,
	OPTION,
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

#endif
