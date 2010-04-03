#include "stdafx.h"
#include "TVTest.h"
#include "AppMain.h"
#include "EpgOptions.h"
#include "DialogUtil.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CEpgOptions::CEpgOptions(CCoreEngine *pCoreEngine,CLogoManager *pLogoManager)
	: m_pCoreEngine(pCoreEngine)
	, m_pLogoManager(pLogoManager)
	, m_hLoadThread(NULL)
	, m_pEpgDataLoader(NULL)
	, m_EpgDataLoaderEventHandler(&pCoreEngine->m_DtvEngine.m_TsPacketParser)
{
	::lstrcpy(m_szEpgDataCapDllPath,TEXT("EpgDataCap2.dll"));
	m_fSaveEpgFile=true;
	::lstrcpy(m_szEpgFileName,TEXT("EpgData"));
	m_fUpdateWhenStandby=false;
	m_fUseEpgData=false;
	if (::SHGetSpecialFolderPath(NULL,m_szEpgDataFolder,CSIDL_PERSONAL,FALSE))
		::PathAppend(m_szEpgDataFolder,TEXT("EpgTimerBon\\EpgData"));
	else
		m_szEpgDataFolder[0]='\0';

	m_fSaveLogoFile=true;
	::lstrcpy(m_szLogoFileName,TEXT("LogoData"));

	NONCLIENTMETRICS ncm;
#if WINVER<0x0600
	ncm.cbSize=sizeof(ncm);
#else
	ncm.cbSize=offsetof(NONCLIENTMETRICS,iPaddedBorderWidth);
#endif
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,ncm.cbSize,&ncm,0);
	m_EventInfoFont=ncm.lfMessageFont;
}


CEpgOptions::~CEpgOptions()
{
	Finalize();
}


void CEpgOptions::Finalize()
{
	if (m_hLoadThread) {
		GetAppClass().AddLog(TEXT("EPGデータ読み込みスレッドの終了を待っています..."));
		if (::WaitForSingleObject(m_hLoadThread,30000)!=WAIT_OBJECT_0)
			::TerminateThread(m_hLoadThread,-1);
		::CloseHandle(m_hLoadThread);
		m_hLoadThread=NULL;
	}
	SAFE_DELETE(m_pEpgDataLoader);
}


bool CEpgOptions::Read(CSettings *pSettings)
{
	pSettings->Read(TEXT("EpgDataCapDll"),m_szEpgDataCapDllPath,lengthof(m_szEpgDataCapDllPath));
	pSettings->Read(TEXT("SaveEpgData"),&m_fSaveEpgFile);
	pSettings->Read(TEXT("EpgDataFileName"),m_szEpgFileName,lengthof(m_szEpgFileName));
	pSettings->Read(TEXT("EpgUpdateWhenStandby"),&m_fUpdateWhenStandby);
	pSettings->Read(TEXT("UseEpgData"),&m_fUseEpgData);
	pSettings->Read(TEXT("EpgDataFolder"),m_szEpgDataFolder,lengthof(m_szEpgDataFolder));

	pSettings->Read(TEXT("SaveLogoData"),&m_fSaveLogoFile);
	pSettings->Read(TEXT("LogoDataFileName"),m_szLogoFileName,lengthof(m_szLogoFileName));

	bool fSaveLogo;
	if (pSettings->Read(TEXT("SaveRawLogo"),&fSaveLogo))
		m_pLogoManager->SetSaveLogo(fSaveLogo);
	if (pSettings->Read(TEXT("SaveBmpLogo"),&fSaveLogo))
		m_pLogoManager->SetSaveLogoBmp(fSaveLogo);
	TCHAR szLogoDir[MAX_PATH];
	if (pSettings->Read(TEXT("LogoDirectory"),szLogoDir,MAX_PATH)) {
		m_pLogoManager->SetLogoDirectory(szLogoDir);
	} else {
		// TVLogoMark のロゴがあれば利用する
		GetAppClass().GetAppDirectory(szLogoDir);
		::PathAppend(szLogoDir,TEXT("Plugins\\Logo"));
		if (::PathIsDirectory(szLogoDir))
			m_pLogoManager->SetLogoDirectory(TEXT(".\\Plugins\\Logo"));
	}

	pSettings->Read(TEXT("EventInfoFont"),&m_EventInfoFont);
	return true;
}


