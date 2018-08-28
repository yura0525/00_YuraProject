#include "TScene.h"
#include "TCore.h"

#define USE_BITMAP_ROTATE
class Sample : public TCore
{
public:
	std::shared_ptr<TScene> m_pGameScene;
	std::shared_ptr<TScene> m_pLobbyScene;
	std::shared_ptr<TScene> m_pEndScene;
	TScene* m_pCurrentScene;

	int m_iLevel;
	TObject m_tmpObj;

#ifdef USE_BITMAP_ROTATE
	HBITMAP m_hColorRotateBitmap;
	HBITMAP m_hMaskRotateBitmap;
	HDC		m_hRotationDC;
	HDC		m_hMemMaskDC;
	HDC		m_hMemColorDC;
	float	m_iMaxDistance;
	HBRUSH	m_hBrBack;
#endif

public:
	bool Init()
	{
		m_iLevel = 1;

		m_pLobbyScene = std::make_shared<TLobbyScene>();
		m_pGameScene = std::make_shared<TGameScene>();
		m_pEndScene = std::make_shared<TEndScene>();

		m_pCurrentScene = m_pLobbyScene.get();

		m_pLobbyScene->Init();
		m_pGameScene->Init();
		m_pEndScene->Init();
		
		m_tmpObj.Set(300, 300, 90, 2, 42, 58);
		m_tmpObj.Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");

#ifdef USE_BITMAP_ROTATE
		m_hbrBack = CreateSolidBrush(RGB(255, 255, 255));

		m_iMaxDistance = sqrt((m_tmpObj.m_rtDraw.right *
									m_tmpObj.m_rtDraw.right) +
									(m_tmpObj.m_rtDraw.bottom *
									m_tmpObj.m_rtDraw.bottom));

		m_hColorRotateBitmap = CreateCompatibleBitmap(m_hScreenDC, 
			m_iMaxDistance, m_iMaxDistance);
		m_hMaskRotateBitmap = CreateCompatibleBitmap(m_hScreenDC, 
			m_iMaxDistance, m_iMaxDistance);
		
		m_hRotationDC = CreateCompatibleDC(m_hScreenDC);
		m_hMemMaskDC = CreateCompatibleDC(m_hScreenDC);
		m_hMemColorDC = CreateCompatibleDC(m_hScreenDC);
#endif
		return true;
	}
#ifdef USE_BITMAP_ROTATE
	void RotateBlt(float fAngle, HBITMAP hBitmap, TBitmap* pSrcBitmap)
	{
		float fRadian = fAngle * 3.141592f / 180.0f;
		float fCosine = cos(fRadian);
		float fSin = sin(fRadian);

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hRotationDC, hBitmap);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hRotationDC, m_hBrBack);
		
		PatBlt(m_hRotationDC, 0, 0, m_iMaxDistance, m_iMaxDistance, PATCOPY);
		SelectObject(m_hRotationDC, hOldBrush);


		int iOldGraphic = SetGraphicsMode(m_hRotationDC, GM_ADVANCED);
		
		XFORM	xform;	//x,y,1 * (3 by 2) = x,y,1 동차좌표계
		xform.eM11 = fCosine;	xform.eM12 = -fSin;
		xform.eM21 = fSin;		xform.eM22 = fCosine;
		xform.eDx = m_iMaxDistance /2;
		xform.eDy = m_iMaxDistance / 2;
		SetWorldTransform(m_hRotationDC, &xform);
		

		//원본을 뿌리기.
		BitBlt(m_hRotationDC, 
			-(m_tmpObj.m_rtDraw.right/2), 
			-(m_tmpObj.m_rtDraw.bottom/2),
			m_tmpObj.m_rtDraw.right,
			m_tmpObj.m_rtDraw.bottom,
			pSrcBitmap->m_hMemDC, 
			m_tmpObj.m_rtDraw.left, 
			m_tmpObj.m_rtDraw.top, SRCCOPY);

		//이거 안해주서 안나왔다. 비트맵이 제대로 안나온다.
		SelectObject(m_hRotationDC, hOldBitmap);
		SelectObject(m_hRotationDC, hOldBrush);

		//다른것들이 영향을 받지 않도록 다시 단위행렬로 돌려주었다.
		xform.eM11 = 1;		xform.eM12 = 0;
		xform.eM21 = 0;		xform.eM22 = 1;
		xform.eDx = 0;
		xform.eDy = 0;

		//모든픽셀이 xform에 의해서 회전된다.
		SetWorldTransform(m_hRotationDC, &xform);
		SetGraphicsMode(m_hRotationDC, iOldGraphic);
	}
