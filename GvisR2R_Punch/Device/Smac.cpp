// Smac.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "Smac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc *pDoc;
extern CGvisR2R_PunchView *pView;

/////////////////////////////////////////////////////////////////////////////
// CSmac

CSmac::CSmac(CWnd* pParent /*=NULL*/)
{
	m_pParent = pParent;
	m_bRs232Run = FALSE;
	CWnd::Create(NULL,NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, CRect(0,0,0,0), pParent, 0);

	m_bTimerStop = FALSE;
	m_bReturnCmdEnd = FALSE;
	m_bRunTimerCheckEnd = FALSE;

	m_nCh = 0;
	m_bCh = FALSE;

	m_nStepIsDoneMark = 0;
	m_bDoneMark = FALSE;
	m_bMisMark = FALSE;
	m_dFinalMkPos = 0.0;
}

CSmac::~CSmac()
{
	Close();
}


BEGIN_MESSAGE_MAP(CSmac, CWnd)
	//{{AFX_MSG_MAP(CSmac)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECEIVE_RS232, OnReceiveRs232)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSmac message handlers

void CSmac::Init()
{
	m_Rs232.SetHwnd(this);
	m_Rs232.m_nPort = (BYTE)_tstoi(pDoc->WorkingInfo.VoiceCoil[m_nCh].sPort);
	m_Rs232.m_dwBaudRate = (DWORD)_tstoi(pDoc->WorkingInfo.VoiceCoil[m_nCh].sBaudRate);
	m_Rs232.m_bCR = pDoc->WorkingInfo.VoiceCoil[m_nCh].bCr;
	m_Rs232.m_bLF = pDoc->WorkingInfo.VoiceCoil[m_nCh].bLf;
	m_bRs232Run = m_Rs232.OpenComPort();

#ifdef USE_SMAC
	if(!m_bRs232Run)
	{
		CString sMsg;
		sMsg.Format(_T("보이스코일이 초기화되지 않았습니다.(COM%d)"), m_Rs232.m_nPort);
//		pView->MsgBox(_T("보이스코일이 초기화되지 않았습니다.");
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	m_sRcvRs232 = _T("");
}

void CSmac::Close()
{
	m_bRunTimerCheckEnd = FALSE;

	if(m_bRs232Run)
	{
		m_Rs232.DestroyRs232();
		m_bRs232Run = FALSE;
	}
}

CString CSmac::Rcv()
{
	CString sRcv = m_sRcvRs232;
	m_Rs232.ClearReceive();
	m_sRcvRs232 = _T("");

	return sRcv;
}

BOOL CSmac::Send(CString str)
{
	int nLen = str.GetLength();
	
	char* pRtn = NULL;
	char* cSend = new char[nLen+1];
	strcpy(cSend, pRtn=StringToChar(str));
	BOOL bRtn = m_Rs232.WriteRs232Block(cSend, nLen);
	delete cSend;

	if(pRtn)
		delete pRtn;

	return TRUE;
}

void CSmac::SetCh(int nCh)
{
	m_nCh = nCh;
}

void CSmac::Reset(int nCh)
{

}

void CSmac::SearchHomeSmac()
{
#ifdef USE_SMAC
	CString strSend;
	strSend.Format(_T("MS220")); // Homming
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	Wait(30);
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("OK")))
	{
		SetEsc();
		pView->ClrDispMsg();
		if(!m_nCh)
			AfxMessageBox(_T("Fail Homming - First SMAC."));
		else
			AfxMessageBox(_T("Fail Homming - Second SMAC."));
	}
#endif
}

BOOL CSmac::IsDoneSearchHomeSmac()
{
	if(!WaitSmacCmdEnd())
		return FALSE;

	ClearReceive();
	return TRUE;
}


