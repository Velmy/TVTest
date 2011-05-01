/*
	TVTest �v���O�C���T���v��

	�ԑg�\��\������
*/


#include <windows.h>
#include <tchar.h>
#include <vector>
#define TVTEST_PLUGIN_CLASS_IMPLEMENT	// �N���X�Ƃ��Ď���
#include "TVTestPlugin.h"


// �E�B���h�E�N���X��
#define MINI_PROGRAM_GUIDE_WINDOW_CLASS TEXT("TV Mini Program Guide Window")

// �R���g���[���̎��ʎq
#define IDC_TUNERLIST	100
#define IDC_CHANNELLIST	101
#define IDC_EVENTLIST	102

// ���ڂ̗]��
#define ITEM_MARGIN 2
// �ԑg������̍s��
#define LINES_PER_EVENT 3


// �v���O�C���N���X
class CMiniProgramGuide : public TVTest::CTVTestPlugin
{
	struct Position {
		int Left,Top,Width,Height;
		Position() : Left(0), Top(0), Width(200), Height(400) {}
	};

	HWND m_hwnd;
	HWND m_hwndTunerList;
	HWND m_hwndChannelList;
	HWND m_hwndEventList;
	Position m_WindowPosition;
	COLORREF m_crBackColor;
	COLORREF m_crTextColor;
	HBRUSH m_hbrBackground;
	TVTest::EpgEventList m_EventList;

	bool Enable(bool fEnable);
	void SetTunerList();
	void SetChannelList();
	void SetEventList();
	void GetColors();

	static LRESULT CALLBACK EventCallback(UINT Event,LPARAM lParam1,LPARAM lParam2,void *pClientData);
	static CMiniProgramGuide *GetThis(HWND hwnd);
	static LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

public:
	CMiniProgramGuide();
	virtual bool GetPluginInfo(TVTest::PluginInfo *pInfo);
	virtual bool Initialize();
	virtual bool Finalize();
};


CMiniProgramGuide::CMiniProgramGuide()
	: m_hwnd(NULL)
	, m_hwndTunerList(NULL)
	, m_hwndChannelList(NULL)
	, m_hwndEventList(NULL)
	, m_hbrBackground(NULL)
{
	m_EventList.NumEvents = 0;
	m_EventList.EventList = NULL;
}


// �v���O�C���̏���Ԃ�
bool CMiniProgramGuide::GetPluginInfo(TVTest::PluginInfo *pInfo)
{
	pInfo->Type           = TVTest::PLUGIN_TYPE_NORMAL;
	pInfo->Flags          = 0;
	pInfo->pszPluginName  = L"�~�j�ԑg�\";
	pInfo->pszCopyright   = L"Public Domain";
	pInfo->pszDescription = L"�������ԑg�\��\�����܂��B";
	return true;
}


// ����������
bool CMiniProgramGuide::Initialize()
{
	// �C�x���g�R�[���o�b�N�֐���o�^
	m_pApp->SetEventCallback(EventCallback, this);

	return true;
}


// �I������
bool CMiniProgramGuide::Finalize()
{
	// �E�B���h�E��j������
	if (m_hwnd)
		::DestroyWindow(m_hwnd);

	return true;
}


