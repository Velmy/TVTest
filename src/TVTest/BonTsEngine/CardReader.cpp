#include "stdafx.h"
#include "CardReader.h"
#include "StdUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CCardReader::CCardReader()
{
}


CCardReader::~CCardReader()
{
}


LPCTSTR CCardReader::EnumReader(int Index) const
{
	if (Index!=0)
		return NULL;
	return GetReaderName();
}


CCardReader *CCardReader::CreateCardReader(ReaderType Type)
{
	CCardReader *pReader;

	switch (Type) {
	case READER_SCARD:
		pReader=new CSCardReader;
		break;
	case READER_HDUS:
		pReader=new CHdusCardReader;
		break;
	default:
		return NULL;
	}
	pReader->m_ReaderType=Type;
	return pReader;
}




///////////////////////////////////////////////////////////////////////////////
//
// 標準スマートカードリーダー
//
///////////////////////////////////////////////////////////////////////////////


#pragma comment(lib, "WinScard.lib")


CSCardReader::CSCardReader()
{
	m_hBcasCard=NULL;
	m_bIsEstablish=false;
	m_pReaderList=NULL;
	m_NumReaders=0;
	m_pszReaderName=NULL;
	if (::SCardEstablishContext(SCARD_SCOPE_USER,NULL,NULL,&m_ScardContext)==SCARD_S_SUCCESS) {
		m_bIsEstablish=true;

		// カードリーダを列挙する
		DWORD dwBuffSize = 0UL;

		if (::SCardListReaders(m_ScardContext,NULL,NULL,&dwBuffSize)==SCARD_S_SUCCESS) {
			m_pReaderList=new TCHAR[dwBuffSize];
			if (::SCardListReaders(m_ScardContext,NULL,m_pReaderList,&dwBuffSize)==SCARD_S_SUCCESS) {
				LPCTSTR p=m_pReaderList;

				while (*p) {
					p+=lstrlen(p)+1;
					m_NumReaders++;
				}
			} else {
				delete [] m_pReaderList;
				m_pReaderList=NULL;
			}
		}
	}
}


CSCardReader::~CSCardReader()
{
	Close();
	if (m_bIsEstablish)
		::SCardReleaseContext(m_ScardContext);
	delete [] m_pReaderList;
	delete [] m_pszReaderName;
}


bool CSCardReader::Open(LPCTSTR pszReader)
{
	if (!m_bIsEstablish) {
		SetError(TEXT("コンテキストを確立できません。"));
		return false;
	}

	// 一旦クローズする
	Close();

	if (pszReader) {
		// 指定されたカードリーダに対してオープンを試みる
		LONG Result;
		DWORD dwActiveProtocol = SCARD_PROTOCOL_UNDEFINED;

		Result=::SCardConnect(m_ScardContext,pszReader,SCARD_SHARE_SHARED,SCARD_PROTOCOL_T1,&m_hBcasCard,&dwActiveProtocol);
		if (Result!=SCARD_S_SUCCESS) {
			SetError(TEXT("カードリーダに接続できません。"),NULL,GetErrorText(Result));
			return false;
		}

		if (dwActiveProtocol!=SCARD_PROTOCOL_T1) {
			Close();
			SetError(TEXT("アクティブプロトコルが不正です。"));
			return false;
		}
	} else {
		// 全てのカードリーダに対してオープンを試みる
		if (m_pReaderList==NULL) {
			SetError(TEXT("カードリーダが見付かりません。"));
			return false;
		}

		LPCTSTR p=m_pReaderList;

		while (*p) {
			if (Open(p))
				return true;
			p+=lstrlen(p)+1;
		}
		return false;
	}

	m_pszReaderName=StdUtil::strdup(pszReader);

	ClearError();

	return true;
}


void CSCardReader::Close()
{
	if (m_hBcasCard) {
		::SCardDisconnect(m_hBcasCard,SCARD_LEAVE_CARD);
		m_hBcasCard=NULL;
		delete [] m_pszReaderName;
		m_pszReaderName=NULL;
	}
}


