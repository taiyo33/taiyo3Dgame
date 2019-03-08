//=============================================================================
//
// ���C������ [main.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "debugproc.h"
#include "hitEffect.h"
#include "bullet.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "time.h"
#include "field.h"
#include "checkhit.h"
#include "explosion.h"
#include "bulletEffect.h"
#include "leftArm.h"
#include "rightArm.h"
#include "head.h"
#include "block.h"
#include "bulletGauge.h"
#include "lifeGauge.h"
#include "item.h"
#include "ball.h"
#include "ai.h"
#include "buttleGauge.h"
#include "gaugeEffect.h"
#include "icon.h"
#include "gameCall.h"
#include "chargeEffect.h"
#include "pause.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"					// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"3D�Q�[���uRe:Plection�v"	// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Init(HINSTANCE hInstance, HWND hWnd);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;			// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
#ifdef _DEBUG
int						g_nCountFPS;			// FPS�J�E���^
#endif
bool					g_bDispDebug = true;	// �f�o�b�O�\��ON/OFF
int						StageSelect;
bool					screen = true;
bool					pause = false;
//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif

				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W���Z��������
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// �e������
	Init(hInstance, hWnd);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// ���͏����̏I������
	UninitInput();

	// �f�o�b�O�\�������̏I������
	UninitDebugProc();

	// �e�̏I������
	UninitShadow();

	// �o���b�g�̏I������
	UninitBullet();

	// �v���C���[�̏I������
	UninitPlayer();

	// �G�t�F�N�g�̏I������
	UninitHitEffect();

	// �^�C�g���̏I������
	UninitTitle();

	// ���U���g�̏I������
	UninitResult();

	// �`���[�g���A���̏I������
	UninitTutorial();

	// �t�B�[���h�̏I������
	UninitField();

	// �����G�t�F�N�g�̏I������
	UninitExplosion();

	// �o���b�g�G�t�F�N�g�̏I������
	UninitBulletEffect();

	// ���r���f���̏I������
	UninitLeftArm();

	// �E�r���f���̏I������
	UninitRightArm();

	// �����f���̏I������
	UninitHead();

	// �u���b�N�̏I������
	UninitBlock();

	// �o���b�g�Q�[�W�̏I������
	UninitBulletGauge();

	// ���C�t�Q�[�W�̏I������
	UninitLifeGauge();

	// �A�C�e���̏I������
	UninitItem();

	// �q�����f���̏I������
	UninitBall();

	// �o�g���Q�[�W�̏I������
	UninitButtleGauge();

	//
	UninitGaugeEffect();

	//
	UninitIcon();

	// �Q�[���J�ڃe���b�v�̏I������
	UninitGameCall();

	// �`���[�W�G�t�F�N�g�̏I������
	UninitChargeEffect();

	// ���j���[�̏I������
	UninitPause();

	// �T�E���h�̏I������
	UninitSound();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	if(GetKeyboardTrigger(DIK_F1))
	{// �f�o�b�O�\��ON/OFF
		g_bDispDebug = g_bDispDebug ? false: true;
	}

	// ���͍X�V
	UpdateInput();

	switch (StageSelect)
	{
		case TITLE:
		{
			// �^�C�g���̏I������
			UpdateTitle();

			// ���f�������̍X�V
			UpdatePlayer();

			// ���r�̍X�V
			UpdateLeftArm();

			// �E�r�̍X�V
			UpdateRightArm();

			// ���̍X�V
			UpdateHead();

			// �q���̍X�V
			UpdateBall();

			break;
		}
		case TUTORIAL:
		{
			// �`���[�g���A���̍X�V����
			UpdateTutorial();

			break;
		}
		case STARTCALL:
		{
			// �Q�[���J�ڃe���b�v�̍X�V����
			UpdateGameCall();

			// �q���̍X�V
			UpdateBall();

			break;
		}
		case START:
		{
			if (!pause)
			{
				// �u���b�N�̍X�V
				UpdateBlock();

				// �t�B�[���h�̍X�V
				UpdateField();

				// �e�����̍X�V
				UpdateShadow();

				// �J�����X�V
				UpdateCamera();

				// ���f�������̍X�V
				UpdatePlayer();

				// ���r�̍X�V
				UpdateLeftArm();

				// �E�r�̍X�V
				UpdateRightArm();

				// ���̍X�V
				UpdateHead();

				// �L�����N�^�[�A�C�R���̍X�V
				//UpdateIcon();

				// �q���̍X�V
				UpdateBall();

				// �`���[�W�G�t�F�N�g�̍X�V
				UpdateChargeEffect();

				// �G�t�F�N�g�̍X�V
				UpdateHitEffect();

				// �e���̍X�V
				UpdateExplosion();

				// ���̍X�V
				UpdateBulletEffect();

				// �o���b�g�̍X�V
				UpdateBullet();

				// �A�C�e���̍X�V
				UpdateItem();

				// ���Ԃ̍X�V
				UpdateTime();

				// �o���b�g�Q�[�W�̍X�V
				UpdateBulletGauge();

				// ���C�t�Q�[�W�̍X�V
				UpdateLifeGauge();

				// �o�g���Q�[�W�X�V
				UpdateButtleGauge();

				// �Q�[�W�G�t�F�N�g�̍X�V
				UpdateGaugeEffect();

				// �����蔻��
				CheckHit();
			}
			else if(pause)
			{
				// �|�[�Y��ʂ̍X�V
				UpdatePause();
				// �Q�[�W�G�t�F�N�g�̍X�V
				UpdateGaugeEffect();
			}

			break;
		}
		case FINISHCALL:
		{
			// �q���̍X�V
			UpdateBall();

			// �Q�[�W�G�t�F�N�g�̍X�V
			UpdateGaugeEffect();

			// �Q�[���J�ڃe���b�v�̍X�V����
			UpdateGameCall();

			break;
		}
		case RESULT:
		{
			// ���U���g�̍X�V
			UpdateResult();

			break;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(75, 144, 205, 180), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (StageSelect)
		{
			case TITLE:
			{
				// �J�����̐ݒ�
				SetCamera();

				// �t�B�[���h�̕`��
				DrawField();

				// �u���b�N�̕`��
				DrawBlock();

				// �v���C���[�����̕`��
				DrawPlayer();

				// ���̕`��
				DrawHead();

				// ���r�̕`��
				DrawLeftArm();

				// �E�r�̕`��
				DrawRightArm();

				// �q�����f���̕`��
				DrawBall();

				// �^�C�g���̏I������
				DrawTitle();

				break;
			}
			case TUTORIAL:
			{
				// �`���[�g���A���̏I������
				DrawTutorial();

				break;
			}
			case STARTCALL:
			{
				// �J�����̐ݒ�
				SetCamera();

				// �t�B�[���h�̕`��
				DrawField();

				// �u���b�N�̕`��
				DrawBlock();

				// �v���C���[�����̕`��
				DrawPlayer();

				// ���̕`��
				DrawHead();

				// ���r�̕`��
				DrawLeftArm();

				// �E�r�̕`��
				DrawRightArm();

				// �q�����f���̕`��
				DrawBall();

				// �Q�[���J�ڃe���b�v�̕`��
				DrawGameCall();

				break;
			}
			case START:
			{
				// �J�����̐ݒ�
				SetCamera();
				
				// �t�B�[���h�̕`��
				DrawField();

				// �u���b�N�̕`��
				DrawBlock();

				// �A�C�e���̕`��
				DrawItem();

				// �e�����̕`��
				DrawShadow();

				// �o���b�g�̕`��
				DrawBullet();

				// �����G�t�F�N�g�̕`��
				DrawExplosion();

				// �o���b�g�G�t�F�N�g�̕`��
				DrawBulletEffect();
				
				// �q�����f���̕`��
				DrawBall();

				// �`���[�W�G�t�F�N�g�̕`��
				DrawChargeEffect();

				// �v���C���[�����̕`��
				DrawPlayer();

				// ���̕`��
				DrawHead();

				// ���r�̕`��
				DrawLeftArm();

				// �E�r�̕`��
				DrawRightArm();

				// ��e�G�t�F�N�g�̕`��
				DrawHitEffect();

				// �L�����N�^�[�A�C�R���̕`��
				//DrawIcon();

				// ���Ԃ̕`��
				DrawTime();

				// ���C�t�Q�[�W�̕`��
				DrawLifeGauge();

				// �o���b�g�Q�[�W�̕`��
				DrawBulletGauge();

				// �o�g���Q�[�W�̕`��
				DrawButtleGauge();

				// �Q�[�W�G�t�F�N�g�̕`��
				DrawGaugeEffect();

				// ���j���[�̕`��
				if (pause)
				{
					DrawPause();
				}

#ifdef _DEBUG
				// �f�o�b�O�\�������̕`��
				if (g_bDispDebug)
				{
					DrawDebugProc();
				}

#endif

				break;
			}
			case FINISHCALL:
			{
				// �J�����̐ݒ�
				SetCamera();

				// �t�B�[���h�̕`��
				DrawField();

				// �u���b�N�̕`��
				DrawBlock();

				// �v���C���[�����̕`��
				DrawPlayer();

				// ���̕`��
				DrawHead();

				// ���r�̕`��
				DrawLeftArm();

				// �E�r�̕`��
				DrawRightArm();

				// �q�����f���̕`��
				DrawBall();

				// ���C�t�Q�[�W�̕`��
				DrawLifeGauge();

				// �o���b�g�Q�[�W�̕`��
				DrawBulletGauge();

				// �o�g���Q�[�W�̕`��
				DrawButtleGauge();

				// �Q�[�W�G�t�F�N�g�̕`��
				DrawGaugeEffect();

				// �Q�[���J�ڃe���b�v�̕`��
				DrawGameCall();

				break;
			}
			case RESULT:
			{

				// �J�����̐ݒ�
				SetCamera();

				// �t�B�[���h�̕`��
				DrawField();

				// �u���b�N�̕`��
				DrawBlock();

				// �v���C���[�����̕`��
				DrawPlayer();

				// ���̕`��
				DrawHead();

				// ���r�̕`��
				DrawLeftArm();

				// �E�r�̕`��
				DrawRightArm();

				// �q�����f���̕`��
				DrawBall();

				// ���C�t�Q�[�W�̕`��
				DrawLifeGauge();

				// �o���b�g�Q�[�W�̕`��
				DrawBulletGauge();

				// �o�g���Q�[�W�̕`��
				DrawButtleGauge();

				// �Q�[�W�G�t�F�N�g�̕`��
				DrawGaugeEffect();


				// ���U���g�̏I������
				DrawResult();

				break;
			}
		}

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//============================================================================
// �Q�[���J�ڂ̐ݒu
// �����F int Stage(�J�ڐ�̔ԍ�)
// �߂�l�F�Ȃ�
//============================================================================
void SetStage(int Stage)
{
	StageSelect = Stage;
}

//============================================================================
// �Q�[���J�ڂ̎擾
// �����F int Stage(�J�ڐ�̔ԍ�)
// �߂�l�F�Ȃ�
//============================================================================
int GetStage(void)
{
	return StageSelect;
}

//============================================================================
// �|�[�Y��ʂ̐ݒu
// �����F int Stage(�J�ڐ�̔ԍ�)
// �߂�l�F�Ȃ�
//============================================================================
bool SetPause(bool on)
{
	return pause = on;
}

//============================================================================
// �|�[�Y��ʂ̐ݒu
// �����F int Stage(�J�ڐ�̔ԍ�)
// �߂�l�F�Ȃ�
//============================================================================
bool GetPause(void)
{
	return &pause;
}

//============================================================================
// �e�t�@�C���̏�����
//============================================================================
void Init(HINSTANCE hInstance, HWND hWnd)
{
	// �T�E���h������
	InitSound(hWnd);

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �f�o�b�O�\�������̏�����
	InitDebugProc();

	// ���C�g�̏�����
	InitLight();

	// �J�����̏�����
	InitCamera();

	// �e�̏�����
	InitShadow(0);

	// �v���C���[�̏�����
	InitPlayer(0);

	// �o���b�g�̏�����
	InitBullet(0);

	// �G�t�F�N�g�̏�����
	InitHitEffect(0);

	// �����G�t�F�N�g�̏�����
	InitExplosion(0);

	// ���Ԃ̏�����
	InitTime(0);

	// �^�C�g���̏�����
	InitTitle(0);

	// �u���b�N�̏�����
	InitBlock(0);

	// ���U���g�̏�����
	InitResult(0);

	// �`���[�g���A���̏�����
	InitTutorial(0);

	// �t�B�[���h�̏�����
	InitField(0);

	// �o���b�g�G�t�F�N�g�̏�����
	InitBulletEffect(0);
	
	// ���r���f���̏�����
	InitLeftArm();

	// �E�r���f���̏�����
	InitRightArm();

	// �����f���̏�����
	InitHead();

	// �L�����N�^�[�A�C�R���̏�����
	InitIcon(0);

	// �o���b�g�Q�[�W�̏�����
	InitBulletGauge(0);

	// ���C�t�Q�[�W�̏�����
	InitLifeGauge(0);

	// �A�C�e���̏�����
	InitItem(0);

	// �q�����f���̏�����
	InitBall();

	// AI�̏�����
	InitAi();

	// �o�g���Q�[�W�̏�����
	InitButtleGauge(0);

	// �Q�[�W�G�t�F�N�g�̏�����
	InitGaugeEffect(0);

	// �Q�[���J�ڃe���b�v�̏�����
	InitGameCall(0);

	// �`���[�W�G�t�F�N�g�̏�����
	InitChargeEffect(0);

	// �|�[�Y��ʂ̏�����
	InitPause(0);

}

//============================================================================
// �Q�[���̍ď�����
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//============================================================================
void InitGame(void)
{
	// ���C�g�̏�����
	InitLight();

	// �J�����̏�����
	InitCamera();

	// �e�̏�����
	InitShadow(INIT_GAME);

	// �v���C���[�̏�����
	InitPlayer(INIT_GAME);

	// �o���b�g�̏�����
	InitBullet(INIT_GAME);

	// �G�t�F�N�g�̏�����
	InitHitEffect(INIT_GAME);

	// �����G�t�F�N�g�̏�����
	InitExplosion(INIT_GAME);

	// ���Ԃ̏�����
	InitTime(INIT_GAME);

	// �^�C�g���̏�����
	InitTitle(INIT_GAME);

	// �u���b�N�̏�����
	InitBlock(INIT_GAME);

	// ���U���g�̏�����
	InitResult(INIT_GAME);

	// �`���[�g���A���̏�����
	InitTutorial(INIT_GAME);

	// �t�B�[���h�̏�����
	InitField(INIT_GAME);

	// �o���b�g�G�t�F�N�g�̏�����
	InitBulletEffect(INIT_GAME);

	// ���r���f���̏�����
	InitLeftArm();

	// �E�r���f���̏�����
	InitRightArm();

	// �����f���̏�����
	InitHead();

	// �L�����N�^�[�A�C�R���̏�����
	InitIcon(INIT_GAME);

	// �o���b�g�Q�[�W�̏�����
	InitBulletGauge(INIT_GAME);

	// ���C�t�Q�[�W�̏�����
	InitLifeGauge(INIT_GAME);

	// �A�C�e���̏�����
	InitItem(INIT_GAME);

	// �q�����f���̏�����
	InitBall();

	// AI�̏�����
	InitAi();
	
	// �|�[�Y��ʂ̏�����
	InitPause(INIT_GAME);

	// �o�g���Q�[�W�̏�����
	InitButtleGauge(INIT_GAME);

	// �Q�[�W�G�t�F�N�g�̏�����
	InitGaugeEffect(INIT_GAME);

	// �Q�[���J�ڃe���b�v�̏�����
	InitGameCall(INIT_GAME);

	// �`���[�W�G�t�F�N�g�̏�����
	InitChargeEffect(INIT_GAME);
}
//============================================================================
// �Q�[���|�[�Y������̏�����
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//============================================================================
void InitPauseGame(void)
{
	// �v���C���[�̏�����
	InitPlayer(INIT_GAME);

	// �o���b�g�̏�����
	InitBullet(INIT_GAME);

	// �G�t�F�N�g�̏�����
	InitHitEffect(INIT_GAME);

	// �����G�t�F�N�g�̏�����
	InitExplosion(INIT_GAME);

	// ���Ԃ̏�����
	InitTime(INIT_GAME);

	// �u���b�N�̏�����
	InitBlock(INIT_GAME);

	// ���U���g�̏�����
	InitResult(INIT_GAME);

	// �o���b�g�G�t�F�N�g�̏�����
	InitBulletEffect(INIT_GAME);

	// ���r���f���̏�����
	InitLeftArm();

	// �E�r���f���̏�����
	InitRightArm();

	// �����f���̏�����
	InitHead();

	// �L�����N�^�[�A�C�R���̏�����
	InitIcon(INIT_GAME);

	// �o���b�g�Q�[�W�̏�����
	InitBulletGauge(INIT_GAME);

	// ���C�t�Q�[�W�̏�����
	InitLifeGauge(INIT_GAME);

	// �q�����f���̏�����
	InitBall();

	// AI�̏�����
	InitAi();

	// �|�[�Y��ʂ̏�����
	InitPause(INIT_GAME);

	// �o�g���Q�[�W�̏�����
	InitButtleGauge(INIT_GAME);

	// �Q�[�W�G�t�F�N�g�̏�����
	InitGaugeEffect(INIT_GAME);

	// �Q�[���J�ڃe���b�v�̏�����
	InitGameCall(INIT_GAME);

	// �`���[�W�G�t�F�N�g�̏�����
	InitChargeEffect(INIT_GAME);
}