double CSmac::GetSmacPosition()
{
	double dGetSmacPos=0.0;
	int nPos1, nPos2, nGetSmacPos;
	CString strSend, strVal;
	strSend.Format(_T("MS200")); // Get Position.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if(!WaitSmacCmdEnd(_T("OK")))
	{
		SetEsc();
		pView->ClrDispMsg(); 
		if(!m_nCh)
			AfxMessageBox(_T("Fail GetPosition First Smac."));
		else
			AfxMessageBox(_T("Fail GetPosition Second Smac."));
		return 0.0;
	}

	strVal = m_strReceiveVoiceCoil;

	nPos1 = strVal.Find(_T("OK"));
	nPos2 = strVal.GetLength();
	strVal = strVal.Left(nPos1-3);

	nPos1 = strVal.ReverseFind(':');
	nPos2 = strVal.GetLength();
	strVal.Delete(0, nPos1+3);

	nGetSmacPos = _tstoi(strVal);
	dGetSmacPos = double(nGetSmacPos * 0.005 ); // 엔코더 해상도 : [5um / 1pluse]
	strVal.Format(_T("%.3f"), dGetSmacPos);
	m_strReceiveVoiceCoil = _T("");

	return dGetSmacPos;
}

void CSmac::MoveSmacShiftPos()
{
	CString strSend;
	strSend.Format(_T("MS40")); // Move Shift Position.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("OK")))
	{
		SetEsc();
		pView->ClrDispMsg(); 
		if (!m_nCh)
			AfxMessageBox(_T("Fail Moving Shift Position(Left)."));
		else
			AfxMessageBox(_T("Fail Moving Shift Position(Right)."));
	}

	Sleep(30);
}

BOOL CSmac::IsDoneMoveSmacShiftPos()
{
	if (!WaitSmacCmdEnd())
		return FALSE;
	ClearReceive();
	return TRUE;
}

void CSmac::MoveSmacFinalPos()
{
	double dPos;
	CString strSend;
	strSend.Format(_T("MS50")); // Move Final Position.
	ClearReceive();
	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("OK")))
	{
		SetEsc();
		pView->ClrDispMsg(); 
		if (!m_nCh)
			AfxMessageBox(_T("Fail Moving Final Position - First Smac."));
		else
			AfxMessageBox(_T("Fail Moving Final Position - Second Smac."));
	}
	
	dPos = GetSmacPosition();
}

void CSmac::MoveSmacMeasPos()
{
	double dPos;
	CString strSend;
	strSend.Format(_T("MS70")); // Move Measuring Position with Prob.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("OK")))
	{
		SetEsc();
		pView->ClrDispMsg();
		if (!m_nCh)
			AfxMessageBox(_T("Fail Moving Final Position - First Smac."));
		else
			AfxMessageBox(_T("Fail Moving Final Position - Second Smac."));
	}

	dPos = GetSmacPosition();
}

void CSmac::ResetSmac() 
{
	CString strSend;
	strSend.Format(_T("MS110")); // Reset.
	ClearReceive();

	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
}

CString CSmac::GetSmacStatus()
{
	double dGetSmacPos=0.0;
	CString strSend, strVal;
	strSend.Format(_T("MS20")); // Get Status.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");

	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		SetEsc();
		pView->ClrDispMsg(); 
		if (!m_nCh)
			AfxMessageBox(_T("Fail GetStatus First Smac - First Smac."));
		else
			AfxMessageBox(_T("Fail GetStatus First Smac - Second Smac."));
		return _T("NG");
	}

	if (m_strReceiveVoiceCoil.Find(_T(":STATUS_OK;")) != -1)
		strVal = _T("Good");

	if (m_strReceiveVoiceCoil.Find(_T(":SERVO_OFF_OK;")) != -1)
		strVal = _T("Motor Off");

	if (m_strReceiveVoiceCoil.Find(_T(":SERVO_ERROR_OK;")) != -1)
		strVal = _T("Error");

	m_strReceiveVoiceCoil = _T("");

	return strVal;
}