LPCTSTR CSCardReader::GetReaderName() const
{
	return m_pszReaderName;
}


int CSCardReader::NumReaders() const
{
	return m_NumReaders;
}


LPCTSTR CSCardReader::EnumReader(int Index) const
{
	if (Index<0 || Index>=m_NumReaders)
		return NULL;
	LPCTSTR p=m_pReaderList;
	for (int i=0;i<Index;i++)
		p+=lstrlen(p)+1;
	return p;
}


bool CSCardReader::Transmit(const void *pSendData,DWORD SendSize,void *pRecvData,DWORD *pRecvSize)
{
	if (m_hBcasCard==NULL) {
		SetError(TEXT("カードリーダが開かれていません。"));
		return false;
	}

	LONG Result=::SCardTransmit(m_hBcasCard,SCARD_PCI_T1,(LPCBYTE)pSendData,SendSize,NULL,(LPBYTE)pRecvData,pRecvSize);

	if (Result!=SCARD_S_SUCCESS) {
		SetError(TEXT("コマンド送信エラーです。"),NULL,GetErrorText(Result));
		return false;
	}

	ClearError();

	return true;
}


LPCTSTR CSCardReader::GetErrorText(LONG Code)
{
	switch (Code) {
	case ERROR_BROKEN_PIPE:
		return TEXT("ERROR_BROKEN_PIPE: The client attempted a smart card operation in a remote session.");
	case SCARD_F_INTERNAL_ERROR:
		return TEXT("SCARD_F_INTERNAL_ERROR: An internal consistency check failed.");
	case SCARD_E_CANCELLED:
		return TEXT("SCARD_E_CANCELLED: The action was cancelled by an SCardCancel request.");
	case SCARD_E_INVALID_HANDLE:
		return TEXT("SCARD_E_INVALID_HANDLE: The supplied handle was invalid.");
	case SCARD_E_INVALID_PARAMETER:
		return TEXT("SCARD_E_INVALID_PARAMETER: One or more of the supplied parameters could not be properly interpreted.");
	case SCARD_E_INVALID_TARGET:
		return TEXT("SCARD_E_INVALID_TARGET: Registry startup information is missing or invalid.");
	case SCARD_E_NO_MEMORY:
		return TEXT("SCARD_E_NO_MEMORY: Not enough memory available to complete this command.");
	case SCARD_F_WAITED_TOO_LONG:
		return TEXT("SCARD_F_WAITED_TOO_LONG: An internal consistency timer has expired.");
	case SCARD_E_INSUFFICIENT_BUFFER:
		return TEXT("SCARD_E_INSUFFICIENT_BUFFER: The data buffer to receive returned data is too small for the returned data.");
	case SCARD_E_UNKNOWN_READER:
		return TEXT("SCARD_E_UNKNOWN_READER: The specified reader name is not recognized.");
	case SCARD_E_TIMEOUT:
		return TEXT("SCARD_E_TIMEOUT: The user-specified timeout value has expired.");
	case SCARD_E_SHARING_VIOLATION:
		return TEXT("SCARD_E_SHARING_VIOLATION: The smart card cannot be accessed because of other connections outstanding.");
	case SCARD_E_NO_SMARTCARD:
		return TEXT("SCARD_E_NO_SMARTCARD: The operation requires a Smart Card, but no Smart Card is currently in the device.");
	case SCARD_E_UNKNOWN_CARD:
		return TEXT("SCARD_E_UNKNOWN_CARD: The specified smart card name is not recognized.");
	case SCARD_E_CANT_DISPOSE:
		return TEXT("SCARD_E_CANT_DISPOSE: The system could not dispose of the media in the requested manner.");
	case SCARD_E_PROTO_MISMATCH:
		return TEXT("SCARD_E_PROTO_MISMATCH: The requested protocols are incompatible with the protocol currently in use with the smart card.");
	case SCARD_E_NOT_READY:
		return TEXT("SCARD_E_NOT_READY: The reader or smart card is not ready to accept commands.");
	case SCARD_E_INVALID_VALUE:
		return TEXT("SCARD_E_INVALID_VALUE: One or more of the supplied parameters values could not be properly interpreted.");
	case SCARD_E_SYSTEM_CANCELLED:
		return TEXT("SCARD_E_SYSTEM_CANCELLED: The action was cancelled by the system, presumably to log off or shut down.");
	case SCARD_F_COMM_ERROR:
		return TEXT("SCARD_F_COMM_ERROR: An internal communications error has been detected.");
	case SCARD_F_UNKNOWN_ERROR:
		return TEXT("SCARD_F_UNKNOWN_ERROR: An internal error has been detected, but the source is unknown.");
	case SCARD_E_INVALID_ATR:
		return TEXT("SCARD_E_INVALID_ATR: An ATR obtained from the registry is not a valid ATR string.");
	case SCARD_E_NOT_TRANSACTED:
		return TEXT("SCARD_E_NOT_TRANSACTED: An attempt was made to end a non-existent transaction.");
	case SCARD_E_READER_UNAVAILABLE:
		return TEXT("SCARD_E_READER_UNAVAILABLE: The specified reader is not currently available for use.");
	case SCARD_P_SHUTDOWN:
		return TEXT("SCARD_P_SHUTDOWN: The operation has been aborted to allow the server application to exit.");
	case SCARD_E_PCI_TOO_SMALL:
		return TEXT("SCARD_E_PCI_TOO_SMALL: The PCI Receive buffer was too small.");
	case SCARD_E_READER_UNSUPPORTED:
		return TEXT("SCARD_E_READER_UNSUPPORTED: The reader driver does not meet minimal requirements for support.");
	case SCARD_E_DUPLICATE_READER:
		return TEXT("SCARD_E_DUPLICATE_READER: The reader driver did not produce a unique reader name.");
	case SCARD_E_CARD_UNSUPPORTED:
		return TEXT("SCARD_E_CARD_UNSUPPORTED: The smart card does not meet minimal requirements for support.");
	case SCARD_E_NO_SERVICE:
		return TEXT("SCARD_E_NO_SERVICE: The Smart card resource manager is not running.");
	case SCARD_E_SERVICE_STOPPED:
		return TEXT("SCARD_E_SERVICE_STOPPED: The Smart card resource manager has shut down.");
	case SCARD_E_UNEXPECTED:
		return TEXT("SCARD_E_UNEXPECTED: An unexpected card error has occurred.");
	case SCARD_E_ICC_INSTALLATION:
		return TEXT("SCARD_E_ICC_INSTALLATION: No Primary Provider can be found for the smart card.");
	case SCARD_E_ICC_CREATEORDER:
		return TEXT("SCARD_E_ICC_CREATEORDER: The requested order of object creation is not supported.");
	case SCARD_E_UNSUPPORTED_FEATURE:
		return TEXT("SCARD_E_UNSUPPORTED_FEATURE: This smart card does not support the requested feature.");
	case SCARD_E_DIR_NOT_FOUND:
		return TEXT("SCARD_E_DIR_NOT_FOUND: The identified directory does not exist in the smart card.");
	case SCARD_E_FILE_NOT_FOUND:
		return TEXT("SCARD_E_FILE_NOT_FOUND: The identified file does not exist in the smart card.");
	case SCARD_E_NO_DIR:
		return TEXT("SCARD_E_NO_DIR: The supplied path does not represent a smart card directory.");
	case SCARD_E_NO_FILE:
		return TEXT("SCARD_E_NO_FILE: The supplied path does not represent a smart card file.");
	case SCARD_E_NO_ACCESS:
		return TEXT("SCARD_E_NO_ACCESS: Access is denied to this file.");
	case SCARD_E_WRITE_TOO_MANY:
		return TEXT("SCARD_E_WRITE_TOO_MANY: The smartcard does not have enough memory to store the information.");
	case SCARD_E_BAD_SEEK:
		return TEXT("SCARD_E_BAD_SEEK: There was an error trying to set the smart card file object pointer.");
	case SCARD_E_INVALID_CHV:
		return TEXT("SCARD_E_INVALID_CHV: The supplied PIN is incorrect.");
	case SCARD_E_UNKNOWN_RES_MNG:
		return TEXT("SCARD_E_UNKNOWN_RES_MNG: An unrecognized error code was returned from a layered component.");
	case SCARD_E_NO_SUCH_CERTIFICATE:
		return TEXT("SCARD_E_NO_SUCH_CERTIFICATE: The requested certificate does not exist.");
	case SCARD_E_CERTIFICATE_UNAVAILABLE:
		return TEXT("SCARD_E_CERTIFICATE_UNAVAILABLE: The requested certificate could not be obtained.");
	case SCARD_E_NO_READERS_AVAILABLE:
		return TEXT("SCARD_E_NO_READERS_AVAILABLE: Cannot find a smart card reader.");
	case SCARD_E_COMM_DATA_LOST:
		return TEXT("SCARD_E_COMM_DATA_LOST: A communications error with the smart card has been detected.  Retry the operation.");
	case SCARD_E_NO_KEY_CONTAINER:
		return TEXT("SCARD_E_NO_KEY_CONTAINER: The requested key container does not exist on the smart card.");
	case SCARD_E_SERVER_TOO_BUSY:
		return TEXT("SCARD_E_SERVER_TOO_BUSY: The Smart card resource manager is too busy to complete this operation.");
	case SCARD_W_UNSUPPORTED_CARD:
		return TEXT("SCARD_W_UNSUPPORTED_CARD: The reader cannot communicate with the smart card, due to ATR configuration conflicts.");
	case SCARD_W_UNRESPONSIVE_CARD:
		return TEXT("SCARD_W_UNRESPONSIVE_CARD: The smart card is not responding to a reset.");
	case SCARD_W_UNPOWERED_CARD:
		return TEXT("SCARD_W_UNPOWERED_CARD: Power has been removed from the smart card, so that further communication is not possible.");
	case SCARD_W_RESET_CARD:
		return TEXT("SCARD_W_RESET_CARD: The smart card has been reset, so any shared state information is invalid.");
	case SCARD_W_REMOVED_CARD:
		return TEXT("SCARD_W_REMOVED_CARD: The smart card has been removed, so that further communication is not possible.");
	case SCARD_W_SECURITY_VIOLATION:
		return TEXT("SCARD_W_SECURITY_VIOLATION: Access was denied because of a security violation.");
	case SCARD_W_WRONG_CHV:
		return TEXT("SCARD_W_WRONG_CHV: The card cannot be accessed because the wrong PIN was presented.");
	case SCARD_W_CHV_BLOCKED:
		return TEXT("SCARD_W_CHV_BLOCKED: The card cannot be accessed because the maximum number of PIN entry attempts has been reached.");
	case SCARD_W_EOF:
		return TEXT("SCARD_W_EOF: The end of the smart card file has been reached.");
	case SCARD_W_CANCELLED_BY_USER:
		return TEXT("SCARD_W_CANCELLED_BY_USER: The action was cancelled by the user.");
	case SCARD_W_CARD_NOT_AUTHENTICATED:
		return TEXT("SCARD_W_CARD_NOT_AUTHENTICATED: No PIN was presented to the smart card.");
	case SCARD_W_CACHE_ITEM_NOT_FOUND:
		return TEXT("SCARD_W_CACHE_ITEM_NOT_FOUND: The requested item could not be found in the cache.");
	case SCARD_W_CACHE_ITEM_STALE:
		return TEXT("SCARD_W_CACHE_ITEM_STALE: The requested cache item is too old and was deleted from the cache.");
	case SCARD_W_CACHE_ITEM_TOO_BIG:
		return TEXT("SCARD_W_CACHE_ITEM_TOO_BIG: The new cache item exceeds the maximum per-item size defined for the cache.");
	}
	return NULL;
}




