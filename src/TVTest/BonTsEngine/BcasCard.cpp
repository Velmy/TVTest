// BcasCard.cpp: CBcasCard クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BcasCard.h"
#include "StdUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CBcasCard::CBcasCard()
	: m_pCardReader(NULL)
{
	// 内部状態初期化
	::ZeroMemory(&m_BcasCardInfo, sizeof(m_BcasCardInfo));
	::ZeroMemory(&m_EcmStatus, sizeof(m_EcmStatus));
}


CBcasCard::~CBcasCard()
{
	CloseCard();
}


const DWORD CBcasCard::GetCardReaderNum(void) const
{
	// カードリーダー数を返す
	if (m_pCardReader)
		return m_pCardReader->NumReaders();
	return 0;
}


LPCTSTR CBcasCard::EnumCardReader(const DWORD dwIndex) const
{
	if (m_pCardReader)
		return m_pCardReader->EnumReader(dwIndex);
	return NULL;
}


const bool CBcasCard::OpenCard(CCardReader::ReaderType ReaderType, LPCTSTR lpszReader)
{
	// 一旦クローズする
	CloseCard();

	m_pCardReader = CCardReader::CreateCardReader(ReaderType);
	if (m_pCardReader == NULL) {
		SetError(ERR_CARDOPENERROR, TEXT("カードリーダのタイプが無効です。"));
		return false;
	}
	if (!m_pCardReader->Open(lpszReader)) {
		SetError(m_pCardReader->GetLastErrorException());
		SetErrorCode(ERR_CARDOPENERROR);
		delete m_pCardReader;
		m_pCardReader=NULL;
		return false;
	}

	// カード初期化
	if (!InitialSetting()) {
		CloseCard();
		return false;
	}

	ClearError();

	return true;
}


void CBcasCard::CloseCard(void)
{
	// カードをクローズする
	if (m_pCardReader) {
		m_pCardReader->Close();
		delete m_pCardReader;
		m_pCardReader=NULL;
	}
}


const bool CBcasCard::ReOpenCard()
{
	if (m_pCardReader==NULL) {
		SetError(ERR_CARDNOTOPEN,NULL);
		return false;
	}
	return OpenCard(m_pCardReader->GetReaderType(),m_pCardReader->GetReaderName());
}


const bool CBcasCard::IsCardOpen() const
{
	return m_pCardReader!=NULL;
}


LPCTSTR CBcasCard::GetCardReaderName() const
{
	if (m_pCardReader)
		return m_pCardReader->GetReaderName();
	return NULL;
}


const bool CBcasCard::InitialSetting(void)
{
	// 「Initial Setting Conditions Command」を処理する
	/*
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, NULL);
		return false;
	}
	*/

	// バッファ準備
	DWORD dwRecvSize;
	BYTE RecvData[1024];

	// コマンド送信
	static const BYTE InitSettingCmd[] = {0x90U, 0x30U, 0x00U, 0x00U, 0x00U};
	::ZeroMemory(RecvData, sizeof(RecvData));
	dwRecvSize=sizeof(RecvData);
	if (!m_pCardReader->Transmit(InitSettingCmd, sizeof(InitSettingCmd), RecvData, &dwRecvSize)) {
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return false;
	}

	if (dwRecvSize < 57UL) {
		SetError(ERR_TRANSMITERROR, TEXT("受信データのサイズが不正です。"));
		return false;
	}

	// レスポンス解析
	m_BcasCardInfo.CASystemID = ((WORD)RecvData[6] << 8) | (WORD)RecvData[7];
	::CopyMemory(m_BcasCardInfo.BcasCardID, &RecvData[8], 6UL);		// +8	Card ID
	m_BcasCardInfo.CardType = RecvData[14];
	m_BcasCardInfo.MessagePartitionLength = RecvData[15];
	::CopyMemory(m_BcasCardInfo.SystemKey, &RecvData[16], 32UL);	// +16	Descrambling system key
	::CopyMemory(m_BcasCardInfo.InitialCbc, &RecvData[48], 8UL);	// +48	Descrambler CBC initial value

	const static BYTE CardIDInfoCmd[] = {0x90, 0x32, 0x00, 0x00, 0x00};
	::ZeroMemory(RecvData, sizeof(RecvData));
	dwRecvSize=sizeof(RecvData);
	if (m_pCardReader->Transmit(CardIDInfoCmd, sizeof(CardIDInfoCmd), RecvData, &dwRecvSize)
			&& dwRecvSize >= 17) {
		m_BcasCardInfo.CardManufacturerID=RecvData[7];
		m_BcasCardInfo.CardVersion=RecvData[8];
		m_BcasCardInfo.CheckCode=(RecvData[15]<<8)|RecvData[16];
	}

	// ECMステータス初期化
	::ZeroMemory(&m_EcmStatus, sizeof(m_EcmStatus));

	return true;
}


