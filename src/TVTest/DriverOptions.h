#ifndef DRIVER_OPTIONS_H
#define DRIVER_OPTIONS_H


#include "Options.h"
#include "DriverManager.h"
#include "PointerArray.h"
#include "ChannelManager.h"


class CDriverSettings;

class CDriverSettingList {
	CPointerVector<CDriverSettings> m_SettingList;
public:
	CDriverSettingList();
	~CDriverSettingList();
	CDriverSettingList &operator=(const CDriverSettingList &List);
	void Clear();
	int NumDrivers() const { return m_SettingList.Length(); }
	bool Add(CDriverSettings *pSettings);
	CDriverSettings *GetDriverSettings(int Index);
	const CDriverSettings *GetDriverSettings(int Index) const;
	int Find(LPCTSTR pszFileName) const;
};

class CDriverOptions : public COptions {
	CDriverManager *m_pDriverManager;
	CDriverSettingList m_SettingList;
	CDriverSettingList m_CurSettingList;
	void InitDlgItem(int Driver);
	static CDriverOptions *GetThis(HWND hDlg);
public:
	CDriverOptions();
	~CDriverOptions();
	// COptions
	bool Load(LPCTSTR pszFileName);
	bool Save(LPCTSTR pszFileName) const;
	// CDriverOptions
	bool Initialize(CDriverManager *pDriverManager);
	struct InitialChannelInfo {
		int Space,Channel,Service;
	};
	bool GetInitialChannel(LPCTSTR pszFileName,InitialChannelInfo *pChannelInfo) const;
	bool IsNoSignalLevel(LPCTSTR pszFileName) const;
	static BOOL CALLBACK DlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
};


#endif