///////////////////////////////////////////////////////////////////////////////
//
// HDUS内蔵カードリーダー
//
///////////////////////////////////////////////////////////////////////////////


static const GUID CLSID_KSCATEGORY_BDA_NETWORK_TUNER
 = {0x71985F48,0x1CA1,0x11D3,{0x9C,0xC8,0x00,0xC0,0x4F,0x79,0x71,0xE0}};
static const GUID CLSID_PropSet
 = {0x9E1781E1,0x9CB1,0x4407,{0xBB,0xCE,0x54,0x26,0xC8,0xD0,0x0A,0x4B}};


CHdusCardReader::CHdusCardReader()
{
	m_pTuner=NULL;
	m_bSent=false;
	::CoInitialize(NULL);
}


CHdusCardReader::~CHdusCardReader()
{
	Close();
	::CoUninitialize();
}


bool CHdusCardReader::Open(LPCTSTR pszReader)
{
	Close();
	m_pTuner = FindDevice(CLSID_KSCATEGORY_BDA_NETWORK_TUNER,L"SKNET HDTV BDA Digital Tuner_0");
	if (m_pTuner == NULL) {
		SetError(TEXT("デバイスが見付かりません。"));
		return false;
	}
	ClearError();
	return true;
}


void CHdusCardReader::Close()
{
	if (m_pTuner) {
		m_pTuner->Release();
		m_pTuner=NULL;
		m_bSent=false;
	}
}


