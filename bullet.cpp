//===============================================================================
//
// �o���b�g���� [bullet.cpp]
// Author : GP11A_341_22_�c�����z GP11A341_22_�c�����z
//
//===============================================================================
#include "bullet.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "field.h"
#include "checkhit.h"
#include "block.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLET		"data/TEXTURE/bullet001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLET_SIZE_X		(20.0f)							// �o���b�g�̕�
#define	BULLET_SIZE_Y		(20.0f)							// �o���b�g�̍���
#define	BULLET_SPEED		(10.0f)							// �ړ����x
#define DELET_TIME			(120)							// ���Ŏ���
#define BULLET_RADY_FRAME	(10)							// ���ˊԊu
#define BULLET_MASS			(5.0f)							// ����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(int Index, float fSizeX, float fSizeY);
bool DeleteBullet(int index,int bno);
void MoveBullet(int index, int bno);
D3DXVECTOR3 ReflectBullet(D3DXVECTOR3 pos, D3DXVECTOR3 normal, int index, int bno);
//void SetDiffuseBullet(int Index, float val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBullet = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
static float			dif_mi[BULLET_MAX];
static int				cntFrame;
BULLET					bulletWk[PLAYER_MAX];

//===============================================================================
// ����������
// ���@���Fint type(�ď�������2������l)
// �߂�l�FHRESULT�^
//===============================================================================
HRESULT InitBullet(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = &bulletWk[0];

	// ���_���̍쐬
	MakeVertexBullet(pDevice);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLET,			// �t�@�C���̖��O
			&g_pD3DTextureBullet);	// �ǂݍ��ރ������[
	}


	for (int i = 0; i < PLAYER_MAX; i++)
	{
		for (int j = 0; j < BULLET_MAX; j++)
		{	
			bullet[i].use[j] = false;							// �g�p��Ԃ�������
			bullet[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu��������
			bullet[i].rot[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]��������
			bullet[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �g�嗦��������
			bullet[i].mass[j] =
			bullet[i].time[j] = DELET_TIME;						// ������������
			dif_mi[i] = INIT_ALPHA;								// ���l�̏�����
		}
	}
	cntFrame = BULLET_RADY_FRAME;							// �t���[����������

	return S_OK;
}

//===============================================================================
// �I������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//===============================================================================
void UninitBullet(void)
{
	if (g_pD3DTextureBullet != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}

	if (g_pD3DVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffBullet->Release();
		g_pD3DVtxBuffBullet = NULL;
	}
}

//===============================================================================
// �X�V����
//===============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = &bulletWk[0];
	int i, j;
	cntFrame++;	// �t���[���J�E���g�̍X�V

	for (i = 0; i < PLAYER_MAX; i++)
	{
		for (j = 0; j < BULLET_SET_MAX; j++)
		{
			// �g�p���Ȃ��
			if (bullet[i].use[j])
			{
				// ���݈ʒu��ۑ�
				bullet[i].prevPos[j] = bullet[i].pos[j];

				// �ړ�����
				MoveBullet(i, j);

				if (!HitCheckBlock(bullet[i].pos[j] , bullet[i].prevPos[j]))
				{
					bullet[i].pos[j] = bullet[i].pos[j] + (ReflectBullet(bullet[i].pos[j], GetNormal(), i, j) * BULLET_SPEED);
				}

				// ���ŏ���
				bullet[i].use[j] = DeleteBullet(i, j);
			}
		}
	}
}

//===============================================================================
// �`�揈��
//===============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	BULLET *bullet = &bulletWk[0];

	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		for (int j = 0; j < BULLET_MAX; j++)
		{
			//// ���ߏ���
			//SetDiffuseBullet(i, dif_mi[i]);

			// ���C���e�B���O�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// �ʏ�u�����h
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			if (bullet[i].use[j])
			{
				// �r���[�}�g���b�N�X���擾
				mtxView = GetMtxView();

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&bullet[i].mtxWorld);

				// �|���S���𐳖ʂɌ�����
				bullet[i].mtxWorld._11 = mtxView._11;
				bullet[i].mtxWorld._12 = mtxView._21;
				bullet[i].mtxWorld._13 = mtxView._31;
				bullet[i].mtxWorld._21 = mtxView._12;
				bullet[i].mtxWorld._22 = mtxView._22;
				bullet[i].mtxWorld._23 = mtxView._32;
				bullet[i].mtxWorld._31 = mtxView._13;
				bullet[i].mtxWorld._32 = mtxView._23;
				bullet[i].mtxWorld._33 = mtxView._33;

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, bullet[i].scl[j].x,
					bullet[i].scl[j].y,
					bullet[i].scl[j].z);
				D3DXMatrixMultiply(&bullet[i].mtxWorld,
					&bullet[i].mtxWorld, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, bullet[i].pos[j].x,
					bullet[i].pos[j].y,
					bullet[i].pos[j].z);
				D3DXMatrixMultiply(&bullet[i].mtxWorld,
					&bullet[i].mtxWorld, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &bullet[i].mtxWorld);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, g_pD3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureBullet);

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
			}
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

