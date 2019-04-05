//=============================================================================
//
// �o���b�g�Q�[�W���� [bulletGauge.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _BULLETGAUGE_H_
#define _BULLETGAUGE_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_BULLETGAUGE_SIZE_X		 (200)	 // �e�N�X�`���T�C�Y
#define TEXTURE_BULLETGAUGE_SIZE_Y		 (150)	 // 

#define BULLETGAUGE_POS_X_01			 (10)	 // ����X���W 
#define BULLETGAUGE_POS_Y_01			 (220)	 // ����Y���W
#define BULLETGAUGE_POS_X_02			 (1720)	 // ����X���W 
#define BULLETGAUGE_POS_Y_02			 (220)	 // ����Y���W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletGauge(int type);
void UninitBulletGauge(void);
void UpdateBulletGauge(void);
void DrawBulletGauge(void);


#endif