LPCTSTR CHdusCardReader::GetReaderName() const
{
	return TEXT("HDUS Card Reader");
}


bool CHdusCardReader::Transmit(const void *pSendData,DWORD SendSize,void *pRecvData,DWORD *pRecvSize)
{
	if (m_pTuner==NULL) {
		SetError(TEXT("カードリーダが開かれていません。"));
		return false;
	}

#ifdef USE_MUTEX
	HANDLE hMutex=::CreateMutex(NULL,TRUE,L"hduscard");

	if (hMutex==NULL) {
		return SCARD_F_INTERNAL_ERROR;
	} else if (::GetLastError()==ERROR_ALREADY_EXISTS) {
		::WaitForSingleObject(hMutex,INFINITE);
	}
#endif

	bool bOK=false;
	HRESULT hr;
	TCHAR szText[64];

	hr=Send(pSendData,SendSize);
	if (FAILED(hr)) {
		::wsprintf(szText,TEXT("コマンド送信エラー(エラーコード %08x)"),hr);
		SetError(szText);
	} else {
		::Sleep(50);
		hr=Receive(pRecvData,pRecvSize);
		if (SUCCEEDED(hr)) {
			ClearError();
			bOK=true;
		} else {
			::wsprintf(szText,TEXT("受信エラー(エラーコード %08x)"),hr);
			SetError(szText);
		}
	}

#ifdef USE_MUTEX
	::ReleaseMutex(hMutex);
	::CloseHandle(hMutex);
#endif
	return bOK;
}


