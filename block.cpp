//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : GP11A_341_22_�c�����z GP11A341_22_�c�����z
//
//=============================================================================
#include "block.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "effect.h"
#include "smoke.h"
#include "checkhit.h"
#include "explosion.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	BLOCK_MODEL			"data/MODEL/block001.x"	// �ǂݍ��ރ��f����
#define TEXTURE_MAX				(5)
#define INIT_POS_X_RIGHTSIDE	(386.5f)	// �E���̃u���b�N�̏���X���W
#define INIT_POS_X_LEFTSIDE		(-388.5f)	// �����̃u���b�N�̏���X���W
#define INIT_POS_Y				(0.0f)		// �u���b�N�̏���Y���W
#define INIT_POS_Z_NEARSIDE		(-136.5f)	// ��O�̃u���b�N�̏���Z���W
#define INIT_POS_Z_FEARSIDE		(138.5f)	// ���̃u���b�N�̏���Z���W
#define BLOCK_VERTEX			(4)			// 
#define VTX_SIZE_X				(370.0f)	// ���_��X���T�C�Y
#define VTX_SIZE_Y				(25.0f)		// �@�@�@Y���T�C�Y
#define VTX_SIZE_Z				(120.0f)	// �@�@�@Z���T�C�Y
#define BLOCK_DAMAGE			(1)			// �u���b�N�ւ̃_���[�W
#define INIT_LIFE				(10)		// �����̏����l	
#define BLOCK_SIZE				(25.0f)		// �u���b�N��1�ӂ̃T�C�Y

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetPosBlock(void);
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);
void InitVertexBlock(void);

enum {
	TEX_TYPE01,
	TEX_TYPE02,
	TEX_TYPE03,
	TEX_TYPE04,
	TEX_TYPE05
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBlock[TEXTURE_MAX] = { 
									NULL,NULL,NULL,NULL,NULL }; // �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH				D3DXMeshBlock;		// ID3DXMeshBlock�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER			D3DXBuffMatBlock;	// ���b�V���̃}�e���A�������i�[
DWORD					numMat;				// �������̑���
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBlock;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				mtxWorld;			// ���[���h�}�g���b�N�X
D3DXMATRIX				mtxWorldVtx;			// ���[���h�}�g���b�N�X
int						textureNum;			// �e�N�X�`���[�̔ԍ�
BLOCK					block[BLOCK_MAX];	// �u���b�N�\����
//=============================================================================
// ����������
// ���@���Fint type(�ď������Q������l)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitBlock(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;
	// ���_���̍쐬
	hr = MakeVertexBlock(pDevice);
	