bool CEpgOptions::Write(CSettings *pSettings) const
{
	pSettings->Write(TEXT("EpgDataCapDll"),m_szEpgDataCapDllPath);
	pSettings->Write(TEXT("SaveEpgData"),m_fSaveEpgFile);
	pSettings->Write(TEXT("EpgDataFileName"),m_szEpgFileName);
	pSettings->Write(TEXT("EpgUpdateWhenStandby"),m_fUpdateWhenStandby);
	pSettings->Write(TEXT("UseEpgData"),m_fUseEpgData);
	pSettings->Write(TEXT("EpgDataFolder"),m_szEpgDataFolder);

	pSettings->Write(TEXT("SaveLogoData"),m_fSaveLogoFile);
	pSettings->Write(TEXT("LogoDataFileName"),m_szLogoFileName);
	pSettings->Write(TEXT("SaveRawLogo"),m_pLogoManager->GetSaveLogo());
	pSettings->Write(TEXT("SaveBmpLogo"),m_pLogoManager->GetSaveLogoBmp());
	pSettings->Write(TEXT("LogoDirectory"),m_pLogoManager->GetLogoDirectory());

	pSettings->Write(TEXT("EventInfoFont"),&m_EventInfoFont);
	return true;
}


bool CEpgOptions::InitializeEpgDataCap()
{
	if (m_szEpgDataCapDllPath[0]=='\0')
		return true;

	TCHAR szPath[MAX_PATH];

	if (!::PathIsFileSpec(m_szEpgDataCapDllPath)
			&& ::PathIsRelative(m_szEpgDataCapDllPath)) {
		TCHAR szTemp[MAX_PATH];

		GetAppClass().GetAppDirectory(szTemp);
		::PathAppend(szTemp,m_szEpgDataCapDllPath);
		::PathCanonicalize(szPath,szTemp);
	} else {
		::lstrcpy(szPath,m_szEpgDataCapDllPath);
	}
	GetAppClass().AddLog(TEXT("EpgDataCap を初期化します... (DLL : \"%s\")"),szPath);
	return m_pCoreEngine->m_DtvEngine.m_TsPacketParser.InitializeEpgDataCap(szPath);
}


bool CEpgOptions::LoadEpgFile(CEpgProgramList *pEpgList)
{
	bool fOK=true;

	if (m_fSaveEpgFile) {
		TCHAR szFileName[MAX_PATH];

		if (!GetAbsolutePath(m_szEpgFileName,szFileName,lengthof(szFileName)))
			return false;
		if (::PathFileExists(szFileName)) {
			GetAppClass().AddLog(TEXT("EPG データを \"%s\" から読み込みます..."),szFileName);
			fOK=pEpgList->LoadFromFile(szFileName);
		}
	}
	return fOK;
}


struct EpgLoadInfo {
	CEpgProgramList *pList;
	TCHAR szFileName[MAX_PATH];
};

bool CEpgOptions::AsyncLoadEpgFile(CEpgProgramList *pEpgList)
{
	if (m_fSaveEpgFile) {
		TCHAR szFileName[MAX_PATH];

		if (!GetAbsolutePath(m_szEpgFileName,szFileName,lengthof(szFileName)))
			return false;
		if (::PathFileExists(szFileName)) {
			GetAppClass().AddLog(TEXT("EPG データを \"%s\" から読み込みます..."),szFileName);

			EpgLoadInfo *pInfo=new EpgLoadInfo;

			pInfo->pList=pEpgList;
			::lstrcpy(pInfo->szFileName,szFileName);
			m_hLoadThread=::CreateThread(NULL,0,LoadThread,pInfo,0,NULL);
			if (m_hLoadThread==NULL) {
				delete pInfo;
				return pEpgList->LoadFromFile(szFileName);
			}
		}
	}
	return true;
}


DWORD WINAPI CEpgOptions::LoadThread(LPVOID lpParameter)
{
	EpgLoadInfo *pInfo=static_cast<EpgLoadInfo*>(lpParameter);

	::SetThreadPriority(::GetCurrentThread(),THREAD_PRIORITY_LOWEST);
	bool fOK=pInfo->pList->LoadFromFile(pInfo->szFileName);
	delete pInfo;
	return fOK;
}