double CSmac::GetSmacMeasureOfSurface()
{
	CString strVal;
	double dPos, dFinalPos;
	CString strSend;
	int nPos1, nPos2;

	strSend.Format(_T("MS70")); // Measure Surface.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("OK")))
	{
		SetEsc();
		pView->ClrDispMsg(); 
		if (!m_nCh)
			AfxMessageBox(_T("Fail Measuring Surface First Smac."));
		else
			AfxMessageBox(_T("Fail Measuring Surface First Smac."));

		//dPos = GetSmacPosition();
		return 0.0;
	}

	strVal = m_strReceiveVoiceCoil;

	nPos1 = strVal.Find(_T("OK"));
	nPos2 = strVal.GetLength();
	strVal = strVal.Left(nPos1 - 3);

	nPos1 = strVal.ReverseFind(':');
	nPos2 = strVal.GetLength();
	strVal.Delete(0, nPos1 + 3);

	dFinalPos = _tstoi(strVal);

	m_strReceiveVoiceCoil = _T("");
	Sleep(500);
	SearchHomeSmac();
		
	return (double(dFinalPos * 0.005));	
}

void CSmac::SendStringToVoiceCoil(CString strSend)
{
	CString strTemp = strSend +_T("\n");
	TRACE(strTemp);
	int strSendLength;
	
	strSendLength=strSend.GetLength();

	LPWSTR pchData = new WCHAR[strSendLength];
	VARIANT vrOutp;
	char* pRtn = NULL;
	MultiByteToWideChar(CP_ACP, 0, pRtn=StringToChar(strSend), strSendLength, pchData, strSendLength);
	vrOutp.vt = VT_BSTR;
	vrOutp.bstrVal = ::SysAllocStringLen(pchData, strSendLength);

	delete pchData;

	if(pRtn)
		delete pRtn;
	
	Send(strSend);
	::SysFreeString(vrOutp.bstrVal);
}

BOOL CSmac::SetWaitSmacCmdEnd(CString strEndCmd)
{
	m_bReturnCmdEnd = FALSE;
	if (!m_bRunTimerCheckEnd)
	{
		m_bRunTimerCheckEnd = TRUE;
		m_strEndCmd = strEndCmd;
		if (!m_bTimerStop)
			SetTimer(TIMER_CHECK_END_CMD, 100, NULL);
	}

	return TRUE;
}

BOOL CSmac::WaitSmacCmdEnd()
{
	if (!m_bReturnCmdEnd)
		return FALSE;

	m_strEndCmd = _T("");
	return TRUE;
}
	
BOOL CSmac::WaitSmacCmdEnd(CString strEndCmd)
{
	
	DWORD CurTimer, StartTimer;
	MSG message;

	StartTimer=GetTickCount();
	CurTimer=GetTickCount();

	m_bReturnCmdEnd = FALSE;
	if (!m_bRunTimerCheckEnd)
	{
		m_bRunTimerCheckEnd = TRUE;
		m_strEndCmd = strEndCmd;
		if (!m_bTimerStop)
			SetTimer(TIMER_CHECK_END_CMD, 100, NULL);
	}

	while (DELAY_TIME_ONE_MINUTE > int(CurTimer - StartTimer) && !m_bReturnCmdEnd)
	{
		CurTimer = GetTickCount();
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (!IsDialogMessage(&message))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
	}

	m_strEndCmd = _T("");

	if (DELAY_TIME_ONE_MINUTE <= int(CurTimer - StartTimer))
	{
		if (m_bReturnCmdEnd)
		{
			int sydTest = 0;
		}
		else
		{
			pView->ClrDispMsg(); 

			if (!m_nCh)
				AfxMessageBox(_T("Error, Wait Smac Cmd End - First SMAC."));
			else
				AfxMessageBox(_T("Error, Wait Smac Cmd End - Second SMAC."));

			return FALSE;
		}
	}

	return TRUE;
}


