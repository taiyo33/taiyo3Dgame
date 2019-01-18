//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : GP11A_341_22_�c�����z GP11A341_22_�c�����z
//
//=============================================================================
#include "effect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EFFECT		"data/TEXTURE/handgun_Fire.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EFFECT_SIZE_X		(5.0f)							// �r���{�[�h�̕�
#define	EFFECT_SIZE_Y		(5.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_EFFECT	(0.30f)							// �ړ����x
#define	VALUE_JUMP				(10.0f)							// �W�����v��
#define	VALUE_GRAVITY			(0.45f)							// �d��
#define	RATE_REGIST				(0.075f)						// ��R�W��
#define	RATE_REFRECT			(-0.90f)						// ���ˌW��
#define EFFECT_MAX				(24)
#define MAX_EFFECT				(256)
#define DEL_TIME				(10)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int Index, float fSizeX, float fSizeY);
void SetDiffuseEffect(int Index, float val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

static int				cnt_frame;
static float			Alpha;
static float			dif_mi[EFFECT_MAX];
EFFECT					effectWk[EFFECT_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *effect = &effectWk[0];

	// ���_���̍쐬
	MakeVertexEffect(pDevice);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EFFECT,					// �t�@�C���̖��O
			&g_pD3DTextureEffect);			// �ǂݍ��ރ������[
	}

	for (int i = 0; i < EFFECT_MAX ; i++)
	{
		effect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		effect[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect[i].time = DEL_TIME;
		dif_mi[i] = INIT_ALPHA;

	}

	Alpha = 0x0;
	cnt_frame = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	if(g_pD3DTextureEffect != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect = NULL;
	}

	if(g_pD3DVtxBuffEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffEffect->Release();
		g_pD3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	EFFECT *effect = &effectWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < EFFECT_MAX; i++)
	{	
		if (effect[i].bUse == true)
		{
			// �ʒu�𒲐�
			effect[i].pos = player->pos + D3DXVECTOR3(0.0f, 3.0f, 0.0f);
			
			// �X�P�[���̊g��
			if (effect[i].time % 4 == 0)
			{
				effect[i].scl += D3DXVECTOR3(0.3f, 0.3f, 0.3f);
			}
			
			dif_mi[i] -= 0.01f;		// ���߂̒l
			
			effect[i].time--;	// �������Ԃ��f�N�������g

			// ���Ŏ��ԂɂȂ��������
			if (effect[i].time % DEL_TIME == 0)
			{
				dif_mi[i] = INIT_ALPHA;
				effect[i].bUse = false;
				effect[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	EFFECT *effect = &effectWk[0];
	
	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		SetDiffuseEffect(i, dif_mi[i]);
	
		// ���C���e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
		// �ʏ�u�����h
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		if (effect[i].bUse)
		{
			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&effect[i].mtxWorld);

			// �|���S���𐳖ʂɌ�����
			effect[i].mtxWorld._11 = mtxView._11;
			effect[i].mtxWorld._12 = mtxView._21;
			effect[i].mtxWorld._13 = mtxView._31;
			effect[i].mtxWorld._21 = mtxView._12;
			effect[i].mtxWorld._22 = mtxView._22;
			effect[i].mtxWorld._23 = mtxView._32;
			effect[i].mtxWorld._31 = mtxView._13;
			effect[i].mtxWorld._32 = mtxView._23;
			effect[i].mtxWorld._33 = mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, effect[i].scl.x,
				effect[i].scl.y,
				effect[i].scl.z);
			D3DXMatrixMultiply(&effect[i].mtxWorld,
				&effect[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, effect[i].pos.x,
				effect[i].pos.y,
				effect[i].pos.z);
			D3DXMatrixMultiply(&effect[i].mtxWorld,
				&effect[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &effect[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureEffect);

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
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffEffect,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < MAX_EFFECT; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEffect(int Index, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//============================================================================
//
//============================================================================
void SetDiffuseEffect(int Index, float val)
{	
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);
		
		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = 
		pVtx[1].diffuse = 
		pVtx[2].diffuse = 
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//==========================================================================
//
//==========================================================================
bool SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest)
{
	EFFECT *effect = &effectWk[0];
	CAMERA *camera = GetCamera();
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < EFFECT_MAX ; i++)
	{
		if (!effect[i].bUse)
		{
			effect[i].bUse = true;	// �g�p��
			effect[i].pos.x = pos.x + sinf(rot.y) * Dest;
			effect[i].pos.z = pos.z + cosf(rot.y) * Dest;
			effect[i].pos.y = pos.y;
			SetVertexEffect(i, 5.0f, 5.0f);

			return true;
		}
	}
	return false;
}