const bool CBcasCard::GetCASystemID(WORD *pID)
{
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, NULL);
		return false;
	}

	if (pID == NULL) {
		SetError(ERR_BADARGUMENT, NULL);
		return false;
	}

	*pID = m_BcasCardInfo.CASystemID;

	ClearError();

	return true;
}


const BYTE * CBcasCard::GetBcasCardID(void)
{
	// Card ID を返す
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, NULL);
		return NULL;
	}

	ClearError();

	return m_BcasCardInfo.BcasCardID;
}


const BYTE CBcasCard::GetCardType(void)
{
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, NULL);
		return CARDTYPE_INVALID;
	}

	return m_BcasCardInfo.CardType;
}


const BYTE CBcasCard::GetMessagePartitionLength(void)
{
	return m_BcasCardInfo.MessagePartitionLength;
}


const BYTE * CBcasCard::GetInitialCbc(void)
{
	// Descrambler CBC Initial Value を返す
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, NULL);
		return NULL;
	}

	ClearError();

	return m_BcasCardInfo.InitialCbc;
}


const BYTE * CBcasCard::GetSystemKey(void)
{
	// Descrambling System Key を返す
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, NULL);
		return NULL;
	}

	ClearError();

	return m_BcasCardInfo.SystemKey;
}


#pragma intrinsic(memcpy, memset, memcmp)

const BYTE * CBcasCard::GetKsFromEcm(const BYTE *pEcmData, const DWORD dwEcmSize)
{
	// 「ECM Receive Command」を処理する
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, TEXT(""));
		return NULL;
	}

	// ECMサイズをチェック
	if (!pEcmData || (dwEcmSize < 30UL) || (dwEcmSize > 256UL)) {
		SetError(ERR_BADARGUMENT, TEXT(""));
		return NULL;
	}

	// キャッシュをチェックする
	if (m_EcmStatus.dwLastEcmSize == dwEcmSize
			&& ::memcmp(m_EcmStatus.LastEcmData, pEcmData, dwEcmSize) == 0) {
		// ECMが同一の場合はキャッシュ済みKsを返す
		ClearError();
		return m_EcmStatus.KsData;
	}
	// ECMデータを保存する
	m_EcmStatus.dwLastEcmSize = dwEcmSize;
	::CopyMemory(m_EcmStatus.LastEcmData, pEcmData, dwEcmSize);

	// バッファ準備
	static const BYTE EcmReceiveCmd[] = {0x90, 0x34, 0x00, 0x00};
	DWORD dwRecvSize = 0UL;
	BYTE SendData[272];
	BYTE RecvData[1024];
	::ZeroMemory(RecvData, sizeof(RecvData));

	// コマンド構築
	::CopyMemory(SendData, EcmReceiveCmd, sizeof(EcmReceiveCmd));				// CLA, INS, P1, P2
	SendData[sizeof(EcmReceiveCmd)] = (BYTE)dwEcmSize;							// COMMAND DATA LENGTH
	::CopyMemory(&SendData[sizeof(EcmReceiveCmd) + 1], pEcmData, dwEcmSize);	// ECM
	SendData[sizeof(EcmReceiveCmd) + dwEcmSize + 1] = 0x00U;					// RESPONSE DATA LENGTH

	// コマンド送信
	dwRecvSize=sizeof(RecvData);
	if (!m_pCardReader->Transmit(SendData, sizeof(EcmReceiveCmd) + dwEcmSize + 2UL, RecvData, &dwRecvSize)){
		::ZeroMemory(&m_EcmStatus, sizeof(m_EcmStatus));
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return NULL;
	}

	// サイズチェック
	if (dwRecvSize != 25UL) {
		::ZeroMemory(&m_EcmStatus, sizeof(m_EcmStatus));
		SetError(ERR_TRANSMITERROR, TEXT("ECMのレスポンスサイズが不正です。"));
		return NULL;
	}

	// レスポンス解析
	::CopyMemory(m_EcmStatus.KsData, &RecvData[6], sizeof(m_EcmStatus.KsData));

	// リターンコード解析
	switch (((WORD)RecvData[4] << 8) | (WORD)RecvData[5]) {
	// Purchased: Viewing
	case 0x0200U :	// Payment-deferred PPV
	case 0x0400U :	// Prepaid PPV
	case 0x0800U :	// Tier

	case 0x4480U :	// Payment-deferred PPV
	case 0x4280U :	// Prepaid PPV
		ClearError();
		return m_EcmStatus.KsData;
	}
	// 上記以外(視聴不可)

	SetError(ERR_ECMREFUSED, TEXT("ECMが受け付けられません。"));

	return NULL;
}