void CSmac::Wait(int imSecond)
{
	DWORD CurTimer, StartTimer;
	MSG message;
	StartTimer=GetTickCount();
	CurTimer=GetTickCount();
	while(imSecond > int(CurTimer-StartTimer))
	{
		CurTimer=GetTickCount();
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
}


int CSmac::CheckSmacErrorCode()
{
	int nLength, nPos, nErrCode=0;
	CString strVal, strTemp; 

	strVal = m_strReceiveVoiceCoil;

	if (strVal.Find(_T("?")) != -1)
	{
		nPos = strVal.Find(_T("?"));
		nLength = strVal.Delete(0, nPos + 1);
		nErrCode = _tstoi(strVal);
		strTemp.Format(_T("Error-%d"), nErrCode);
	}

	return nErrCode; // 0 : Not exist error code...
}

CString CSmac::DisplaySmacErrorList(int nErrCode)
{
	CString strError, strVal;

	switch(nErrCode)
	{
	case 0:
		strError = _T("NONE");
		return strError;
	case 1:
		strError = _T("ARGUMENT ERROR");
		break;
	case 2:
		strError = _T("INVALID COMMAND");
		break;
	case 3:
		strError = _T("INVALID MACRO COMMAND");
		break;
	case 4:
		strError = _T("MACRO ARGUMENT ERROR");
		break;
	case 5:
		strError = _T("MACRO NOT DEFINED");
		break;
	case 6:
		strError = _T("MACRO OUT OF RANGE");
		break;
	case 7:
		strError = _T("OUT OF MACRO SPACE");
		break;
	case 8:
		strError = _T("CAN'T DEFINE MACRO IN A MACRO");
		break;
	case 9:
		strError = _T("CAN'T DEFINE MACRO WHILE SERVO ENABLED");
		break;
	case 10:
		strError = _T("MACRO JUMP ERROR");
		break;
	case 11:
		strError = _T("OUT OF MACRO STACK SPACE");
		break;
	case 12:
		strError = _T("MACRO MUST BE FIRST COMMAND");
		break;
	case 13:
		strError = _T("STRING ERROR");
		break;
	case 14:
		strError = _T("MACRO STRING ERROR");
		break;
	case 15:
		strError = _T("SYNTAX ERROR");
		break;
	case 16:
		strError = _T("MACRO SYNTAX ERROR");
		break;
	case 17:
		strError = _T("AXIS RANGE ERROR");
		break;
	case 18:
		strError = _T("INTERRUPT MACRO NOT DEFINED");
		break;
	case 19:
		strError = _T("INTERRUPT MACRO STACK ERROR");
		break;
	case 20:
		strError = _T("MACRO STACK OVERFLOW");
		break;
	case 21:
		strError = _T("MACRO STACK UNDERFLOW");
		break;
	default:
		strError=_T("");
		return strError;
	}
		
	if (!m_nCh)
		strVal.Format(_T("First SMAC Error code is %s ."), strError);
	else
		strVal.Format(_T("Second SMAC Error code is %s ."), strError);
	
	pView->ClrDispMsg(); 
	AfxMessageBox(strVal);

	return strError;
}

void CSmac::CalcAveSmacHeight()
{
	CString strVal;
	int i;
	double dAverDist=0.0;

	for (i = 0; i < 4; i++)
		dAverDist += _tstof(pDoc->WorkingInfo.VoiceCoil[m_nCh].sSmacHeightAtPos[i]);

	strVal.Format(_T("%.3f"), dAverDist / 4.0);
	pDoc->WorkingInfo.Marking[m_nCh].sAverDist = strVal;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), strVal, PATH_WORKING_INFO);
}