//===============================================================================
// ���_���̍쐬
//===============================================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLET_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffBullet,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < BULLET_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			SetVertexBullet(i, BULLET_SIZE_X, BULLET_SIZE_Y);					// ���_���쐬

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
		g_pD3DVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//=================================================================================
// ���_���W�̐ݒ�
// ���@���Fint Index(b���b�g�̃A�h���X�ԍ�),float fSizeX(����),float fSizeY(�c��)
// �߂�l�F�Ȃ�
//=================================================================================
void SetVertexBullet(int Index, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBullet->Unlock();
	}
}


//=========================================================================
// �o���b�g�̃A�h���X���擾
// ���@���F�o���b�g�̃A�h���X�ԍ�
// �߂�l�FBULLET�\���̌^
//=========================================================================
BULLET *GetBullet(int bno)
{
	return &bulletWk[bno];
}

//=========================================================================
// �o���b�g�̐���
// ���@���FD3DXVECTOR3 pos[j](�ʒu)�AD3DXVECTOR3 rot(�p�x)�Afloat Dest(����)
// �߂�l�Fbool�^�@���g�p�̏ꍇ true�A�g�p���̏ꍇ false
//=========================================================================
bool SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest, int index)
{
	BULLET *bullet = &bulletWk[index];
	for (int i = 0; i < BULLET_SET_MAX; i++)
	{
		if (!bullet[index].use[i] && cntFrame > BULLET_RADY_FRAME)
		{
			bullet[index].use[i] = true;							// �g�p����
			bullet[index].pos[i].x = pos.x + cosf(rot.y) * Dest;	// �v���C���[�̈ʒu�ց@
			bullet[index].pos[i].z = pos.z + sinf(rot.y) * Dest;	//�@
			bullet[index].pos[i].y = pos.y;							//
			bullet[index].rot[i] = rot;								// ��]�ʂ���
			cntFrame = 0;											// �t���[���J�E���g��������
			SetVertexBullet(i, BULLET_SIZE_X, BULLET_SIZE_Y);		// ���_���쐬

			return true;
		}
	}

	return false;
}

//========================================================================
// �o���b�g�̈ړ�����
// ���@���Fint index(�o���b�g�̃A�h���X)
// �߂�l�F�ȁ@��
//========================================================================
void MoveBullet(int index, int bno)
{
	BULLET *bullet = &bulletWk[index];

	// �ړ���
	bullet[index].pos[bno].x += sinf(bullet[index].rot[bno].y) * BULLET_SPEED;
	bullet[index].pos[bno].y -= tanf(bullet[index].rot[bno].x) * BULLET_SPEED;
	bullet[index].pos[bno].z += cosf(bullet[index].rot[bno].y) * BULLET_SPEED;
}

//========================================================================
// �o���b�g�̉�ʓ��O���菈��
// ���@���Fint index(�g�o���b�g�̃A�h���X), int bno(�o���b�g�P�̂̃A�h���X)
// �߂�l�Fbool�^�@true�ł���Ή�ʓ��ɂ���Afalse�Ȃ�Ή�ʊO�ɂ���
//========================================================================
bool DeleteBullet(int index, int bno)
{
	BULLET *bullet = &bulletWk[index];
	
	if (bullet[index].pos[bno].x > SCREEN_WIDTH  / 2)	return false;
	if (bullet[index].pos[bno].x < -SCREEN_WIDTH / 2)	return false;
	if (bullet[index].pos[bno].z > SCREEN_HEIGHT / 2)	return false;
	if (bullet[index].pos[bno].z < -SCREEN_HEIGHT / 2)	return false;

	return true;
}

//==============================================================================
// �o���b�g�̔���
// ���@���FD3DXVECTOR3 pos(�o���b�g�̈ʒu)�AD3DXVECTOR3 normal(�u���b�N�̖@��)�A
//		   int Index(�g�p�҂̃A�h���X�ԍ�)�Aint bno(�o���b�g�̃A�h���X)
// �߂�l�FD3DXVECTOR3�^
//==============================================================================
D3DXVECTOR3 ReflectBullet(D3DXVECTOR3 pos, D3DXVECTOR3 normal, int index, int bno)
{
	BULLET *bullet = &bulletWk[index];
	D3DXVECTOR3 normal_n;
	D3DXVECTOR3 frontVec = pos - bullet[index].prevPos[bno];
	D3DXVECTOR3	out;

	D3DXVec3Normalize(&normal_n, &normal);
	D3DXVec3Normalize(&out, &(frontVec - 2.0f * D3DXVec3Dot(&frontVec, &normal_n) * normal_n));
	return out;
}

////==============================================================================
//// �o���b�g�̓��ߏ���
//// ���@���Fint Index(�o���b�g�̃A�h���X�ԍ�)�Afloat val(���l)
//// �߂�l�F�Ȃ�
////==============================================================================
//void SetDiffuseBullet(int Index, float val)
//{
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_3D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
//
//		pVtx += (Index * 4);
//
//		// ���ˌ��̐ݒ�
//		pVtx[0].diffuse =
//			pVtx[1].diffuse =
//			pVtx[2].diffuse =
//			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffBullet->Unlock();
//	}
//}