// �C�x���g�R�[���o�b�N�֐�
// �����C�x���g���N����ƌĂ΂��
LRESULT CALLBACK CMiniProgramGuide::EventCallback(UINT Event,LPARAM lParam1,LPARAM lParam2,void *pClientData)
{
	CMiniProgramGuide *pThis=static_cast<CMiniProgramGuide*>(pClientData);

	switch (Event) {
	case TVTest::EVENT_PLUGINENABLE:
		// �v���O�C���̗L����Ԃ��ω�����
		return pThis->Enable(lParam1 != 0);

	case TVTest::EVENT_STANDBY:
		// �ҋ@��Ԃ��ω�����
		if (pThis->m_pApp->IsPluginEnabled()) {
			// �ҋ@��Ԃ̎��̓E�B���h�E���B��
			::ShowWindow(pThis->m_hwnd, lParam1 != 0 ? SW_HIDE : SW_SHOW);
		}
		return TRUE;

	case TVTest::EVENT_COLORCHANGE:
		// �F�̐ݒ肪�ω�����
		if (pThis->m_hwndEventList != NULL) {
			pThis->GetColors();
			::RedrawWindow(pThis->m_hwndEventList, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		return TRUE;

	case TVTest::EVENT_DRIVERCHANGE:
		// BonDriver ���ς����
		if (pThis->m_hwnd != NULL) {
			pThis->SetTunerList();
			pThis->SetChannelList();
			pThis->SetEventList();
		}
		return TRUE;
	}
	return 0;
}


// �L����Ԃ��ς�������̏���
bool CMiniProgramGuide::Enable(bool fEnable)
{
	if (fEnable) {
		static bool fInitialized = false;

		if (!fInitialized) {
			// �E�B���h�E�N���X�̓o�^
			WNDCLASS wc;

			wc.style         = 0;
			wc.lpfnWndProc   = WndProc;
			wc.cbClsExtra    = 0;
			wc.cbWndExtra    = 0;
			wc.hInstance     = g_hinstDLL;
			wc.hIcon         = NULL;
			wc.hCursor       = ::LoadCursor(NULL,IDC_ARROW);
			wc.hbrBackground = NULL;
			wc.lpszMenuName  = NULL;
			wc.lpszClassName = MINI_PROGRAM_GUIDE_WINDOW_CLASS;
			if (::RegisterClass(&wc) == 0)
				return false;
			fInitialized = true;
		}

		if (m_hwnd == NULL) {
			// �E�B���h�E�̍쐬
			if (::CreateWindowEx(WS_EX_TOOLWINDOW,
								 MINI_PROGRAM_GUIDE_WINDOW_CLASS,
								 TEXT("�~�j�ԑg�\"),
								 WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,
								 m_WindowPosition.Left, m_WindowPosition.Top,
								 m_WindowPosition.Width, m_WindowPosition.Height,
								 m_pApp->GetAppWindow(), NULL, g_hinstDLL, this) == NULL)
				return false;
		}

		::ShowWindow(m_hwnd, SW_SHOWNORMAL);
	} else {
		// �E�B���h�E�̔j��
		if (m_hwnd)
			::DestroyWindow(m_hwnd);
	}

	return true;
}


// �`���[�j���O��Ԃ̃��X�g��ݒ肷��
void CMiniProgramGuide::SetTunerList()
{
	::SendMessage(m_hwndTunerList, CB_RESETCONTENT, 0, 0);

	WCHAR szName[64];
	for (int i = 0; m_pApp->GetTuningSpaceName(i, szName, sizeof(szName) / sizeof(WCHAR)) > 0; i++) {
		::SendMessage(m_hwndTunerList, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(szName));
	}

	::SendMessage(m_hwndTunerList, CB_SETCURSEL, m_pApp->GetTuningSpace(), 0);
}


// �`�����l���̃��X�g��ݒ肷��
void CMiniProgramGuide::SetChannelList()
{
	::SendMessage(m_hwndChannelList, CB_RESETCONTENT, 0, 0);

	int CurTuningSpace = (int)::SendMessage(m_hwndTunerList, CB_GETCURSEL, 0, 0);
	if (CurTuningSpace >= 0) {
		TVTest::ChannelInfo CurChannelInfo,ChannelInfo;

		bool fCurChannel = m_pApp->GetCurrentChannelInfo(&CurChannelInfo);

		int Sel = -1;
		for (int i = 0; m_pApp->GetChannelInfo(CurTuningSpace, i, &ChannelInfo); i++) {
			if ((ChannelInfo.Flags&TVTest::CHANNEL_FLAG_DISABLED) == 0) {
				int Index = (int)::SendMessage(m_hwndChannelList, CB_ADDSTRING, 0,
											   reinterpret_cast<LPARAM>(ChannelInfo.szChannelName));
				::SendMessage(m_hwndChannelList, CB_SETITEMDATA, Index, i);
				if (Sel < 0 && fCurChannel
						&& ChannelInfo.Channel == CurChannelInfo.Channel
						&& ChannelInfo.ServiceID == CurChannelInfo.ServiceID)
					Sel = Index;
			}
		}

		if (Sel >= 0)
			::SendMessage(m_hwndChannelList, CB_SETCURSEL, Sel, 0);
	}
}


// �ԑg�̃��X�g��ݒ肷��
void CMiniProgramGuide::SetEventList()
{
	if (m_EventList.EventList != NULL)
		m_pApp->FreeEpgEventList(&m_EventList);

	::SendMessage(m_hwndEventList, LB_RESETCONTENT, 0, 0);

	int TuningSpace = (int)::SendMessage(m_hwndTunerList, CB_GETCURSEL, 0, 0);
	int Channel = (int)::SendMessage(m_hwndChannelList, CB_GETCURSEL, 0, 0);
	if (TuningSpace >= 0 && Channel >= 0) {
		TVTest::ChannelInfo ChannelInfo;

		Channel = (int)::SendMessage(m_hwndChannelList, CB_GETITEMDATA, Channel, 0);
		if (m_pApp->GetChannelInfo(TuningSpace, Channel, &ChannelInfo)) {
			m_EventList.NetworkID = ChannelInfo.NetworkID;
			m_EventList.TransportStreamID = ChannelInfo.TransportStreamID;
			m_EventList.ServiceID = ChannelInfo.ServiceID;
			if (m_pApp->GetEpgEventList(&m_EventList)) {
				for (WORD i = 0; i < m_EventList.NumEvents; i++) {
					::SendMessage(m_hwndEventList, LB_ADDSTRING, 0,
								  reinterpret_cast<LPARAM>(m_EventList.EventList[i]));
				}
			}
		}
	}
}


// �z�F���擾����
void CMiniProgramGuide::GetColors()
{
	m_crBackColor = m_pApp->GetColor(L"ProgramGuideBack");
	m_crTextColor = m_pApp->GetColor(L"ProgramGuideText");

	if (m_hbrBackground != NULL)
		::DeleteObject(m_hbrBackground);
	m_hbrBackground = ::CreateSolidBrush(m_crBackColor);
}


// �E�B���h�E�n���h������this���擾����
CMiniProgramGuide *CMiniProgramGuide::GetThis(HWND hwnd)
{
	return reinterpret_cast<CMiniProgramGuide*>(::GetWindowLongPtr(hwnd,GWLP_USERDATA));
}


// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK CMiniProgramGuide::WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		{
			LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			CMiniProgramGuide *pThis = static_cast<CMiniProgramGuide*>(pcs->lpCreateParams);

			::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
			pThis->m_hwnd = hwnd;

			HFONT hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));

			pThis->m_hwndTunerList = ::CreateWindowEx(0, TEXT("COMBOBOX"), NULL,
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
				0, 0, 0, 200, hwnd, reinterpret_cast<HMENU>(IDC_TUNERLIST), g_hinstDLL, NULL);
			::SendMessage(pThis->m_hwndTunerList, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 0);

			pThis->m_hwndChannelList = ::CreateWindowEx(0, TEXT("COMBOBOX"), NULL,
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
				0, 0, 0, 200, hwnd, reinterpret_cast<HMENU>(IDC_CHANNELLIST), g_hinstDLL, NULL);
			::SendMessage(pThis->m_hwndChannelList, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 0);

			pThis->m_hwndEventList = ::CreateWindowEx(0, TEXT("LISTBOX"), NULL,
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT,
				0, 0, 0, 0, hwnd, reinterpret_cast<HMENU>(IDC_EVENTLIST), g_hinstDLL, NULL);
			::SendMessage(pThis->m_hwndEventList, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 0);

			// �A�C�e���̑傫����ݒ肷��
			HDC hdc = ::GetDC(pThis->m_hwndEventList);
			HGDIOBJ hOldFont = ::SelectObject(hdc, hFont);
			TEXTMETRIC tm;
			::GetTextMetrics(hdc, &tm);
			::SelectObject(hdc, hOldFont);
			::ReleaseDC(pThis->m_hwndEventList, hdc);
			::SendMessage(pThis->m_hwndEventList, LB_SETITEMHEIGHT, 0,
						  ITEM_MARGIN * 2 + tm.tmHeight * LINES_PER_EVENT);

			pThis->GetColors();

			pThis->SetTunerList();
			pThis->SetChannelList();
			pThis->SetEventList();
		}
		return 0;

	case WM_SIZE:
		{
			CMiniProgramGuide *pThis = GetThis(hwnd);
			const int Width = LOWORD(lParam), Height = HIWORD(lParam);
			int y;
			RECT rc;

			::GetWindowRect(pThis->m_hwndTunerList, &rc);
			::MoveWindow(pThis->m_hwndTunerList, 0, 0, Width, rc.bottom - rc.top, TRUE);
			y = rc.bottom - rc.top;
			::GetWindowRect(pThis->m_hwndChannelList, &rc);
			::MoveWindow(pThis->m_hwndChannelList, 0, y, Width, rc.bottom - rc.top, TRUE);
			y += rc.bottom - rc.top;
			::MoveWindow(pThis->m_hwndEventList, 0, y, Width, max(Height - y, 0), TRUE);
			::InvalidateRect(pThis->m_hwndEventList, NULL, TRUE);
		}
		return 0;

	case WM_DRAWITEM:
		// �ԑg�̃��X�g�̃A�C�e����`��
		{
			CMiniProgramGuide *pThis = GetThis(hwnd);
			LPDRAWITEMSTRUCT pdis = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

			if ((int)pdis->itemID < 0 || pdis->itemID >= pThis->m_EventList.NumEvents) {
				::FillRect(pdis->hDC, &pdis->rcItem, pThis->m_hbrBackground);
				return TRUE;
			}

			const TVTest::EpgEventInfo *pEventInfo = reinterpret_cast<TVTest::EpgEventInfo*>(pdis->itemData);

			int OldBkMode = ::SetBkMode(pdis->hDC, TRANSPARENT);
			COLORREF OldTextColor = ::SetTextColor(pdis->hDC, pThis->m_crTextColor);

			static const LPCWSTR ContentColorList[] = {
				L"EPGContentNews",
				L"EPGContentSports",
				L"EPGContentInformation",
				L"EPGContentDrama",
				L"EPGContentMusic",
				L"EPGContentVariety",
				L"EPGContentMovie",
				L"EPGContentAnime",
				L"EPGContentDocumentary",
				L"EPGContentTheater",
				L"EPGContentEducation",
				L"EPGContentWelfare",
			};
			COLORREF cr;
			if (pEventInfo->ContentListLength > 0
					&& pEventInfo->ContentList[0].ContentNibbleLevel1 <= 11)
				cr=pThis->m_pApp->GetColor(ContentColorList[pEventInfo->ContentList[0].ContentNibbleLevel1]);
			else
				cr=pThis->m_pApp->GetColor(L"EPGContentOther");
			HBRUSH hbr = ::CreateSolidBrush(cr);
			::FillRect(pdis->hDC, &pdis->rcItem, hbr);
			::DeleteObject(hbr);

			RECT rc = pdis->rcItem;
			::InflateRect(&rc, -ITEM_MARGIN, -ITEM_MARGIN);

			TCHAR szText[256];
			::wsprintf(szText, TEXT("%d/%02d/%02d %02d:%02d %s"),
					   pEventInfo->StartTime.wYear,
					   pEventInfo->StartTime.wMonth,
					   pEventInfo->StartTime.wDay,
					   pEventInfo->StartTime.wHour,
					   pEventInfo->StartTime.wMinute,
					   pEventInfo->pszEventName != NULL?
					   pEventInfo->pszEventName : TEXT(""));

			::DrawText(pdis->hDC, szText, -1, &rc,
					   DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);

			::SetTextColor(pdis->hDC, OldTextColor);
			::SetBkMode(pdis->hDC, OldBkMode);
		}
		return TRUE;

	case WM_CTLCOLORLISTBOX:
		{
			CMiniProgramGuide *pThis = GetThis(hwnd);

			if (reinterpret_cast<HWND>(lParam) == pThis->m_hwndEventList)
				return reinterpret_cast<LRESULT>(pThis->m_hbrBackground);
		}
		break;

	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_CLOSE) {
			// ���鎞�̓v���O�C���𖳌��ɂ���
			CMiniProgramGuide *pThis = GetThis(hwnd);

			pThis->m_pApp->EnablePlugin(false);
			return TRUE;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_TUNERLIST:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				CMiniProgramGuide *pThis = GetThis(hwnd);

				pThis->SetChannelList();
			}
			return 0;

		case IDC_CHANNELLIST:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				CMiniProgramGuide *pThis = GetThis(hwnd);

				pThis->SetEventList();
			}
			return 0;
		}
		return 0;

	case WM_NCDESTROY:
		{
			CMiniProgramGuide *pThis = GetThis(hwnd);

			if (pThis->m_EventList.EventList != NULL)
				pThis->m_pApp->FreeEpgEventList(&pThis->m_EventList);

			// �E�B���h�E�ʒu�̋L��
			RECT rc;
			::GetWindowRect(hwnd, &rc);
			pThis->m_WindowPosition.Left = rc.left;
			pThis->m_WindowPosition.Top = rc.top;
			pThis->m_WindowPosition.Width = rc.right - rc.left;
			pThis->m_WindowPosition.Height = rc.bottom - rc.top;

			pThis->m_hwnd = NULL;
			pThis->m_hwndTunerList = NULL;
			pThis->m_hwndChannelList = NULL;
			pThis->m_hwndEventList = NULL;

			if (pThis->m_hbrBackground != NULL) {
				::DeleteObject(pThis->m_hbrBackground);
				pThis->m_hbrBackground = NULL;
			}
		}
		return 0;
	}
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}




// �v���O�C���N���X�̃C���X�^���X�𐶐�����
TVTest::CTVTestPlugin *CreatePluginClass()
{
	return new CMiniProgramGuide;
}