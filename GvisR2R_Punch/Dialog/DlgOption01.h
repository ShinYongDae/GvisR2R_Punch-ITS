#pragma once
#include <afxwin.h>

// CDlgOption01 ��ȭ �����Դϴ�.

#define TIM_DISP_STS				450

class CDlgOption01 : public CDialog
{
	//DECLARE_DYNAMIC(CDlgOption01)

	BOOL m_bTIM_DISP_STS;

public:
	CDlgOption01(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgOption01();

	CRect* m_pRect;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

// ��ȭ ���� �������Դϴ�.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OPTION_01 };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