const bool CBcasCard::SendEmmSection(const BYTE *pEmmData, const DWORD dwEmmSize)
{
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, TEXT(""));
		return false;
	}

	if (pEmmData == NULL || dwEmmSize < 17UL || dwEmmSize > 263UL) {
		SetError(ERR_BADARGUMENT, TEXT(""));
		return false;
	}

	static const BYTE EmmReceiveCmd[] = {0x90, 0x36, 0x00, 0x00};
	BYTE SendData[272], RecvData[1024];

	::CopyMemory(SendData, EmmReceiveCmd, sizeof(EmmReceiveCmd));
	SendData[sizeof(EmmReceiveCmd)] = (BYTE)dwEmmSize;
	::CopyMemory(&SendData[sizeof(EmmReceiveCmd) + 1], pEmmData, dwEmmSize);
	SendData[sizeof(EmmReceiveCmd) + dwEmmSize + 1] = 0x00;

	::ZeroMemory(RecvData, sizeof(RecvData));
	DWORD RecvSize = sizeof(RecvData);
	if (!m_pCardReader->Transmit(SendData, sizeof(EmmReceiveCmd) + dwEmmSize + 2UL, RecvData, &RecvSize)) {
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return false;
	}

	if (RecvSize != 8UL) {
		SetError(ERR_TRANSMITERROR, TEXT("EMMのレスポンスサイズが不正です。"));
		return false;
	}

	switch (((WORD)RecvData[4] << 8) | (WORD)RecvData[5]) {
	case 0x2100U :	// 正常終了
		ClearError();
		return true;

	//case 0xA102U :	// 非運用(運用外プロトコル番号)
	//case 0xA107U :	// セキュリティエラー(EMM改ざんエラー)
	}

	SetError(ERR_EMMERROR, TEXT("EMMが受け付けられません。"));

	return false;
}


const int CBcasCard::FormatCardID(LPTSTR pszText, int MaxLength) const
{
	if (pszText == NULL || MaxLength <= 0)
		return 0;

	ULONGLONG ID;

	ID = ((((ULONGLONG)(m_BcasCardInfo.BcasCardID[0] & 0x1F) << 40) |
		 ((ULONGLONG)m_BcasCardInfo.BcasCardID[1] << 32) |
		 ((ULONGLONG)m_BcasCardInfo.BcasCardID[2] << 24) |
		 ((ULONGLONG)m_BcasCardInfo.BcasCardID[3] << 16) |
		 ((ULONGLONG)m_BcasCardInfo.BcasCardID[4] << 8) |
		 (ULONGLONG)m_BcasCardInfo.BcasCardID[5]) * 100000ULL) +
		 m_BcasCardInfo.CheckCode;
	return StdUtil::snprintf(pszText, MaxLength,
			TEXT("%d%03lu %04lu %04lu %04lu %04lu"),
			m_BcasCardInfo.BcasCardID[0] >> 5,
			(DWORD)(ID / (10000ULL * 10000ULL * 10000ULL * 10000ULL)) % 10000,
			(DWORD)(ID / (10000ULL * 10000ULL * 10000ULL)) % 10000,
			(DWORD)(ID / (10000ULL * 10000ULL) % 10000ULL),
			(DWORD)(ID / 10000ULL % 10000ULL),
			(DWORD)(ID % 10000ULL));
}


const char CBcasCard::GetCardManufacturerID() const
{
	return m_BcasCardInfo.CardManufacturerID;
}


const BYTE CBcasCard::GetCardVersion() const
{
	return m_BcasCardInfo.CardVersion;
}