LRESULT CSmac::OnReceiveRs232(WPARAM wP, LPARAM lP)
{
	CString str;
	if(m_sRcvRs232.IsEmpty())
		m_sRcvRs232.Format(_T("%s"),m_Rs232.m_cRxBuffer);
	else
	{
		str.Format(_T("%s"),m_Rs232.m_cRxBuffer);
		m_sRcvRs232 += str;
		m_strReceiveVoiceCoil = m_sRcvRs232;
	}
	m_Rs232.ClearReceive();
	return 0L;
}

void CSmac::ClearReceive()
{
	m_sRcvRs232 = _T("");
}

BOOL CSmac::SetCmdEndChk(CString strEndCmd)
{
	
	DWORD CurTimer, StartTimer;
	MSG message;
	
	StartTimer=GetTickCount();
	CurTimer=GetTickCount();
	
	m_bReturnCmdEnd = FALSE; // Chk 
	if (!m_bRunTimerCheckEnd)
	{
		m_bRunTimerCheckEnd = TRUE;
		m_strEndCmd = strEndCmd;
		if (!m_bTimerStop)
			SetTimer(TIMER_CHECK_END_CMD, 100, NULL);
	}
	else
		return FALSE;
	
	return TRUE;
}


void CSmac::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int nErrCode;

	switch (nIDEvent)
	{
	case TIMER_CHECK_END_CMD:
		KillTimer(TIMER_CHECK_END_CMD);
		if (m_bRunTimerCheckEnd)
		{
			if (m_strReceiveVoiceCoil.Find(m_strEndCmd) != -1)
			{
				m_bRunTimerCheckEnd = FALSE;
				m_bReturnCmdEnd = TRUE;
				Sleep(10);
			}
			else
				SetTimer(TIMER_CHECK_END_CMD, 100, NULL);
		}
		break;
	//case TIMER_CHECK_END_CMD:
	//	if(m_strReceiveVoiceCoil.Find(m_strEndCmd) != -1)
	//	{
	//		if(m_bRunTimerCheckEnd)
	//		{
	//			KillTimer(TIMER_CHECK_END_CMD);
	//			m_bRunTimerCheckEnd = FALSE;
	//		}
	//		m_bReturnCmdEnd = TRUE;
	//		Sleep(10);
	//	}
	//	break;
	case TIMER_CHECK_ERROR_CODE:    
		KillTimer(TIMER_CHECK_ERROR_CODE);
		nErrCode = CheckSmacErrorCode();
		if(nErrCode) // 0: No Error...
			DisplaySmacErrorList(nErrCode);
		break;
	default:    //MPIMessageFATAL_ERROR;		
		break;
	}

	CWnd::OnTimer(nIDEvent);
}


void CSmac::SetMarkShiftData()
{
	double dShiftPos, dShiftVel, dShiftAcc;
	CString strVal;
	
	dShiftPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitPos) * 200;
	dShiftVel = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitVel) * 2621;
	dShiftAcc = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitAcc) * 0.5;
	
	strVal.Format(_T("AL%d,AR14"), int(dShiftPos));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR30"), int(dShiftVel));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR32"), int(dShiftAcc));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!m_bTimerStop)
		SetTimer(TIMER_CHECK_ERROR_CODE, 100, NULL);
}

void CSmac::SetMarkFinalData()
{
	double dFinalPos, dFinalVel, dFinalAcc, dFinalTorque, dFinalTorqueOffset;
	CString strVal;
	
	dFinalPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingPos) * 200;
	dFinalVel = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingVel) * 2621;	// 100[mm/s] = SV26214
	dFinalAcc = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingAcc) * 0.5;	// 100[mm/s2] = SA52
	dFinalTorque = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingToq)/3.19;	// [-32767~32767]
	dFinalTorqueOffset = 8000 + (_tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingToqOffset))*18.0;
	
	if(dFinalTorque > 1024.0)
		dFinalTorque = 1024.0;
	if(dFinalTorqueOffset > 32767.0)
		dFinalTorqueOffset = 32767.0;
	
	strVal.Format(_T("AL%d,AR16"), int(dFinalPos));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR20"), int(dFinalVel));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR22"), int(dFinalAcc));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR40"), int(dFinalTorque));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR48"), int(dFinalTorqueOffset));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!m_bTimerStop)
		SetTimer(TIMER_CHECK_ERROR_CODE, 100, NULL);
}