	// �u���b�N�̏����ʒu��ݒ�
	SetPosBlock();

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		block[i].use = true;		// �g�p�֏�����
		block[i].life = INIT_LIFE;	// ���C�t�̏�����
	}

	D3DXMeshBlock = NULL;			// �C���^�[�t�F�[�X�̏�����
	D3DXBuffMatBlock = NULL;		// �}�e���A���̏�����
	textureNum = NULL;				// �e�N�X�`���[�ԍ���������

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(BLOCK_MODEL,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&D3DXBuffMatBlock,
		NULL,
		&numMat,
		&D3DXMeshBlock)))
	{
		return E_FAIL;
	}

	//if (type == 0)
	//{
	//	// �e�N�X�`���̓ǂݍ���
	//	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
	//						HANDGUM_TEXTURE,	// �t�@�C���̖��O
	//					&D3DTextureBlock[TEX_TYPE01]);	// �ǂݍ��ރ������[
	//}	
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlock(void)
{
	if(D3DXMeshBlock != NULL)
	{// ���b�V���̊J��
		D3DXMeshBlock->Release();
		D3DXMeshBlock = NULL;
	}

	if(D3DXBuffMatBlock != NULL)
	{// �}�e���A���̊J��
		D3DXBuffMatBlock->Release();
		D3DXBuffMatBlock = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBlock[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureBlock[i]->Release();
			D3DTextureBlock[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlock(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (block[i].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, block[i].pos.x, block[i].pos.y, block[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatBlock->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)numMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureBlock[textureNum]);

				// �`��
				D3DXMeshBlock->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldVtx);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldVtx);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, D3DVtxBuffBlock, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTextureBlock[0]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}
}


//=============================================================================
// �u���b�N�̃A�h���X�擾
//=============================================================================
BLOCK *GetBlock(int bno)
{
	return &block[bno];
}

//=============================================================================
// �u���b�N�̐ݒu
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void SetPosBlock(void)
{
	block[0].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[1].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 113.5f);
	block[2].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 88.5f);
	block[3].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 63.5f);
	block[4].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 38.5f);
	block[5].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 13.5f);
	block[6].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -11.5f);
	block[7].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -36.5f);
	block[8].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -61.5f);
	block[9].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -86.5f);
	block[10].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -111.5f);
	block[11].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[12].pos = D3DXVECTOR3(-363.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[13].pos = D3DXVECTOR3(-338.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[14].pos = D3DXVECTOR3(-313.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[15].pos = D3DXVECTOR3(-288.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[16].pos = D3DXVECTOR3(-263.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[17].pos = D3DXVECTOR3(-238.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[18].pos = D3DXVECTOR3(-213.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[19].pos = D3DXVECTOR3(-188.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[20].pos = D3DXVECTOR3(-163.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[21].pos = D3DXVECTOR3(-138.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[22].pos = D3DXVECTOR3(-113.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[23].pos = D3DXVECTOR3(-88.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[24].pos = D3DXVECTOR3(-63.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[25].pos = D3DXVECTOR3(-38.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[26].pos = D3DXVECTOR3(-13.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[27].pos = D3DXVECTOR3(11.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[28].pos = D3DXVECTOR3(36.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[29].pos = D3DXVECTOR3(61.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[30].pos = D3DXVECTOR3(86.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[31].pos = D3DXVECTOR3(111.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[32].pos = D3DXVECTOR3(136.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[33].pos = D3DXVECTOR3(161.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[34].pos = D3DXVECTOR3(186.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[35].pos = D3DXVECTOR3(211.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[36].pos = D3DXVECTOR3(236.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[37].pos = D3DXVECTOR3(261.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[38].pos = D3DXVECTOR3(286.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[39].pos = D3DXVECTOR3(311.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[40].pos = D3DXVECTOR3(336.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[41].pos = D3DXVECTOR3(361.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[42].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[43].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -113.5f);
	block[44].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -88.5f);
	block[45].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -63.5f);
	block[46].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -38.5f);
	block[47].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -13.5f);
	block[48].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 11.5f);
	block[49].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 36.5f);
	block[50].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 61.5f);
	block[51].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 86.5f);
	block[52].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 111.5f);
	block[53].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[54].pos = D3DXVECTOR3(361.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[55].pos = D3DXVECTOR3(336.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[56].pos = D3DXVECTOR3(311.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[57].pos = D3DXVECTOR3(286.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[58].pos = D3DXVECTOR3(261.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[59].pos = D3DXVECTOR3(236.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[60].pos = D3DXVECTOR3(211.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[61].pos = D3DXVECTOR3(186.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[62].pos = D3DXVECTOR3(161.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[63].pos = D3DXVECTOR3(136.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[64].pos = D3DXVECTOR3(111.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[65].pos = D3DXVECTOR3(86.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[66].pos = D3DXVECTOR3(61.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[67].pos = D3DXVECTOR3(36.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[68].pos = D3DXVECTOR3(11.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[69].pos = D3DXVECTOR3(-13.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[70].pos = D3DXVECTOR3(-38.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[71].pos = D3DXVECTOR3(-63.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[72].pos = D3DXVECTOR3(-88.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[73].pos = D3DXVECTOR3(-113.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[74].pos = D3DXVECTOR3(-138.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[75].pos = D3DXVECTOR3(-163.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[76].pos = D3DXVECTOR3(-188.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[77].pos = D3DXVECTOR3(-213.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[78].pos = D3DXVECTOR3(-238.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[79].pos = D3DXVECTOR3(-263.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[80].pos = D3DXVECTOR3(-288.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[81].pos = D3DXVECTOR3(-313.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[82].pos = D3DXVECTOR3(-338.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[83].pos = D3DXVECTOR3(-363.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
}

//================================================================================
// �u���b�N�ւ̃_���[�W����
// ���@���Fint bno(�u���b�N�̃A�h���X�ԍ�)
// �߂�l�F�Ȃ�
//================================================================================
void BlockDamageManager(int bno)
{
	block[bno].life -= BLOCK_DAMAGE;

	if (block[bno].life == 3)
	{
		//textureNum = TEX_TYPE02;
	}
	else if (block[bno].life == 0)
	{
		block[bno].use = false;
		SetExplosion(block[bno].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BLOCK_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffBlock,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	/* ���_���̐ݒ� */
	InitVertexBlock();

	return S_OK;
}



//============================================================================
// �Q�[���t�B�[���h�̍쐬
//============================================================================
void InitVertexBlock(void)
{
	int i = 0;
	float size = 0.0f;

	// BOX�̒��_���̐ݒ�
	{	//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D			   *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

		for (i = 0; i < BLOCK_NUM_LEFTSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X, VTX_SIZE_Y, VTX_SIZE_Z - size);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X, VTX_SIZE_Y, VTX_SIZE_Z + BLOCK_SIZE - size);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X, 0.0f, VTX_SIZE_Z - size);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X, 0.0f, VTX_SIZE_Z + BLOCK_SIZE - size);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		size = 25.0f;

		for (i = BLOCK_NUM_LEFTSIDE; i < BLOCK_NUM_NEARSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X - BLOCK_SIZE + size, VTX_SIZE_Y, -VTX_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X + size, VTX_SIZE_Y, -VTX_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X - BLOCK_SIZE + size, 0.0f, -VTX_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X + size, 0.0f, -VTX_SIZE_Z);

			// �@���x�N�g���̐ݒ�
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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}

		size = 0.0f;

		for (i = BLOCK_NUM_NEARSIDE; i < BLOCK_NUM_RIGHTSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X, VTX_SIZE_Y, -VTX_SIZE_Z + size);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X, VTX_SIZE_Y, -VTX_SIZE_Z - BLOCK_SIZE + size);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X, 0.0f, -VTX_SIZE_Z + size);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X, 0.0f, -VTX_SIZE_Z - BLOCK_SIZE + size);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		size = 25.0f;

		for (i = BLOCK_NUM_RIGHTSIDE; i < BLOCK_NUM_FEARSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X - size, VTX_SIZE_Y, VTX_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X + BLOCK_SIZE - size, VTX_SIZE_Y, VTX_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X - size, 0.0f, VTX_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X + BLOCK_SIZE - size, 0.0f, VTX_SIZE_Z);

			// �@���x�N�g���̐ݒ�
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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBlock->Unlock();
	}
}

//=============================================================================
// �u���b�N�̓����|���S���Ƃ̓����蔻��
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
//=============================================================================
bool HitCheckBlock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		pos[NUM_VERTEX]; // ���_���W�̕ۑ�
	bool			ans;
	VERTEX_3D	    *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < BLOCK_NUM_FEARSIDE; i++, pVtx += 4)
	{
		if (block[i].use)
		{
			// ���_���W�̐ݒ�
			pos[0] = pVtx[0].vtx;
			pos[1] = pVtx[1].vtx;
			pos[2] = pVtx[2].vtx;
			pos[3] = pVtx[3].vtx;

			// �������|���S���Ɛ����̓����蔻��
			ans = CheckHitPolygon(pos[0], pos[2], pos[3], pos0, pos1);
			if (!ans)
			{
				break;
			}
			// �E�㑤�|���S���Ɛ����̓����蔻��
			ans = CheckHitPolygon(pos[0], pos[3], pos[1], pos0, pos1);
			if (!ans)
			{
				break;
			}
		}
	}

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffBlock->Unlock();

	return ans;
}

//=============================================================================
// 4�p�Ƃ̓����蔻��
// ���@���FD3DXVECTOR3 pos0(����Ώۂ̈ʒu�j
// �߂�l�Fbool�^�@true���Ɠ������Ă���Afalse���Ɠ������Ă��Ȃ�
//=============================================================================
bool HitCheckCornerBlock(D3DXVECTOR3 pos0)
{
	if (CheckHitBB(pos0, block[0].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(15.0f, 25.0f, 15.0f))) return true;

	if (CheckHitBB(pos0, block[11].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(15.0f, 25.0f, 15.0f))) return true;

	if (CheckHitBB(pos0, block[42].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(15.0f, 25.0f, 15.0f))) return true;

	if (CheckHitBB(pos0, block[53].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(15.0f, 25.0f, 15.0f))) return true;

	return false;
}