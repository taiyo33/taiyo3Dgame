//=============================================================================
//
// �{�[�����ŃG�t�F�N�g���� [explosion.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "explosion.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EXPLOSION		"data/TEXTURE/explosion000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EXPLOSION_SIZE			(50.0f)							// �r���{�[�h�̕�
#define MAX_EXPLOSION			(60)							// �ő吔
#define DEL_TIME				(60)							// ���Ŏ���
#define TEXTURE_PATTERN_X		(8)								// �e�N�X�`���[������X����
#define TEXTURE_PATTERN_Y		(2)								// �e�N�X�`���[������Y����
#define ANIM_TIME				(8)								// �A�j���[�V�����̊Ԋu
#define ANIM_STOP				(7)								// �A�j���[�V�����̒�~
#define ANIM_PATTERN_NUM		(TEXTURE_PATTERN_X * TEXTURE_PATTERN_Y)	// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void SetVertexExplosion(int index, float Size);
void SetDiffuseExplosion(int index, float val);
void SetTextureExplosion(int index, int cntPattern);
void TextureAnim(int index);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureExplosion = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffExplosion = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

EXPLOSION				ExplosionWk[MAX_EXPLOSION]; 
//=============================================================================
// ����������
// ���@���Fint type(�ď���������2������ϐ�)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitExplosion(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EXPLOSION *explosion = &ExplosionWk[0];

	// ���_���̍쐬
	MakeVertexExplosion(pDevice);

	// �ď��������͓ǂݍ��܂Ȃ�
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EXPLOSION,					// �t�@�C���̖��O
			&D3DTextureExplosion);			// �ǂݍ��ރ������[
	}

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		explosion[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏�����
		explosion[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �傫���̏�����
		explosion[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]�ʂ̏�����
		explosion[i].cntAnim = 0;							// �A�j���[�V�����J�E���g�̏�����
		explosion[i].patternAnim = 0;						// �A�j���[�V�����p�^�[���̏�����
	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitExplosion(void)
{
	if (D3DTextureExplosion != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureExplosion->Release();
		D3DTextureExplosion = NULL;
	}

	if (D3DVtxBuffExplosion != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffExplosion->Release();
		D3DVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateExplosion(void)
{
	EXPLOSION *explosion = &ExplosionWk[0];

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (explosion[i].use)
		{
			TextureAnim(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX *mtxView, mtxScale, mtxRot, mtxTranslate;
	EXPLOSION *explosion = &ExplosionWk[0];

	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 254);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		// ���C���e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (explosion[i].use)
		{
			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&explosion[i].mtxWorld);

			// �|���S���𐳖ʂɌ�����
			explosion[i].mtxWorld._11 = mtxView->_11;
			explosion[i].mtxWorld._12 = mtxView->_21;
			explosion[i].mtxWorld._13 = mtxView->_31;
			explosion[i].mtxWorld._21 = mtxView->_12;
			explosion[i].mtxWorld._22 = mtxView->_22;
			explosion[i].mtxWorld._23 = mtxView->_32;
			explosion[i].mtxWorld._31 = mtxView->_13;
			explosion[i].mtxWorld._32 = mtxView->_23;
			explosion[i].mtxWorld._33 = mtxView->_33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, explosion[i].scl.x, explosion[i].scl.y, explosion[i].scl.z);
			D3DXMatrixMultiply(&explosion[i].mtxWorld, &explosion[i].mtxWorld, &mtxScale);
			
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, explosion[i].rot.y, explosion[i].rot.x, explosion[i].rot.z);
			D3DXMatrixMultiply(&explosion[i].mtxWorld, &explosion[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, explosion[i].pos.x, explosion[i].pos.y, explosion[i].pos.z);
			D3DXMatrixMultiply(&explosion[i].mtxWorld, &explosion[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &explosion[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, D3DVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTextureExplosion);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);

	// Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EXPLOSION,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffExplosion,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	EXPLOSION *explosion = &ExplosionWk[0];
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EXPLOSION; i++, pVtx += 4)
	{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-EXPLOSION_SIZE / 2, EXPLOSION_SIZE / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(EXPLOSION_SIZE / 2, EXPLOSION_SIZE / 2, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		SetTextureExplosion(i, explosion[i].cntAnim);
	}

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffExplosion->Unlock();

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Fint index(�����G�t�F�N�g�̃A�h���X�ԍ�), float size(�e�N�X�`���[�̃T�C�Y)
// �߂�l�F�Ȃ�
//=============================================================================
void SetVertexExplosion(int index, float Size)
{
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (index * 4);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-Size / 2, -Size / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(-Size / 2, Size / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(Size / 2, -Size / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(Size / 2, Size / 2, 0.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffExplosion->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// ���@���Fintindex(�����G�t�F�N�g�̃A�h���X�ԍ�), 
//		 �@int cntPatten(�A�j���[�V�����̃J�E���g�p�^�[��)
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureExplosion(int index, int cntPattern)
{
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += (index * 4);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(cntPattern * 0.125f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((cntPattern + 1) * 0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(cntPattern * 0.125f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((cntPattern + 1) * 0.125f, 1.0f);
	
	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffExplosion->Unlock();
}


//============================================================================
// �A���t�@�l�̐ݒ�
// ���@���F�@int index(�A�h���X�ԍ�), flaot val(�A���t�@�l)
// �߂�l�G�@�Ȃ�
//============================================================================
void SetDiffuseExplosion(int index, float val)
{
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (index * 4);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffExplosion->Unlock();
}

//==========================================================================
// �{�[�����ŃG�t�F�N�g�̐ݒu
// ���@���FD3DXVECTOR3 pos(�ʒu), D3DXVECTOR3 rot(��]), flaot Dest(����)
// �߂�l�Fbool�^�@�g�p���Ȃ�false , ���g�p�Ȃ�true
//==========================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest)
{
	EXPLOSION *explosion = &ExplosionWk[0];		// �A�h���X���擾

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		// ���g�p�Ȃ�
		if (!explosion[i].use)
		{
			explosion[i].use = true;	 // �g�p��
			explosion[i].pos.x = pos.x + sinf(rot.y) * Dest;		// �ʒu����
			explosion[i].pos.z = pos.z + cosf(rot.y) * Dest;		//
			explosion[i].pos.y = pos.y;								//
			
			return;
		}
	}
}

//=================================================================================
// �e�N�X�`���[�̃A�j���[�V��������
// ���@���Fint index(�����G�t�F�N�g�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==================================================================================
void TextureAnim(int index)
{
	EXPLOSION *explosion = &ExplosionWk[0];

	explosion[index].cntAnim++;		// �A�j���[�V�����J�E���g�̍X�V 

	// �A�j���[�V����Wait�`�F�b�N
	if ((explosion[index].cntAnim % ANIM_TIME) == 0)
	{
		// �p�^�[���̐؂�ւ�
		explosion[index].patternAnim = (explosion[index].patternAnim + 1) % ANIM_PATTERN_NUM;
	}
	// �A�j���[�V�����̒�~
	if (explosion[index].patternAnim == ANIM_STOP)
	{
		explosion[index].use = false;
		explosion[index].patternAnim = 0;
	}

	SetVertexExplosion(index, EXPLOSION_SIZE);					// ���_�쐬
	SetTextureExplosion(index, explosion[index].patternAnim);	// �e�N�X�`���[���W�̌v�Z
}

////==========================================================================
//// �{�[�����ŃG�t�F�N�g�̌���
//// ���@���FD3DXVECTOR3 nor0(�{�[�����ł̖@��), D3DXVECTOR3 nor1(�|���S���̖@��), 
////		   D3DXVECTOR3 *rot(�{�[�����ł̉�]��)
//// �߂�l�F�ȁ@��
////==========================================================================
//void SetRotExplosion(D3DXVECTOR3 nor0, D3DXVECTOR3 nor1, D3DXVECTOR3 *rot)
//{
//	// �@����X�������Ȃ�
//	if (nor1.x != 0.0f)
//	{
//		nor1.x > 0.0f ? rot->y = D3DX_PI * 0.5f : rot->y = -D3DX_PI * 0.5f;
//		rot->x = 0.0f;
//	}
//	// �@����Z�������Ȃ�
//	else if (nor1.z != 0.0f)
//	{
//		nor1.z > 0.0f ? rot->y = 0.0f : rot->y = D3DX_PI;
//		rot->x = 0.0f;
//	}
//	// �@����Y�������Ȃ�
//	else if (nor1.y != 0.0f)
//	{
//		nor1.y > 0.0f ? rot->x = -D3DX_PI * 0.5f : rot->x = D3DX_PI * 0.5f;
//	}
//
//}

////===================================================================================
//// �����G�t�F�N�g�̏��ŏ���
//// ���@���Fint eno(�����G�t�F�N�g�̃A�h���X�ԍ�)
//// �߂�l�F�Ȃ�
////===================================================================================
//void DeleteExplosion(int eno)
//{
//	EXPLOSION *explosion = &ExplosionWk[0];
//
//	dif_mi[eno] -= 0.01f;	// ���߂̒l
//
//	explosion[eno].time--;	// �������Ԃ��f�N�������g
//
//	// ���Ŏ��ԂɂȂ��������
//	if (explosion[eno].time % DEL_TIME == 0)
//	{
//		dif_mi[eno] = INIT_ALPHA;
//		explosion[eno].use = false;
//		explosion[eno].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//	}
//}