void CSmac::SetMarking()
{
	CString strSend;
	strSend.Format(_T("MS60")); // Punching
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("PUNCHING_OK")))
	{
		SetEsc();
		pView->ClrDispMsg();
		if (!m_nCh)
			AfxMessageBox(_T("Fail Punching First Smac."));
		else
			AfxMessageBox(_T("Fail Punching Second Smac."));
	}
	Sleep(30);
}

BOOL CSmac::SetMark()
{
	if (m_bRunTimerCheckEnd)
		return FALSE;

	CString strSend;
	strSend.Format(_T("MS60")); // Punching
	ClearReceive();
	
	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	return SetCmdEndChk(_T("PUNCHING_OK"));
}

BOOL CSmac::IsMissMark()
{
	return m_bMisMark;
}

double CSmac::GetMkFinalPos()
{
	return m_dFinalMkPos;
}

BOOL CSmac::IsDoneMark(int nMkPcsIdx)
{
	//return m_bReturnCmdEnd;
	double dPos, dShiftPos, dFinalPos, dJudgePos;
	int nPos, nSerial;
	CString sPos, sLog;

	if (!pDoc->m_bChkSmacWaitPos)
	{
		if (!m_bDoneMark)
		{
			if (m_bReturnCmdEnd)
			{
				nPos = m_strReceiveVoiceCoil.Find(_T(":PUNCHING_OK"), 0);
				if (nPos > 0)
				{
					sPos = m_strReceiveVoiceCoil.Left(nPos);
					nPos = sPos.ReverseFind('\r');
					sPos = sPos.Left(nPos);
					nPos = sPos.Find('\n');
					sPos = sPos.Right(nPos + 1);
					m_dFinalMkPos = _tstof(sPos) * 0.005; // 엔코더 해상도 : [5um / 1pluse]
					dFinalPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingPos);
					dJudgePos = dFinalPos + _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingSensingPosOffset);
					if (m_dFinalMkPos < dJudgePos) // 미마킹으로 판단
						m_bMisMark = TRUE;
					else
						m_bMisMark = FALSE;
					nSerial = pView->m_nBufUpSerial[m_nCh]; // Cam1
					if (m_nCh)
						sLog.Format(_T("%s DN%d: %.3f"), pView->GetMkInfo1(nSerial, nMkPcsIdx, FALSE), m_nCh, m_dFinalMkPos);
					else
						sLog.Format(_T("%s DN%d: %.3f"), pView->GetMkInfo0(nSerial, nMkPcsIdx, FALSE), m_nCh, m_dFinalMkPos);
					pDoc->LogPunch(sLog);
				}
				else
				{
					pDoc->LogPunch(_T("No_Return"));
					m_bMisMark = TRUE;
				}
				m_bDoneMark = TRUE;
				return TRUE;
			}

			return FALSE;
		}
		else
			return TRUE;
	}
	else
	{
		if (!m_bDoneMark)
		{
			switch (m_nStepIsDoneMark)
			{
			case 0:
				if (m_bReturnCmdEnd)
				{
					nPos = m_strReceiveVoiceCoil.Find(_T(":PUNCHING_OK"), 0);
					if (nPos > 0)
					{
						sPos = m_strReceiveVoiceCoil.Left(nPos);
						nPos = sPos.ReverseFind('\r');
						sPos = sPos.Left(nPos);
						nPos = sPos.Find('\n');
						sPos = sPos.Right(nPos + 1);
						m_dFinalMkPos = _tstof(sPos) * 0.005; // 엔코더 해상도 : [5um / 1pluse]
						dFinalPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingPos);
						dJudgePos = dFinalPos + _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingSensingPosOffset);
						if (m_dFinalMkPos < dJudgePos) // 미마킹으로 판단
							m_bMisMark = TRUE;
						else
							m_bMisMark = FALSE;
						nSerial = pView->m_nBufUpSerial[m_nCh]; // Cam1
						if (m_nCh)
							sLog.Format(_T("%s DN%d: %.3f"), pView->GetMkInfo1(nSerial, nMkPcsIdx, FALSE), m_nCh, m_dFinalMkPos);
						else
							sLog.Format(_T("%s DN%d: %.3f"), pView->GetMkInfo0(nSerial, nMkPcsIdx, FALSE), m_nCh, m_dFinalMkPos);
						pDoc->LogPunch(sLog);
					}
					else
					{
						pDoc->LogPunch(_T("No_Return"));
						m_bMisMark = TRUE;
					}
					m_nStepIsDoneMark++;
				}
				break;
			case 1:
				dShiftPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitPos);
				dFinalPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingPos);
				dJudgePos = (dShiftPos + dFinalPos) / 2.0;
				dPos = GetSmacPosition();
				if (dPos < dJudgePos)	// 대기위치까지 무한 대기 상태
				{
					nSerial = pView->m_nBufUpSerial[m_nCh]; // Cam1
					if (m_nCh)
						sLog.Format(_T("%s UP%d: %.3f"), pView->GetMkInfo1(nSerial, nMkPcsIdx, FALSE), m_nCh, dPos);
					else
						sLog.Format(_T("%s UP%d: %.3f"), pView->GetMkInfo0(nSerial, nMkPcsIdx, FALSE), m_nCh, dPos);
					pDoc->LogPunch(sLog);
					m_nStepIsDoneMark++;
				}
				else
					Sleep(30);
				break;
			case 2:
				m_bDoneMark = TRUE;
				return TRUE;
			}
			return FALSE;
		}
		else
			return TRUE;
	}
}

