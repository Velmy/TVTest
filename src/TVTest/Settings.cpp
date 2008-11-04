#include "stdafx.h"
#include "Settings.h"




static int StrToInt(LPCTSTR pszValue)
{
	int nValue;
	LPCTSTR p;

	nValue=0;
	p=pszValue;
	if (*p=='-' || *p=='+')
		p++;
	while (*p>='0' && *p<='9') {
		nValue=nValue*10+(*p-'0');
		p++;
	}
	return pszValue[0]=='-'?-nValue:nValue;
}


static unsigned int StrToUInt(LPCTSTR pszValue)
{
	unsigned int uValue;
	LPCTSTR p;

	uValue=0;
	p=pszValue;
	while (*p>='0' && *p<='9') {
		uValue=uValue*10+(*p-'0');
		p++;
	}
	return uValue;
}


static int HexToNum(TCHAR cCode)
{
	if (cCode>='0' && cCode<='9')
		return cCode-'0';
	if (cCode>='A' && cCode<='F')
		return cCode-'A'+10;
	if (cCode>='a' && cCode<='f')
		return cCode-'a'+10;
	return 0;
}


CSettings::CSettings()
{
	m_OpenFlags=0;
}


CSettings::~CSettings()
{
	Close();
}


bool CSettings::Open(LPCTSTR pszFileName,LPCTSTR pszSection,unsigned int Flags)
{
	lstrcpy(m_szFileName,pszFileName);
	lstrcpy(m_szSection,pszSection);
	m_OpenFlags=Flags;
	return true;
}


void CSettings::Close()
{
	if ((m_OpenFlags&OPEN_WRITE)!=0)
		WritePrivateProfileString(NULL,NULL,NULL,m_szFileName);	// Flush
	m_OpenFlags=0;
}


bool CSettings::Clear()
{
	return WritePrivateProfileSection(m_szSection,TEXT("\0"),m_szFileName);
}


bool CSettings::Read(LPCTSTR pszValueName,int *pData)
{
	TCHAR szValue[16];

	if (!Read(pszValueName,szValue,16) || szValue[0]=='\0')
		return false;
	*pData=StrToInt(szValue);
	return true;
}


bool CSettings::Write(LPCTSTR pszValueName,int Data)
{
	TCHAR szValue[16];

	wsprintf(szValue,TEXT("%d"),Data);
	return Write(pszValueName,szValue);
}


bool CSettings::Read(LPCTSTR pszValueName,unsigned int *pData)
{
	TCHAR szValue[16];

	if (!Read(pszValueName,szValue,16) || szValue[0]=='\0')
		return false;
	*pData=StrToUInt(szValue);
	return true;
}


bool CSettings::Write(LPCTSTR pszValueName,unsigned int Data)
{
	TCHAR szValue[16];

	wsprintf(szValue,TEXT("%u"),Data);
	return Write(pszValueName,szValue);
}


bool CSettings::Read(LPCTSTR pszValueName,LPTSTR pszData,unsigned int Max)
{
	TCHAR cBack[2];

	cBack[0]=pszData[0];
	if (Max>1)
		cBack[1]=pszData[1];
	GetPrivateProfileString(m_szSection,pszValueName,TEXT("\x1A"),pszData,Max,
																m_szFileName);
	if (pszData[0]=='\x1A') {
		pszData[0]=cBack[0];
		if (Max>1)
			pszData[1]=cBack[1];
		return false;
	}
	return true;
}


bool CSettings::Write(LPCTSTR pszValueName,LPCTSTR pszData)
{
	return WritePrivateProfileString(m_szSection,pszValueName,pszData,
																m_szFileName);
}


bool CSettings::Read(LPCTSTR pszValueName,bool *pfData)
{
	TCHAR szData[5];

	if (!Read(pszValueName,szData,5))
		return false;
	if (lstrcmpi(szData,TEXT("yes"))==0)
		*pfData=true;
	else if (lstrcmpi(szData,TEXT("no"))==0)
		*pfData=false;
	else
		return false;
	return true;
}


bool CSettings::Write(LPCTSTR pszValueName,bool fData)
{
	return Write(pszValueName,fData?TEXT("yes"):TEXT("no"));
}


bool CSettings::ReadColor(LPCTSTR pszValueName,COLORREF *pcrData)
{
	TCHAR szText[8];

	if (!Read(pszValueName,szText,8) || szText[0]!='#' || lstrlen(szText)!=7)
		return false;
	*pcrData=RGB((HexToNum(szText[1])<<4) | HexToNum(szText[2]),
				 (HexToNum(szText[3])<<4) | HexToNum(szText[4]),
				 (HexToNum(szText[5])<<4) | HexToNum(szText[6]));
	return true;
}


bool CSettings::WriteColor(LPCTSTR pszValueName,COLORREF crData)
{
	TCHAR szText[8];

	wsprintf(szText,TEXT("#%02x%02x%02x"),
						GetRValue(crData),GetGValue(crData),GetBValue(crData));
	return Write(pszValueName,szText);
}