//=============================================================================
//
// �`���[�g���A������ [Tutorial.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_


// �}�N����`
#define TEXTURE_MAX					 (3)	 // �摜����
#define TEXTURE_TUTORIAL_SIZE_X01	 (1090)  // �e�N�X�`���T�C�Y
#define TEXTURE_TUTORIAL_SIZE_Y01	 (800)	 // 

#define TUTORIAL_POS_X01	 (400)	 // ����X���W 
#define TUTORIAL_POS_Y01	 (150)	 // ����Y���W

#define TEXTURE_TUTORIAL_SIZE_X02	 (200)	 // �e�N�X�`���T�C�Y
#define TEXTURE_TUTORIAL_SIZE_Y02	 (100)	 // 

#define TUTORIAL_POS_X02	 (790)	 // ����X���W 
#define TUTORIAL_POS_Y02	 (960)	 // ����Y���W

#define TEXTURE_TUTORIAL_SIZE_X03	 (75)	 // �e�N�X�`���T�C�Y
#define TEXTURE_TUTORIAL_SIZE_Y03	 (75)	 // 

#define TUTORIAL_POS_X03	 (1010)	 // ����X���W 
#define TUTORIAL_POS_Y03	 (970)	 // ����Y���W

#define	NUM_TUTORIAL				 (2)	 // �|���S����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

#endif
