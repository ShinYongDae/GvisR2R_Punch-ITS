#if !defined(AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_)
#define AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Smac.h : header file
//
#include "Rs232Smac.h"

#define TIMER_GET_POS			100
#define TIMER_CHECK_END_CMD		110
#define TIMER_CHECK_ERROR_CODE	120

#define DELAY_SMAC				3000

/////////////////////////////////////////////////////////////////////////////
// CSmac window

class CSmac : public CWnd
{
// Construction
	CWnd*			m_pParent;
	CCriticalSection m_cs;
	BOOL m_bCh;
	int m_nCh;
	CString m_sRcvRs232;
	int m_nStepIsDoneMark;
	BOOL m_bDoneMark;
	BOOL m_bMisMark;
	double m_dFinalMkPos;

	BOOL Send(CString str);

public:
	CSmac(CWnd* pParent =NULL);

// Attributes
public:
	CRs232Smac m_Rs232;
	BOOL m_bRs232Run;

	CString m_strReceiveVoiceCoil;

	BOOL m_bReturnCmdEnd;
	BOOL m_bRunTimerCheckEnd;

	CString m_strEndCmd;
	BOOL m_bTimerStop, m_bFeedingRollerUp;

// Operations
public:
	afx_msg LRESULT OnReceiveRs232(WPARAM wP, LPARAM lP);
	CString Rcv();

	void ResetStepIsDoneMark();
	void Init();
	void SetCh(int nCh);
	void Reset(int nCh);
	void Close();

	double GetSmacMeasureOfSurface();
	void SendStringToVoiceCoil(CString strSend);
	BOOL WaitSmacCmdEnd(); // Return Value is TRUE (OK), FALSE (NG)
	BOOL SetWaitSmacCmdEnd(CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	BOOL WaitSmacCmdEnd(CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	double GetSmacPosition();
	void SearchHomeSmac();
	BOOL IsDoneSearchHomeSmac();
	int CheckSmacErrorCode();
	CString DisplaySmacErrorList(int nErrCode);
	void Wait(int imSecond);
	void CalcAveSmacHeight();
	void ResetSmac();
	CString GetSmacStatus();
	void MoveSmacFinalPos();
	void MoveSmacShiftPos();
	BOOL IsDoneMoveSmacShiftPos();
	void MoveSmacMeasPos();

	void SetMarking();
	void SetMarkShiftData();
	void SetMarkFinalData();
	BOOL SetMark();
	BOOL IsDoneMark(int nMkPcsIdx);
	BOOL IsMissMark();
	double GetMkFinalPos();
	
	void SetProbShiftData();
	void SetProbFinalData();

	BOOL SetCmdEndChk(CString strEndCmd);
	void ClearReceive();
	void SetEsc();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmac)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSmac();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSmac)
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_)