bool CEpgOptions::SaveEpgFile(CEpgProgramList *pEpgList)
{
	bool fOK=true;

	if (m_fSaveEpgFile) {
		TCHAR szFileName[MAX_PATH];

		if (!GetAbsolutePath(m_szEpgFileName,szFileName,lengthof(szFileName)))
			return false;
		if (pEpgList->NumServices()>0) {
			GetAppClass().AddLog(TEXT("EPG データを \"%s\" に保存します..."),szFileName);
			fOK=pEpgList->SaveToFile(szFileName);
			if (!fOK)
				::DeleteFile(szFileName);
		} else {
			if (::DeleteFile(szFileName)
					|| ::GetLastError()==ERROR_FILE_NOT_FOUND)
				fOK=true;
		}
	}
	return fOK;
}


bool CEpgOptions::LoadEpgData()
{
	bool fOK=true;

	if (m_fUseEpgData && m_szEpgDataFolder[0]!='\0'
			&& m_pCoreEngine->m_DtvEngine.m_TsPacketParser.IsEpgDataCapLoaded()) {
		CEpgDataLoader Loader(m_pCoreEngine->m_DtvEngine.m_TsPacketParser.GetEpgDataCapDllUtil());

		fOK=Loader.Load(m_szEpgDataFolder);
	}
	return fOK;
}


bool CEpgOptions::AsyncLoadEpgData(CEpgLoadEventHandler *pEventHandler)
{
	bool fOK=true;

	if (m_fUseEpgData && m_szEpgDataFolder[0]!='\0'
			&& m_pCoreEngine->m_DtvEngine.m_TsPacketParser.IsEpgDataCapLoaded()) {
		delete m_pEpgDataLoader;
		m_pEpgDataLoader=
			new CEpgDataLoader(m_pCoreEngine->m_DtvEngine.m_TsPacketParser.GetEpgDataCapDllUtil());

		m_EpgDataLoaderEventHandler.SetEventHandler(pEventHandler);
		fOK=m_pEpgDataLoader->LoadAsync(m_szEpgDataFolder,&m_EpgDataLoaderEventHandler);
	}
	return fOK;
}


bool CEpgOptions::IsEpgDataLoading() const
{
	return m_EpgDataLoaderEventHandler.IsLoading();
}


bool CEpgOptions::LoadLogoFile()
{
	if (m_fSaveLogoFile && m_szLogoFileName[0]!='\0') {
		TCHAR szFileName[MAX_PATH];

		if (!GetAbsolutePath(m_szLogoFileName,szFileName,lengthof(szFileName)))
			return false;
		if (::PathFileExists(szFileName)) {
			GetAppClass().AddLog(TEXT("ロゴデータを \"%s\" から読み込みます..."),szFileName);
			if (!m_pLogoManager->LoadLogoFile(szFileName))
				return false;
		}
	}
	return true;
}


bool CEpgOptions::SaveLogoFile()
{
	if (m_fSaveLogoFile && m_szLogoFileName[0]!='\0'
			&& m_pLogoManager->IsLogoDataUpdated()) {
		TCHAR szFileName[MAX_PATH];

		if (!GetAbsolutePath(m_szLogoFileName,szFileName,lengthof(szFileName)))
			return false;
		GetAppClass().AddLog(TEXT("ロゴデータを \"%s\" に保存します..."),szFileName);
		if (!m_pLogoManager->SaveLogoFile(szFileName))
			return false;
	}
	return true;
}


CEpgOptions *CEpgOptions::GetThis(HWND hDlg)
{
	return static_cast<CEpgOptions*>(::GetProp(hDlg,TEXT("This")));
}


static void SetFontInfo(HWND hDlg,const LOGFONT *plf)
{
	HDC hdc;
	TCHAR szText[LF_FACESIZE+16];

	hdc=::GetDC(hDlg);
	if (hdc==NULL)
		return;
	::wsprintf(szText,TEXT("%s, %d pt"),plf->lfFaceName,CalcFontPointHeight(hdc,plf));
	::SetDlgItemText(hDlg,IDC_EVENTINFOOPTIONS_FONT_INFO,szText);
	::ReleaseDC(hDlg,hdc);
}

