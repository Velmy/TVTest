#ifndef NETWORK_REMOCON_H
#define NETWORK_REMOCON_H


#include <winsock.h>
#include "CoreEngine.h"
#include "ChannelManager.h"
#include "Options.h"


class CNetworkRemoconReciver {
public:
	virtual void OnRecive(LPCSTR pszText) = 0;
};

class CNetworkRemocon {
	bool m_fInitialized;
	WSADATA m_WSAData;
	DWORD m_Address;
	WORD m_Port;
	HANDLE m_hThread;
	SOCKET m_Socket;
	bool m_fConnected;
	CChannelList m_ChannelList;
	static DWORD WINAPI SendProc(LPVOID pParam);
	bool Send(const char *pBuffer,int Length,
										CNetworkRemoconReciver *pReciver=NULL);
public:
	CNetworkRemocon();
	~CNetworkRemocon();
	bool Init(LPCSTR pszAddress,WORD Port);
	bool Shutdown();
	DWORD GetAddress() const { return m_Address; }
	WORD GetPort() const { return m_Port; }
	bool SetChannel(int ChannelNo);
	bool GetChannel(CNetworkRemoconReciver *pReciver);
	bool SetService(int Service);
	bool LoadChannelText(LPCTSTR pszFileName,const CChannelList *pChannelList);
	const CChannelList &GetChannelList() const { return m_ChannelList; }
	CChannelList &GetChannelList() { return m_ChannelList; }
};

class CNetworkRemoconOptions : public COptions {
	bool m_fUseNetworkRemocon;
	char m_szAddress[16];
	unsigned int m_Port;
	TCHAR m_szChannelFileName[MAX_PATH];
	bool m_fTempEnable;
	unsigned int m_TempPort;
	void GetChannelFilePath(LPTSTR pszPath) const;
	static CNetworkRemoconOptions *GetThis(HWND hDlg);
public:
	CNetworkRemoconOptions();
	~CNetworkRemoconOptions();
	bool Read(CSettings *pSettings);
	bool Write(CSettings *pSettings) const;
	unsigned int GetPort() const { return m_Port; }
	bool SetTempEnable(bool fEnable);
	bool SetTempPort(unsigned int Port);
	bool IsSettingValid() const;
	bool InitNetworkRemocon(CNetworkRemocon **ppNetworkRemocon,
		const CCoreEngine *pCoreEngine,CChannelManager *pChannelManager) const;
	static BOOL CALLBACK DlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
};


#endif
