#if !defined(AFX_DLGMENU05_H__124A9844_FBEE_4383_B010_090D3E38035D__INCLUDED_)
#define AFX_DLGMENU05_H__124A9844_FBEE_4383_B010_090D3E38035D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMenu05.h : header file
//
#include "MyStatic.h"
#include "DlgUtil01.h"
#include "MyLabel.h"

#define MAX_MENU05_LABEL			2


/////////////////////////////////////////////////////////////////////////////
// CDlgMenu05 dialog

class CDlgMenu05 : public CDialog
{
	CRect* m_pRect;
	CFont m_FontOfListCtrl;
	int m_nCurSelLotIdx, m_nCurSelLayerIdx;
	int m_nCntModelNames, m_nCntLotNames; 
	CString m_strLotStartTime, m_strLotEndTime, m_strLotWorkingTime;
	int m_nEntirePieceNum, m_nGoodPieceNum, m_nDefectPieceNum, m_nEntireStripDef[4];
	int m_nEntireStripNum;

	//CString m_strDefectTitle[MAX_DEF];

	int m_nDefStrip[4], m_nDefPerStrip[4][MAX_DEF];
	int m_nStripOut[4], m_nTotStOut;
	int m_nEntireAddedDefect[MAX_DEF];
// 	int m_nArrayDefNum[MAX_DEF][MAX_DEF];

	CMyStatic myStcLot, myStcSerialSt, myStcSerialEd, myStcFromTo, myStcProc, myStcIts;
	CString m_sEditRst;//, m_sRmapPath; 
	int m_nSerialSt, m_nSerialEd, m_nMarkedShot, m_nCompletedShot;
	int m_nNodeX, m_nNodeY;

	CDlgUtil01 *m_pDlgUtil01;
	CMyLabel myLabel[MAX_MENU05_LABEL];
	BOOL m_bLoadImg;
	CString m_sLayerUp, m_sLayerDn;

	void InitLabel();
	void LoadImg();
	void DelImg();

	void ModifyModelData();
	void ModifyLotData();
	void ModifyLayerData();
	void DispTestMode();
	//void DisplayReelMapData();
	void DisplayReelMapUser();
	void DisplayResultData();
	int CntModelNames();
	int CntLotNamesInModelFolder();
// 	CString SearchLayerName();
	CString DisplayData();
	BOOL ShowKeypad(int nCtlID, CPoint ptSt=(0, 0), int nDir=TO_NONE);
	void InitStc();
	//CString TxtData();
	BOOL ShowMyKeypad(int nCtlID, CPoint ptSt=(0, 0), int nDir=TO_NONE);
	BOOL ReloadReelmap();
	void InsertLine(CString sPath);
	void DispProcCode(CString sPath);
	void DispItsCode();
	CString GetItsCode();
	CString GetProcCode(CString sPath);
	void Print(HWND hWndMain);
	void PrintData(HDC hPrtdc, int xpage, int ypage);
	void ShowDlg(int nID);
	CString LoadFile(CString sPath);

// Construction
public:
	CDlgMenu05(CWnd* pParent = NULL);   // standard constructor
	~CDlgMenu05();

	CString m_sModel, m_sLot, m_sLayer, m_sProcessNum, m_sRmapPath;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();
	void InitModel();
	BOOL GetResult();
	void MakeSapp3();
	void MakeIts();
	void MakeItsFile(int nSerial, int nLayer);
	CString GetItsFileData(int nSerial, int nLayer); // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
	CString Sapp3Data();
	CString TxtDataMDS();

	int LoadPCRUpFromMk(int nSerial);
	int LoadPCRDnFromMk(int nSerial);

	int GetIdxTopLayer();
	void SelchangeComboLayer(int nIndex);

	int GetLayer(CString strLayer); // RMAP_NONE = -1, RMAP_UP = 0, RMAP_DN = 1


// Dialog Data
	//{{AFX_DATA(CDlgMenu05)
	enum { IDD = IDD_DLG_MENU_05 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenu05)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMenu05)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeComboModel();
	afx_msg void OnSelchangeListLot();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSearch();
	afx_msg void OnChkReelmap();
	afx_msg void OnStcLot();
	afx_msg void OnSelchangeComboLayer();
	afx_msg void OnBtnSave();
	afx_msg void OnStcSerialSt();
	afx_msg void OnStcSerialEd();
	afx_msg void OnStcProc();
	afx_msg void OnBtnSave4();
	afx_msg void OnBtnSave3();
	afx_msg void OnBtnSave2();
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave5();
	afx_msg void OnStnClickedStcIts();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENU05_H__124A9844_FBEE_4383_B010_090D3E38035D__INCLUDED_)