INT_PTR CALLBACK CEpgOptions::DlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		{
			CEpgOptions *pThis=dynamic_cast<CEpgOptions*>(OnInitDialog(hDlg,lParam));

			::SendDlgItemMessage(hDlg,IDC_EPGOPTIONS_EPGDATACAPDLLPATH,
								 EM_LIMITTEXT,MAX_PATH-1,0);
			::SetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGDATACAPDLLPATH,
												pThis->m_szEpgDataCapDllPath);
			DlgCheckBox_Check(hDlg,IDC_EPGOPTIONS_SAVEEPGFILE,pThis->m_fSaveEpgFile);
			::EnableDlgItems(hDlg,IDC_EPGOPTIONS_EPGFILENAME_LABEL,
					IDC_EPGOPTIONS_EPGFILENAME_BROWSE,pThis->m_fSaveEpgFile);
			::SendDlgItemMessage(hDlg,IDC_EPGOPTIONS_EPGFILENAME,
								 EM_LIMITTEXT,MAX_PATH-1,0);
			::SetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGFILENAME,
							 pThis->m_szEpgFileName);
			DlgCheckBox_Check(hDlg,IDC_EPGOPTIONS_UPDATEWHENSTANDBY,
							  pThis->m_fUpdateWhenStandby);
			DlgCheckBox_Check(hDlg,IDC_EPGOPTIONS_USEEPGDATA,pThis->m_fUseEpgData);
			::SendDlgItemMessage(hDlg,IDC_EPGOPTIONS_EPGDATAFOLDER,EM_LIMITTEXT,MAX_PATH-1,0);
			::SetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGDATAFOLDER,pThis->m_szEpgDataFolder);
			EnableDlgItems(hDlg,IDC_EPGOPTIONS_EPGDATAFOLDER_LABEL,
								IDC_EPGOPTIONS_EPGDATAFOLDER_BROWSE,
						   pThis->m_fUseEpgData);

			DlgCheckBox_Check(hDlg,IDC_LOGOOPTIONS_SAVEDATA,pThis->m_fSaveLogoFile);
			::SendDlgItemMessage(hDlg,IDC_LOGOOPTIONS_DATAFILENAME,EM_LIMITTEXT,MAX_PATH-1,0);
			::SetDlgItemText(hDlg,IDC_LOGOOPTIONS_DATAFILENAME,pThis->m_szLogoFileName);
			EnableDlgItems(hDlg,IDC_LOGOOPTIONS_DATAFILENAME_LABEL,
								IDC_LOGOOPTIONS_DATAFILENAME_BROWSE,
						   pThis->m_fSaveLogoFile);
			DlgCheckBox_Check(hDlg,IDC_LOGOOPTIONS_SAVERAWLOGO,pThis->m_pLogoManager->GetSaveLogo());
			DlgCheckBox_Check(hDlg,IDC_LOGOOPTIONS_SAVEBMPLOGO,pThis->m_pLogoManager->GetSaveLogoBmp());
			::SendDlgItemMessage(hDlg,IDC_LOGOOPTIONS_LOGOFOLDER,EM_LIMITTEXT,MAX_PATH-1,0);
			::SetDlgItemText(hDlg,IDC_LOGOOPTIONS_LOGOFOLDER,pThis->m_pLogoManager->GetLogoDirectory());

			pThis->m_CurEventInfoFont=pThis->m_EventInfoFont;
			SetFontInfo(hDlg,&pThis->m_CurEventInfoFont);
		}
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_EPGOPTIONS_EPGDATACAPDLLPATH_BROWSE:
			{
				OPENFILENAME ofn;
				TCHAR szFileName[MAX_PATH],szInitialDir[MAX_PATH];

				::GetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGDATACAPDLLPATH,
								 szFileName,lengthof(szFileName));
				ofn.lStructSize=sizeof(OPENFILENAME);
				ofn.hwndOwner=hDlg;
				ofn.lpstrFilter=TEXT("EpgDataCap2.dll\0EpgDataCap2.dll\0")
								TEXT("DLLファイル(*.dll)\0*.dll\0")
								TEXT("すべてのファイル\0*.*\0");
				ofn.lpstrCustomFilter=NULL;
				ofn.nFilterIndex=1;
				ofn.lpstrFile=szFileName;
				ofn.nMaxFile=lengthof(szFileName);
				ofn.lpstrFileTitle=NULL;
				if (szFileName[0]=='\0' || ::PathIsFileSpec(szFileName)) {
					GetAppClass().GetAppDirectory(szInitialDir);
					ofn.lpstrInitialDir=szInitialDir;
				} else
					ofn.lpstrInitialDir=NULL;
				ofn.lpstrTitle=TEXT("DLLファイル名");
				ofn.Flags=OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
				ofn.lpstrDefExt=TEXT("dll");
#if _WIN32_WINNT>=0x500
				ofn.pvReserved=NULL;
				ofn.dwReserved=0;
				ofn.FlagsEx=0;