IBaseFilter *CHdusCardReader::FindDevice(REFCLSID category,BSTR varFriendlyName)
{
	HRESULT hr;
	ICreateDevEnum *pSysDevEnum;
	IEnumMoniker *pEnumCat;
	IMoniker *pMoniker;
	IPropertyBag *pPropBag;
	IBaseFilter *pFilter=NULL;
	ULONG cFetched;
	VARIANT varName;

	hr=::CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,
							IID_ICreateDevEnum,(void**)&pSysDevEnum);
	if (FAILED(hr))
		return NULL;
	hr=pSysDevEnum->CreateClassEnumerator(category,&pEnumCat,0);
	if (hr!=S_OK) {
		pSysDevEnum->Release();
		return NULL;
	}
	::VariantInit(&varName);
	while (pEnumCat->Next(1,&pMoniker,&cFetched)==S_OK) {
		hr=pMoniker->BindToStorage(0,0,IID_IPropertyBag,(void **)&pPropBag);
		if (FAILED(hr)) {
			pMoniker->Release();
			break;
		}
		hr=pPropBag->Read(L"FriendlyName",&varName,NULL);
		pPropBag->Release();
		if (FAILED(hr) || ::lstrcmpW(varName.bstrVal,varFriendlyName)!=0) {
			pMoniker->Release();
			continue;
		}
		hr=pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pFilter);
		pMoniker->Release();
		if (SUCCEEDED(hr))
			break;
	}
	::VariantClear(&varName);
	pEnumCat->Release();
	pSysDevEnum->Release();
	return pFilter;
}