#endif
	bool Frame()
	{
		m_tmpObj.Frame();
		m_pCurrentScene->Frame();
		switch (m_pCurrentScene->m_iSceneID)
		{
		case GAME_SCENE_LOBBY:
			if (m_pCurrentScene->m_bNextSceneStart == true)
			{
				m_pCurrentScene->m_bNextSceneStart = false;
				m_pCurrentScene = m_pGameScene.get();
			}
			break;

		case GAME_SCENE_PLAY:
			if (m_pCurrentScene->m_bNextSceneStart == true)
			{
				if (m_iLevel > 3)
				{
					m_pCurrentScene = m_pEndScene.get();
					m_pCurrentScene->SetNPCCount(10);
					m_pCurrentScene->Reset();
					m_iLevel = 0;
				}
				else
				{
					m_pCurrentScene = m_pGameScene.get();
					++m_iLevel;
					m_pCurrentScene->SetNPCCount((m_iLevel * 10));
					m_pCurrentScene->Reset();
				}

			}
			break;
		case GAME_SCENE_END:
			break;
		}
		
		return true;
	}
	bool Render()
	{
		m_pCurrentScene->Render();

#ifdef USE_BITMAP_ROTATE
		static float fAngle = 0.0f;
		fAngle += g_fSecPerFrame * 100.0f;
		RotateBlt(fAngle, m_hMaskRotateBitmap, m_tmpObj.m_pMaskBitmap);
		RotateBlt(fAngle, m_hColorRotateBitmap, m_tmpObj.m_pColorBitmap);

		HBITMAP hOldMask = (HBITMAP)SelectObject(m_hMemMaskDC, m_hMaskRotateBitmap);
		HBITMAP hOldColor = (HBITMAP)SelectObject(m_hMemColorDC, m_hColorRotateBitmap);

		BitBlt(g_hOffScreenDC, 
			m_tmpObj.m_pos.x - (m_iMaxDistance / 2), 
			m_tmpObj.m_pos.y - (m_iMaxDistance / 2),
			m_iMaxDistance, m_iMaxDistance,
			m_hMemMaskDC, 0, 0, SRCAND);
		BitBlt(g_hOffScreenDC, 
			m_tmpObj.m_pos.x - (m_iMaxDistance / 2), 
			m_tmpObj.m_pos.y - (m_iMaxDistance / 2),
			m_iMaxDistance, m_iMaxDistance,
			m_hMemColorDC, 0, 0, SRCINVERT);
		BitBlt(g_hOffScreenDC, 
			m_tmpObj.m_pos.x - (m_iMaxDistance / 2), 
			m_tmpObj.m_pos.y - (m_iMaxDistance / 2),
			m_iMaxDistance, m_iMaxDistance,
			m_hMemMaskDC, 0, 0, SRCINVERT);

		//m_tmpObj.Render();
		SelectObject(m_hMemMaskDC, hOldMask);
		SelectObject(m_hMemColorDC, hOldColor);
#endif
		return true;
	}
	bool Release()
	{
		m_pLobbyScene->Release();
		m_pGameScene->Release();
		m_pEndScene->Release();

#ifdef USE_BITMAP_ROTATE
		DeleteObject(m_hbrBack);
		DeleteObject(m_hColorRotateBitmap);
		DeleteObject(m_hMaskRotateBitmap);
		ReleaseDC(g_hWnd, m_hRotationDC);
		ReleaseDC(g_hWnd, m_hMemMaskDC);
		ReleaseDC(g_hWnd, m_hMemColorDC);
#endif
		return true;
	}
public:
	Sample() {}
	~Sample()
	{
	}
};

GAMERUN("SampleRotationBitmap", 800, 600);