// DlgOption01.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgOption01.h"
//#include "afxdialogex.h"


#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;


// CDlgOption01 대화 상자입니다.

//IMPLEMENT_DYNAMIC(CDlgOption01, CDialog)

CDlgOption01::CDlgOption01(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOption01::IDD, pParent)
{
	m_pRect = NULL;
	m_bTIM_DISP_STS = FALSE;
}

CDlgOption01::~CDlgOption01()
{
	m_bTIM_DISP_STS = FALSE;
	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgOption01::Create()
{
	return CDialog::Create(CDlgOption01::IDD);
}

void CDlgOption01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOption01, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK1, &CDlgOption01::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDlgOption01::OnBnClickedCheck2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgOption01 메시지 처리기입니다.


void CDlgOption01::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_pRect)
	{
		m_pRect = new CRect;

		this->GetClientRect(m_pRect);
		m_pRect->top = 375;
		m_pRect->bottom += 375 + GetSystemMetrics(SM_CYSIZE);
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if (bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgOption01::AtDlgShow()
{
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(pDoc->m_bOffLogAuto);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(pDoc->m_bOffLogPLC);
}

void CDlgOption01::AtDlgHide()
{

}


BOOL CDlgOption01::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_bTIM_DISP_STS = TRUE;
	SetTimer(TIM_DISP_STS, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgOption01::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	pDoc->m_bOffLogAuto = bOn;

	if(bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("OffLogAuto"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("OffLogAuto"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnBnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	pDoc->m_bOffLogPLC = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("OffLogPLC"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("OffLogPLC"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIM_DISP_STS)
	{
		KillTimer(TIM_DISP_STS);
		if (this->IsWindowVisible())
		{
			m_bTIM_DISP_STS = FALSE;
		}
		else
		{
			this->ShowWindow(SW_SHOW);
		}
		if (m_bTIM_DISP_STS)
			SetTimer(TIM_DISP_STS, 100, NULL);
	}


	CDialog::OnTimer(nIDEvent);
}