#endif
				if (::GetOpenFileName(&ofn))
					::SetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGDATACAPDLLPATH,szFileName);
			}
			return TRUE;

		case IDC_EPGOPTIONS_SAVEEPGFILE:
			::EnableDlgItems(hDlg,IDC_EPGOPTIONS_EPGFILENAME_LABEL,
									IDC_EPGOPTIONS_EPGFILENAME_BROWSE,
				::IsDlgButtonChecked(hDlg,IDC_EPGOPTIONS_SAVEEPGFILE)==BST_CHECKED);
			return TRUE;

		case IDC_EPGOPTIONS_EPGFILENAME_BROWSE:
			{
				OPENFILENAME ofn;
				TCHAR szFileName[MAX_PATH],szInitialDir[MAX_PATH];

				::GetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGFILENAME,
								 szFileName,lengthof(szFileName));
				ofn.lStructSize=sizeof(OPENFILENAME);
				ofn.hwndOwner=hDlg;
				ofn.lpstrFilter=TEXT("すべてのファイル\0*.*\0");
				ofn.lpstrCustomFilter=NULL;
				ofn.nFilterIndex=1;
				ofn.lpstrFile=szFileName;
				ofn.nMaxFile=lengthof(szFileName);
				ofn.lpstrFileTitle=NULL;
				if (szFileName[0]=='\0' || ::PathIsFileSpec(szFileName)) {
					::GetModuleFileName(NULL,szInitialDir,lengthof(szInitialDir));
					::PathRemoveFileSpec(szInitialDir);
					ofn.lpstrInitialDir=szInitialDir;
				} else
					ofn.lpstrInitialDir=NULL;
				ofn.lpstrTitle=TEXT("EPGファイル名");
				ofn.Flags=OFN_EXPLORER | OFN_HIDEREADONLY;
				ofn.lpstrDefExt=NULL;
#if _WIN32_WINNT>=0x500
				ofn.pvReserved=NULL;
				ofn.dwReserved=0;
				ofn.FlagsEx=0;
