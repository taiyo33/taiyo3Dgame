//===============================================================================
//
// �o���b�g���� [bullet.cpp]
// Author : GP11A_341_22_�c�����z 
//
//===============================================================================
#include "bullet.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "field.h"
#include "checkhit.h"
#include "block.h"
#include "bulletEffect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLET001		"data/TEXTURE/bullet001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_BULLET002		"data/TEXTURE/bullet002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	BULLET_SIZE_X		(20.0f)							// �o���b�g�̕�
#define	BULLET_SIZE_Y		(20.0f)							// �o���b�g�̍���
#define	BULLET_SIZE_Z		(20.0f)							// �o���b�g�̉��s
#define	BULLET_SPEED		(8.0f)							// �ړ����x
#define BULLET_RADY_FRAME	(10)							// ���ˊԊu
#define TEXTURE_MAX			(2)								// �e�N�X�`���[�̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(int Index, float fSizeX, float fSizeY);
bool CheckBlockInBullet(int index, int bno);
void MoveBullet(int index, int bno);
//bool CheckReflectBullet(int index, int bno);
//void SetDiffuseBullet(int Index, float val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

enum {
	TEX_NUM001,
	TEX_NUM002
};
LPDIRECT3DTEXTURE9		D3DTextureBullet[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBullet = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
static float			dif_mi[BULLET_MAX];
static int				cntFrame[BULLET_SET_MAX];
BULLET					bulletWk[BULLET_SET_MAX];

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
			TEXTURE_BULLET001,			// �t�@�C���̖��O
			&D3DTextureBullet[TEX_NUM001]);	// �ǂݍ��ރ������[
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLET002,			// �t�@�C���̖��O
			&D3DTextureBullet[TEX_NUM002]);	// �ǂݍ��ރ������[
	}


	for (int i = 0; i < BULLET_SET_MAX; i++)
	{
		cntFrame[i] = BULLET_RADY_FRAME;						// �t���[����������
		bullet[i].sclIncrease = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�P�[���̑�����
		bullet[i].speedIncrease = 0.0f;	// �X�P�[���̑�����

		for (int j = 0; j < BULLET_ONESET_MAX; j++)
		{	
			bullet[i].use[j] = false;									// �g�p��Ԃ�������
			bullet[i].reflect[j] = false;
			bullet[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu��������
			bullet[i].rot[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]��������
			bullet[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// �g�嗦��������
			bullet[i].size[j] = D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, BULLET_SIZE_Z); // �傫����������
			bullet[i].cntReflect[j] = INIT_REFLECT_CNT;					// �����̏�����
			bullet[i].speed[j] = INIT_BULLET_SPEED;
		}
	}
	return S_OK;
}

//===============================================================================
// �I������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//===============================================================================
void UninitBullet(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBullet[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureBullet[i]->Release();
			D3DTextureBullet[i] = NULL;
		}
	}
	if (D3DVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffBullet->Release();
		D3DVtxBuffBullet = NULL;
	}
}

