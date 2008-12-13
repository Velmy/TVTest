#pragma once


#include "MediaData.h"
#include "BonSrcPin.h"


// このフィルタのGUID {DCA86296-964A-4e64-857D-8D140E630707}
DEFINE_GUID(CLSID_BONSOURCE, 0xdca86296, 0x964a, 0x4e64, 0x85, 0x7d, 0x8d, 0x14, 0x0e, 0x63, 0x07, 0x07);

class CBonSrcFilter : public CBaseFilter
{
friend CBonSrcPin;

public:
	DECLARE_IUNKNOWN

	CBonSrcFilter(LPUNKNOWN pUnk, HRESULT *phr);
	virtual ~CBonSrcFilter();
	static IBaseFilter * WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr,CBonSrcFilter **ppBonSrcFilterIf=NULL);

// CBaseFilter
#ifdef _DEBUG
	STDMETHODIMP Run(REFERENCE_TIME tStart);
	STDMETHODIMP Pause(void);
	STDMETHODIMP Stop(void);
#endif
	STDMETHODIMP GetState(DWORD dwMSecs, __out FILTER_STATE *State);
	int GetPinCount(void);
	CBasePin *GetPin(int n);

// CBonSrcFilter
	const bool InputMedia(CMediaData *pMediaData);

	void Reset();
	void Flush();
	void SetOutputWhenPaused(bool bOutput);
	//bool CheckHangUp(DWORD TimeOut);
protected:
	CBonSrcPin *m_pSrcPin;
	CCritSec m_cStateLock;
	bool m_bOutputWhenPaused;
};