#endif
				if (::GetOpenFileName(&ofn))
					::SetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGFILENAME,szFileName);
			}
			return TRUE;

		case IDC_EPGOPTIONS_USEEPGDATA:
			EnableDlgItems(hDlg,IDC_EPGOPTIONS_EPGDATAFOLDER_LABEL,
								IDC_EPGOPTIONS_EPGDATAFOLDER_BROWSE,
						   DlgCheckBox_IsChecked(hDlg,IDC_EPGOPTIONS_USEEPGDATA));
			return TRUE;

		case IDC_EPGOPTIONS_EPGDATAFOLDER_BROWSE:
			{
				TCHAR szFolder[MAX_PATH];

				::GetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGDATAFOLDER,szFolder,lengthof(szFolder));
				if (BrowseFolderDialog(hDlg,szFolder,TEXT("EPGデータのフォルダ")))
					::SetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGDATAFOLDER,szFolder);
			}
			return TRUE;

		case IDC_LOGOOPTIONS_SAVEDATA:
			EnableDlgItems(hDlg,IDC_LOGOOPTIONS_DATAFILENAME_LABEL,
								IDC_LOGOOPTIONS_DATAFILENAME_BROWSE,
						   DlgCheckBox_IsChecked(hDlg,IDC_LOGOOPTIONS_SAVEDATA));
			return TRUE;

		case IDC_LOGOOPTIONS_DATAFILENAME_BROWSE:
			{
				OPENFILENAME ofn;
				TCHAR szFileName[MAX_PATH],szInitialDir[MAX_PATH];

				::GetDlgItemText(hDlg,IDC_LOGOOPTIONS_DATAFILENAME,szFileName,lengthof(szFileName));
				ofn.lStructSize=sizeof(OPENFILENAME);
				ofn.hwndOwner=hDlg;
				ofn.lpstrFilter=TEXT("すべてのファイル\0*.*\0");
				ofn.lpstrCustomFilter=NULL;
				ofn.nFilterIndex=1;
				ofn.lpstrFile=szFileName;
				ofn.nMaxFile=lengthof(szFileName);
				ofn.lpstrFileTitle=NULL;
				if (szFileName[0]=='\0' || ::PathIsFileSpec(szFileName)) {
					::GetModuleFileName(NULL,szInitialDir,lengthof(szInitialDir));
					::PathRemoveFileSpec(szInitialDir);
					ofn.lpstrInitialDir=szInitialDir;
				} else
					ofn.lpstrInitialDir=NULL;
				ofn.lpstrTitle=TEXT("ロゴファイル名");
				ofn.Flags=OFN_EXPLORER | OFN_HIDEREADONLY;
				ofn.lpstrDefExt=NULL;
#if _WIN32_WINNT>=0x500
				ofn.pvReserved=NULL;
				ofn.dwReserved=0;
				ofn.FlagsEx=0;
#endif
				if (::GetOpenFileName(&ofn))
					::SetDlgItemText(hDlg,IDC_LOGOOPTIONS_DATAFILENAME,szFileName);
			}
			return TRUE;

		case IDC_LOGOOPTIONS_LOGOFOLDER_BROWSE:
			{
				TCHAR szFolder[MAX_PATH];

				::GetDlgItemText(hDlg,IDC_LOGOOPTIONS_LOGOFOLDER,szFolder,lengthof(szFolder));
				if (BrowseFolderDialog(hDlg,szFolder,TEXT("ロゴのフォルダ")))
					::SetDlgItemText(hDlg,IDC_LOGOOPTIONS_LOGOFOLDER,szFolder);
			}
			return TRUE;

		case IDC_EVENTINFOOPTIONS_FONT_CHOOSE:
			{
				CEpgOptions *pThis=GetThis(hDlg);

				if (ChooseFontDialog(hDlg,&pThis->m_CurEventInfoFont))
					SetFontInfo(hDlg,&pThis->m_CurEventInfoFont);
			}
			return TRUE;
		}
		return TRUE;

	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case PSN_APPLY:
			{
				CEpgOptions *pThis=GetThis(hDlg);
				TCHAR szPath[MAX_PATH];

				::GetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGDATACAPDLLPATH,
								 szPath,lengthof(szPath));
				if (::lstrcmpi(szPath,pThis->m_szEpgDataCapDllPath)!=0) {
					pThis->m_pCoreEngine->m_DtvEngine.m_TsPacketParser.UnInitializeEpgDataCap();
					::lstrcpy(pThis->m_szEpgDataCapDllPath,szPath);
					pThis->InitializeEpgDataCap();
				}
				pThis->m_fSaveEpgFile=
					DlgCheckBox_IsChecked(hDlg,IDC_EPGOPTIONS_SAVEEPGFILE);
				::GetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGFILENAME,
					 pThis->m_szEpgFileName,lengthof(pThis->m_szEpgFileName));
				pThis->m_fUpdateWhenStandby=
					DlgCheckBox_IsChecked(hDlg,IDC_EPGOPTIONS_UPDATEWHENSTANDBY);
				bool fUseEpgData=
					DlgCheckBox_IsChecked(hDlg,IDC_EPGOPTIONS_USEEPGDATA);
				::GetDlgItemText(hDlg,IDC_EPGOPTIONS_EPGDATAFOLDER,
					pThis->m_szEpgDataFolder,lengthof(pThis->m_szEpgDataFolder));
				if (!pThis->m_fUseEpgData && fUseEpgData) {
					pThis->m_fUseEpgData=fUseEpgData;
					pThis->AsyncLoadEpgData();
				}
				pThis->m_fUseEpgData=fUseEpgData;

				pThis->m_fSaveLogoFile=DlgCheckBox_IsChecked(hDlg,IDC_LOGOOPTIONS_SAVEDATA);
				::GetDlgItemText(hDlg,IDC_LOGOOPTIONS_DATAFILENAME,
								 pThis->m_szLogoFileName,lengthof(pThis->m_szLogoFileName));
				pThis->m_pLogoManager->SetSaveLogo(DlgCheckBox_IsChecked(hDlg,IDC_LOGOOPTIONS_SAVERAWLOGO));
				pThis->m_pLogoManager->SetSaveLogoBmp(DlgCheckBox_IsChecked(hDlg,IDC_LOGOOPTIONS_SAVEBMPLOGO));
				::GetDlgItemText(hDlg,IDC_LOGOOPTIONS_LOGOFOLDER,szPath,lengthof(szPath));
				pThis->m_pLogoManager->SetLogoDirectory(szPath);

				if (!CompareLogFont(&pThis->m_EventInfoFont,&pThis->m_CurEventInfoFont)) {
					pThis->m_EventInfoFont=pThis->m_CurEventInfoFont;
					SetGeneralUpdateFlag(UPDATE_GENERAL_EVENTINFOFONT);
				}
			}
			break;
		}
		break;

	case WM_DESTROY:
		{
			CEpgOptions *pThis=GetThis(hDlg);

			pThis->OnDestroy();
		}
		return TRUE;
	}
	return FALSE;
}