static inline DWORD DiffTime(DWORD Start,DWORD End)
{
	if (Start<=End)
		return End-Start;
	return 0xFFFFFFFFUL-Start+1+End;
}


HRESULT CHdusCardReader::Send(const void *pSendData,DWORD SendSize)
{
	IKsPropertySet *pKsPropertySet;
	HRESULT hr;
	BYTE Buffer[0x42];
	DWORD Time;

	if (SendSize>0x3C)
		return E_FAIL;
	hr=m_pTuner->QueryInterface(IID_IKsPropertySet,(void**)&pKsPropertySet);
	if (FAILED(hr))
		return hr;
	::ZeroMemory(Buffer,sizeof(Buffer));
	Buffer[1]=(BYTE)SendSize;
	::CopyMemory(Buffer+2,pSendData,SendSize);
	Time=::GetTickCount();
	do {
		hr=pKsPropertySet->Set(CLSID_PropSet,0x10,NULL,0,Buffer,0x42);
		if (m_bSent)
			break;
		if (hr==0x8007001F)
			::Sleep(50);
	} while (hr==0x8007001F && DiffTime(Time,::GetTickCount())<2000);
	/*
	if (hr!=S_OK) {
		TCHAR szErr[128];
		::wsprintf(szErr,TEXT("CHdusCardReader::Send Error %08X\n"),hr);
		::OutputDebugString(szErr);
	}
	*/
	pKsPropertySet->Release();
	m_bSent=true;
	return hr;
}


HRESULT CHdusCardReader::Receive(void *pRecvData,DWORD *pRecvSize)
{
	IKsPropertySet *pKsPropertySet;
	HRESULT hr;
	BYTE Buffer[0x42];
	DWORD Time,Returned;

	if (*pRecvSize<0x3C)
		return E_FAIL;
	hr=m_pTuner->QueryInterface(IID_IKsPropertySet,(void**)&pKsPropertySet);
	if (FAILED(hr))
		return hr;
	Time=::GetTickCount();
	Buffer[0]=0;
	do {
		hr=pKsPropertySet->Get(CLSID_PropSet,0x11,NULL,0,Buffer,0x42,&Returned);
		if (hr!=S_OK) {
			/*
			TCHAR szErr[128];
			::wsprintf(szErr,TEXT("CHdusCardReader::Recieve Error %08X\n"),hr);
			::OutputDebugString(szErr);
			*/
			pKsPropertySet->Release();
			return hr;
		}
		if (Buffer[0]!=1)
			::Sleep(50);
	} while (Buffer[0]!=1 && DiffTime(Time,::GetTickCount())<2000);
	pKsPropertySet->Release();
	if (Buffer[0]!=1)
		return E_FAIL;
	if (*pRecvSize<Buffer[1])
		return E_FAIL;
	::CopyMemory(pRecvData,Buffer+2,Buffer[1]);
	*pRecvSize=Buffer[1];
	return S_OK;
}
