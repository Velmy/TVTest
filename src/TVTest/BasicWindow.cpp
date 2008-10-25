#include "stdafx.h"
#include "TVTest.h"
#include "BasicWindow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CBasicWindow::CBasicWindow()
{
	m_hwnd=NULL;
	m_WindowPosition.Left=0;
	m_WindowPosition.Top=0;
	m_WindowPosition.Width=0;
	m_WindowPosition.Height=0;
}


CBasicWindow::~CBasicWindow()
{
	Destroy();
}


void CBasicWindow::Destroy()
{
	if (m_hwnd!=NULL)
		::DestroyWindow(m_hwnd);
}


bool CBasicWindow::SetPosition(int Left,int Top,int Width,int Height)
{
	if (Width<0 || Height<0)
		return false;
	if (m_hwnd!=NULL) {
		::MoveWindow(m_hwnd,Left,Top,Width,Height,TRUE);
	} else {
		m_WindowPosition.Left=Left;
		m_WindowPosition.Top=Top;
		m_WindowPosition.Width=Width;
		m_WindowPosition.Height=Height;
	}
	return true;
}


bool CBasicWindow::SetPosition(const RECT *pPosition)
{
	return SetPosition(pPosition->left,pPosition->top,
						pPosition->right-pPosition->left,
						pPosition->bottom-pPosition->top);
}


void CBasicWindow::GetPosition(int *pLeft,int *pTop,int *pWidth,int *pHeight) const
{
	if (m_hwnd!=NULL) {
		RECT rc;

		if ((GetWindowStyle(m_hwnd)&WS_CHILD)!=0) {
			::GetWindowRect(m_hwnd,&rc);
			::MapWindowPoints(NULL,::GetParent(m_hwnd),reinterpret_cast<POINT*>(&rc),2);
			if (pLeft)
				*pLeft=rc.left;
			if (pTop)
				*pTop=rc.top;
			if (pWidth)
				*pWidth=rc.right-rc.left;
			if (pHeight)
				*pHeight=rc.bottom-rc.top;
		} else {
			WINDOWPLACEMENT wc;

			wc.length=sizeof(WINDOWPLACEMENT);
			::GetWindowPlacement(m_hwnd,&wc);
			if (wc.showCmd==SW_SHOWNORMAL) {
				// 通常表示時はGetWindowRectの方が座標変換の問題がないので確実
				::GetWindowRect(m_hwnd,&rc);
			} else {
				/*
					WS_EX_TOOLWINDOWスタイルが付いていない場合は、
					rcNormalPositionはワークスペース座標になる(仕様が意味不明...)
				*/
				if ((GetWindowExStyle(m_hwnd)&WS_EX_TOOLWINDOW)==0) {
					/*
						ワークスペース座標をスクリーン座標に変換
						しかし、マルチモニタの時はどのモニタのワークスペース座標が
						基準になっているのか不明...
					*/
					HMONITOR hMonitor=::MonitorFromWindow(m_hwnd,MONITOR_DEFAULTTONEAREST);
					MONITORINFO mi;

					mi.cbSize=sizeof(MONITORINFO);
					::GetMonitorInfo(hMonitor,&mi);
					::OffsetRect(&wc.rcNormalPosition,
											mi.rcWork.left-mi.rcMonitor.left,
											mi.rcWork.top-mi.rcMonitor.top);
				}
				rc=wc.rcNormalPosition;
			}
			if (pLeft)
				*pLeft=rc.left;
			if (pTop)
				*pTop=rc.top;
			if (pWidth)
				*pWidth=rc.right-rc.left;
			if (pHeight)
				*pHeight=rc.bottom-rc.top;
		}
	} else {
		if (pLeft)
			*pLeft=m_WindowPosition.Left;
		if (pTop)
			*pTop=m_WindowPosition.Top;
		if (pWidth)
			*pWidth=m_WindowPosition.Width;
		if (pHeight)
			*pHeight=m_WindowPosition.Height;
	}
}


void CBasicWindow::GetPosition(RECT *pPosition) const
{
	int Left,Top,Width,Height;

	GetPosition(&Left,&Top,&Width,&Height);
	::SetRect(pPosition,Left,Top,Left+Width,Top+Height);
}


bool CBasicWindow::GetScreenPosition(RECT *pPosition) const
{
	if (m_hwnd==NULL) {
		GetPosition(pPosition);
		return true;
	}
	return ::GetWindowRect(m_hwnd,pPosition)!=FALSE;
}


void CBasicWindow::SetVisible(bool fVisible)
{
	if (m_hwnd!=NULL)
		::ShowWindow(m_hwnd,fVisible?SW_SHOW:SW_HIDE);
}


bool CBasicWindow::GetVisible() const
{
	return m_hwnd!=NULL && ::IsWindowVisible(m_hwnd);
}


bool CBasicWindow::GetMaximize() const
{
	return m_hwnd!=NULL && ::IsZoomed(m_hwnd);
}


bool CBasicWindow::Invalidate(bool fErase)
{
	return m_hwnd!=NULL && ::InvalidateRect(m_hwnd,NULL,fErase);
}


bool CBasicWindow::Update()
{
	return m_hwnd!=NULL && ::UpdateWindow(m_hwnd);
}