// Elec Check =================================

void CSmac::SetProbShiftData()
{
	double dShiftPos, dShiftVel, dShiftAcc;
	CString strVal;
	
	dShiftPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitPos) * 200;
	dShiftVel = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitVel) * 2621;
	dShiftAcc = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitAcc) * 0.5;
	
	strVal.Format(_T("AL%d,AR54"), int(dShiftPos));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR70"), int(dShiftVel));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR72"), int(dShiftAcc));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!m_bTimerStop)
		SetTimer(TIMER_CHECK_ERROR_CODE, 100, NULL);
}

void CSmac::SetProbFinalData()
{
	double dFinalPos, dFinalVel, dFinalAcc, dFinalTorque;
	CString strVal;
	
	dFinalPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingPos) * 200;
	dFinalVel = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingVel) * 2621;	// 100[mm/s] = SV26214
	dFinalAcc = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingAcc) * 0.5;		// 100[mm/s2] = SA52
	dFinalTorque = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingToq)/3.19;	// [-32767~32767]

	if(dFinalTorque > 32767.0)
		dFinalTorque = 32767.0;

	strVal.Format(_T("AL%d,AR56"), int(dFinalPos));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR70"), int(dFinalVel));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR72"), int(dFinalAcc));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	strVal.Format(_T("AL%d,AR80"), int(dFinalTorque));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!m_bTimerStop)
		SetTimer(TIMER_CHECK_ERROR_CODE, 100, NULL);
}


void CSmac::ResetStepIsDoneMark()
{
	m_nStepIsDoneMark = 0;
	m_bDoneMark = FALSE;
	m_bMisMark = FALSE;
}


void CSmac::SetEsc()
{
	if (m_bRunTimerCheckEnd)
		m_bRunTimerCheckEnd = FALSE;
#ifdef USE_SMAC
	CString strSend;
	strSend.Format(_T("%c"), 27);	// Escape(27)
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	Wait(30);
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
#endif
}
