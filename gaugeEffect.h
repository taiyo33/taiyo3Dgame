//=============================================================================
//
// �Q�[�W�G�t�F�N�g���� [gaugeEffect.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _GAUGEEFFECT_H_
#define _GAUGEEFFECT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAUGEEFFECT_SIZE_X	 (200)	 // �e�N�X�`���T�C�Y
#define TEXTURE_GAUGEEFFECT_SIZE_Y	 (150)	 // 

#define GAUGEEFFECT_POS_X_01		 (428)	 // ����X���W 
#define GAUGEEFFECT_POS_Y_01		 (0)	 // ����Y���W
#define GAUGEEFFECT_POS_X_02		 (754)	 // 
#define GAUGEEFFECT_POS_Y_02		 (0)	 // 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGaugeEffect(int type);
void UninitGaugeEffect(void);
void UpdateGaugeEffect(void);
void DrawGaugeEffect(void);

#endif