bool CBasicWindow::GetClientRect(RECT *pRect) const
{
	return m_hwnd!=NULL && ::GetClientRect(m_hwnd,pRect);
}


bool CBasicWindow::GetClientSize(SIZE *pSize) const
{
	RECT rc;

	if (m_hwnd==NULL || !::GetClientRect(m_hwnd,&rc))
		return false;
	pSize->cx=rc.right;
	pSize->cy=rc.bottom;
	return true;
}


bool CBasicWindow::SetParent(HWND hwnd)
{
	return m_hwnd!=NULL && ::SetParent(m_hwnd,hwnd);
}


bool CBasicWindow::SetParent(CBasicWindow *pWindow)
{
	return m_hwnd!=NULL && ::SetParent(m_hwnd,pWindow->m_hwnd);
}


HWND CBasicWindow::GetParent() const
{
	if (m_hwnd==NULL)
		return NULL;
	return ::GetParent(m_hwnd);
}


bool CBasicWindow::MoveToMonitorInside()
{
	RECT rc;
	HMONITOR hMonitor;
	MONITORINFO mi;

	GetPosition(&rc);
	hMonitor=::MonitorFromRect(&rc,MONITOR_DEFAULTTONEAREST);
	mi.cbSize=sizeof(MONITORINFO);
	::GetMonitorInfo(hMonitor,&mi);
	if (rc.left>=mi.rcMonitor.right || rc.top>=mi.rcMonitor.bottom
			|| rc.right<=mi.rcMonitor.left || rc.bottom<=mi.rcMonitor.top) {
		int XOffset,YOffset;

		if (rc.left>=mi.rcMonitor.right)
			XOffset=mi.rcMonitor.right-rc.right;
		else if (rc.right<=mi.rcMonitor.left)
			XOffset=mi.rcMonitor.left-rc.left;
		if (rc.top>=mi.rcMonitor.bottom)
			YOffset=mi.rcMonitor.bottom-rc.bottom;
		else if (rc.bottom<=mi.rcMonitor.top)
			YOffset=mi.rcMonitor.top-rc.top;
		::OffsetRect(&rc,XOffset,YOffset);
		SetPosition(&rc);
		return true;
	}
	return false;
}


DWORD CBasicWindow::GetStyle() const
{
	if (m_hwnd==NULL)
		return 0;
	return ::GetWindowLong(m_hwnd,GWL_STYLE);
}


bool CBasicWindow::SetStyle(DWORD Style,bool fFrameChange)
{
	if (m_hwnd==NULL)
		return false;
	::SetWindowLong(m_hwnd,GWL_STYLE,Style);
	if (fFrameChange)
		::SetWindowPos(m_hwnd,NULL,0,0,0,0,
			SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
	return true;
}


DWORD CBasicWindow::GetExStyle() const
{
	if (m_hwnd==NULL)
		return 0;
	return ::GetWindowLong(m_hwnd,GWL_EXSTYLE);
}


bool CBasicWindow::SetExStyle(DWORD ExStyle,bool fFrameChange)
{
	if (m_hwnd==NULL)
		return false;
	::SetWindowLong(m_hwnd,GWL_EXSTYLE,ExStyle);
	if (fFrameChange)
		::SetWindowPos(m_hwnd,NULL,0,0,0,0,
			SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
	return true;
}


bool CBasicWindow::CreateBasicWindow(HWND hwndParent,DWORD Style,DWORD ExStyle,
				int ID,LPCTSTR pszClassName,LPCTSTR pszText,HINSTANCE hinst)
{
	if (m_hwnd!=NULL)
		return false;
	m_hwnd=::CreateWindowEx(ExStyle,pszClassName,pszText,Style,
		m_WindowPosition.Left,m_WindowPosition.Top,
		m_WindowPosition.Width,m_WindowPosition.Height,
		hwndParent,reinterpret_cast<HMENU>(ID),hinst,this);
	return m_hwnd!=NULL;
}


CBasicWindow *CBasicWindow::OnCreate(HWND hwnd,LPARAM lParam)
{
	LPCREATESTRUCT pcs=reinterpret_cast<LPCREATESTRUCT>(lParam);
	CBasicWindow *pWindow=static_cast<CBasicWindow*>(pcs->lpCreateParams);

	pWindow->m_hwnd=hwnd;
	::SetWindowLongPtr(hwnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(pWindow));
	return pWindow;
}


void CBasicWindow::OnDestroy()
{
	GetPosition(&m_WindowPosition.Left,&m_WindowPosition.Top,
				&m_WindowPosition.Width,&m_WindowPosition.Height);
	SetWindowLongPtr(m_hwnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>((LPVOID)NULL));
	m_hwnd=NULL;
}


CBasicWindow *CBasicWindow::GetBasicWindow(HWND hwnd)
{
	return reinterpret_cast<CBasicWindow*>(::GetWindowLongPtr(hwnd,GWLP_USERDATA));
}


LRESULT CBasicWindow::SendMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	if (m_hwnd==NULL)
		return 0;
	return ::SendMessage(m_hwnd,Msg,wParam,lParam);
}


bool CBasicWindow::PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	if (m_hwnd==NULL)
		return false;
	return ::PostMessage(m_hwnd,Msg,wParam,lParam)!=FALSE;
}