//===============================================================================
// �X�V����
//===============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = &bulletWk[0];
	int i, j;

	for (i = 0; i < BULLET_SET_MAX; i++)
	{
		cntFrame[i]++;	// �t���[���J�E���g�̍X�V

		for (j = 0; j < BULLET_ONESET_MAX; j++)
		{
			// �g�p���Ȃ��
			if (bullet[i].use[j])
			{
				// ���݈ʒu��ۑ�
				bullet[i].prevPos[j] = bullet[i].pos[j];

				SetBulletEffect(bullet[i].pos[j], bullet[i].rot[j], bullet[i].scl[j], 0, bullet[i].size[j].x, bullet[i].size[j].y, i);
				
				// �ړ�����
				MoveBullet(i, j);

				// �ړ���
				bullet[i].pos[j].x += bullet[i].move[j].x;
				bullet[i].pos[j].y -= bullet[i].move[j].y;
				bullet[i].pos[j].z += bullet[i].move[j].z;

				// ���ŏ���
				if (!CheckBlockInBullet(i, j))
				{
					bullet[i].use[j] = false;
					bullet[i].reflect[j] = false;
					bullet[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					bullet[i].speed[j] = INIT_BULLET_SPEED;
				}
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
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < BULLET_SET_MAX; i++)
	{
		for (int j = 0; j < BULLET_ONESET_MAX; j++)
		{
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
				D3DXMatrixScaling(&mtxScale, bullet[i].scl[j].x, bullet[i].scl[j].y, bullet[i].scl[j].z);
				D3DXMatrixMultiply(&bullet[i].mtxWorld,&bullet[i].mtxWorld, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, bullet[i].pos[j].x, bullet[i].pos[j].y, bullet[i].pos[j].z);
				D3DXMatrixMultiply(&bullet[i].mtxWorld, &bullet[i].mtxWorld, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &bullet[i].mtxWorld);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, D3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureBullet[i]);

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
		&D3DVtxBuffBullet,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < BULLET_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			SetVertexBullet(i, BULLET_SIZE_X, BULLET_SIZE_Y);

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
		D3DVtxBuffBullet->Unlock();
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
		D3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBullet->Unlock();
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
// ���@���FD3DXVECTOR3 pos(�ʒu)�AD3DXVECTOR3 rot(�p�x)�Afloat Dest(����)
// �߂�l�Fbool�^�@���g�p�̏ꍇ true�A�g�p���̏ꍇ false
//=========================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float speed, float Dest, int index)
{
	BULLET *bullet = &bulletWk[index];

	for (int i = 0; i < BULLET_ONESET_MAX; i++)
	{
		if (!bullet->use[i])
		{
			bullet->use[i] = true;									// �g�p����
			bullet->pos[i].x = pos.x + cosf(rot.y) * Dest;			// �v���C���[�̈ʒu�ց@
			bullet->pos[i].z = pos.z + sinf(rot.y) * Dest;			//�@
			bullet->pos[i].y = pos.y;								//
			bullet->rot[i] = rot;									// ��]�ʂ���
 			bullet->speed[i] = bullet->speed[i] + speed;
			//bullet->scl[i] = scl;									// �X�P�[������
			//bullet->size[i] = BULLET_SIZE_X * scl;					//
			SetVertexBullet(i, bullet->size[i].x, bullet->size[i].y);	// ���_���쐬
			cntFrame[index] = 0;									// �t���[���J�E���g��������
			bullet->sclIncrease = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�P�[���̑����l��������
			bullet->speedIncrease = 0.0f;							// ���xA�̑����l��������

			return;
		}
	}
	return;
}

//========================================================================
// �o���b�g�̈ړ�����
// ���@���Fint index(�o���b�g�̃A�h���X)
// �߂�l�F�ȁ@��
//========================================================================
void MoveBullet(int index, int bno)
{
	BULLET *bullet = &bulletWk[index];

	if (!bullet->reflect[bno])
	{
		bullet->move[bno].x = sinf(bullet->rot[bno].y) * bullet->speed[bno];
		bullet->move[bno].y = tanf(bullet->rot[bno].x) * bullet->speed[bno];
		bullet->move[bno].z = cosf(bullet->rot[bno].y) * bullet->speed[bno];

		// �o���b�g�ƃu���b�N�̓����蔻��
		if (!HitCheckBlock(bullet->pos[bno] + bullet->move[bno], bullet->prevPos[bno], BLOCK_VTX_MAX))
		{
			bullet->refVec[bno] = ReflectVector(bullet->pos[bno] + bullet->move[bno], bullet->prevPos[bno],GetNormal());
			bullet->move[bno] = bullet->refVec[bno] * bullet->speed[bno];
			bullet->reflect[bno] = true;
			bullet->cntReflect[bno]--;
		}
	}
	else if (bullet->reflect[bno])
	{
		bullet->move[bno] = bullet->refVec[bno] * bullet->speed[bno];
	}
}

//========================================================================
// �o���b�g�̉�ʓ��O���菈��
// ���@���Fint index(�g�o���b�g�̃A�h���X), int bno(�o���b�g�P�̂̃A�h���X)
// �߂�l�Fbool�^�@true�ł���Ή�ʓ��ɂ���Afalse�Ȃ�Ή�ʊO�ɂ���
//========================================================================
bool CheckBlockInBullet(int index, int bno)
{
	BULLET *bullet = &bulletWk[index];
	
	if (bullet->pos[bno].x > SCREEN_WIDTH  / 2)	return false;
	if (bullet->pos[bno].x < -SCREEN_WIDTH / 2)	return false;
	if (bullet->pos[bno].z > SCREEN_HEIGHT / 2)	return false;
	if (bullet->pos[bno].z < -SCREEN_HEIGHT / 2)	return false;

	return true;
}

//========================================================================
// �o���b�g�̔��ˉ񐔔��菈��
// ���@���Fint index(�g�o���b�g�̃A�h���X), int blockNo(�u���b�N�̃A�h���X)
//		   D3DXVECTOR3 pos(�Ώۃo���b�g�̈ʒu)
// �߂�l�Fbool�^�@true�ł���΁Afalse�Ȃ�Ώ���
//========================================================================
void CheckBlockHitBullet(int blockNo, int index, D3DXVECTOR3 pos)
{
	BULLET *bullet = GetBullet(index);	//�o���b�g�̃A�h���X���擾
	for (int i = 0; i < BULLET_ONESET_MAX; i++)
	{
		if (!bullet->use[i]) continue;
		if (CheckHitBB(bullet->pos[i], pos,
			D3DXVECTOR3(15.0f, 15.0f, 15.0f), D3DXVECTOR3(25.0f, 25.0f, 25.0f)))
		{
			bullet->cntReflect[i]--;
			if (bullet->cntReflect[i] < 0)
			{
				bullet->use[i] = false;
				bullet->reflect[i] = false;
				bullet->cntReflect[i] = 2;
				bullet->speed[i] = INIT_BULLET_SPEED;
			}
		}
	}
}
