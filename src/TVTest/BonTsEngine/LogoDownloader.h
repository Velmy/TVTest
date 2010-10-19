#pragma once


#include <vector>
#include "MediaDecoder.h"
#include "TsTable.h"


/*
	���S�f�[�^�擾�N���X
*/
class CLogoDownloader : public CMediaDecoder
					  , public CPsiStreamTable::ISectionHandler
{
public:
	CLogoDownloader(IEventHandler *pEventHandler = NULL);
	virtual ~CLogoDownloader();

// IMediaDecoder
	virtual void Reset(void);
	virtual const bool InputMedia(CMediaData *pMediaData, const DWORD dwInputIndex = 0UL);

// CLogoDownloader
	struct LogoService {
		WORD OriginalNetworkID;
		WORD TransportStreamID;
		WORD ServiceID;
	};

	struct LogoData {
		WORD OriginalNetworkID;
		std::vector<LogoService> ServiceList;
		WORD LogoID;
		WORD LogoVersion;
		BYTE LogoType;
		WORD DataSize;
		const BYTE *pData;
	};

	class ABSTRACT_CLASS_DECL ILogoHandler
	{
	public:
		virtual ~ILogoHandler() {}
		virtual void OnLogo(const LogoData *pData) = 0;
	};

	void SetLogoHandler(ILogoHandler *pHandler);

private:
// CPsiStreamTable::ISectionHandler
	virtual void OnSection(CPsiStreamTable *pTable, const CPsiSection *pSection);

	static void CALLBACK OnLogoDataModule(const LogoData *pData, void *pParam);

	static void CALLBACK OnPatUpdated(const WORD wPID, CTsPidMapTarget *pMapTarget, CTsPidMapManager *pMapManager, const PVOID pParam);
	static void CALLBACK OnPmtUpdated(const WORD wPID, CTsPidMapTarget *pMapTarget, CTsPidMapManager *pMapManager, const PVOID pParam);
	static void CALLBACK OnNitUpdated(const WORD wPID, CTsPidMapTarget *pMapTarget, CTsPidMapManager *pMapManager, const PVOID pParam);

	int GetServiceIndexByID(const WORD ServiceID) const;
	bool MapDataEs(const int Index);
	bool UnmapDataEs(const int Index);

	CTsPidMapManager m_PidMapManager;

	ILogoHandler *m_pLogoHandler;

	struct ServiceInfo {
		WORD ServiceID;
		WORD PmtPID;
		BYTE ServiceType;
		std::vector<WORD> EsList;
	};
	std::vector<ServiceInfo> m_ServiceList;
};