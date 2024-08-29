// DlgMenu03.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgMenu03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../MainFrm.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu03 dialog


CDlgMenu03::CDlgMenu03(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu03::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenu03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_bTIM_MENU03_DISP = FALSE;
	m_bTIM_CHK_DONE_MK = FALSE;
	m_bTIM_CHK_DONE_AOI = FALSE;
	m_bTIM_CHK_DONE_MKAOI = FALSE;
	m_bTIM_CHK_DONE_BUF_HOME = FALSE;
	m_bTIM_CHK_DONE_BUF_INIT = FALSE;
	m_bTIM_CHK_DONE_READY = FALSE;
	m_bTIM_SET_RC_RELATION = FALSE;
	m_bTIM_SW_RC_RELATION = FALSE;
	m_bTIM_SW_UC_RELATION = FALSE;

	m_nFdSpd = 2; // Normal
	for(int i=0; i<MAX_AXIS; i++)
	{
		m_dPrevEnc[i] = 0.0;
		m_nDir[i] = 0; // -1 : CCW , 0 : NO Dir , 1 : CW
	}

	m_bBufInitPosMove = FALSE;
	m_bVMoveMkFdVac = FALSE;
	m_bVMoveAoiFdVac = FALSE;

	m_bTIM_CHK_DONE_ENGRAVE = FALSE;

	m_bTIM_CHK_DONE_ENG_BUF_HOME = FALSE;
	m_bTIM_CHK_DONE_ENG_BUF_INIT = FALSE;
}

CDlgMenu03::~CDlgMenu03()
{
	m_bTIM_MENU03_DISP = FALSE;
	m_bTIM_CHK_DONE_MK = FALSE;
	m_bTIM_CHK_DONE_AOI = FALSE;
	m_bTIM_CHK_DONE_MKAOI = FALSE;
	m_bTIM_CHK_DONE_BUF_HOME = FALSE;
	m_bTIM_CHK_DONE_BUF_INIT = FALSE;
	m_bTIM_CHK_DONE_READY = FALSE;
	m_bTIM_SET_RC_RELATION = FALSE;
	m_bTIM_SW_RC_RELATION = FALSE;
	m_bTIM_SW_UC_RELATION = FALSE;

	m_bTIM_CHK_DONE_ENGRAVE = FALSE;

	m_bTIM_CHK_DONE_ENG_BUF_HOME = FALSE;
	m_bTIM_CHK_DONE_ENG_BUF_INIT = FALSE;

	DelImg();
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgMenu03::Create()
{
	return CDialog::Create(CDlgMenu03::IDD);
}

void CDlgMenu03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenu03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenu03, CDialog)
	//{{AFX_MSG_MAP(CDlgMenu03)
	ON_BN_CLICKED(IDC_CHK_24, OnChk24)
	ON_BN_CLICKED(IDC_CHK_62, OnChk62)
	ON_BN_CLICKED(IDC_CHK_16, OnChk16)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHK_79, &CDlgMenu03::OnBnClickedChk79)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu03 message handlers

void CDlgMenu03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 75 + 2;
		m_pRect->bottom += 75 + 2;
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if(bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}	
}

void CDlgMenu03::AtDlgShow()
{
	LoadImg();
	//m_bTIM_MENU03_DISP = TRUE;
	//SetTimer(TIM_MENU03_DISP, 300, NULL);	// Disp();
	SetTimer(TIM_CHK_MREG, 300, NULL);

	UpdateSignal();
}

void CDlgMenu03::AtDlgHide()
{
	//m_bTIM_MENU03_DISP = FALSE;
	DelImg();
}

void CDlgMenu03::LoadImg()
{
	if (m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for (i = 0; i < MAX_MENU03_LABEL; i++)
	{
		myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(20, 20), LBL_POS_CENTER);
		myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(20, 20), LBL_POS_CENTER);
	}

	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		switch (i)
		{
		case 0:		// Run-Main
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 33:	// Stop-Main
		case 49:	// LaserPoint-Punching
		case 50:	// LaserPoint-AOI��
		case 65:	// LaserPoint-AOI��
		case 82:	// LaserPoint-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_RED_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 34:	// �����غ�-Main
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 1:		// Reset-Main
		case 40:	// ����-Uncoiler
		case 47:	// ����-Recoiler
			myBtn[i].LoadBkImage(IMG_CIR_YLO_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_YLO_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 4:		// ��������-Recoiler
		case 9:		// ��������-Punching
		case 17:	// ��������-AOI��
		case 25:	// ��������-Uncoiler
		case 55:	// ��������-AOI��
		case 72:	// ��������-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRY_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 2:		// TorqueMotor-Punching
		case 3:		// TorqueMotor-AOI
		case 5:		// ��ȸ��-Recoiler
		case 6:		// ��ȸ��-Recoiler
		case 10:	// ��ȸ��-Punching
		case 11:	// ��ȸ��-Punching
		case 18:	// ��ȸ��-AOI��
		case 19:	// ��ȸ��-AOI��
		case 26:	// ��ȸ��-Uncoiler
		case 27:	// ��ȸ��-Uncoiler
		case 32:	// ���� ��ȸ��-Uncoiler
		case 39:	// ���� ��ȸ��-Uncoiler
		case 45:	// ���� ��ȸ��-Uncoiler
		case 46:	// ���� ��ȸ��-Uncoiler
		case 56:	// ��ȸ��-AOI��
		case 57:	// ��ȸ��-AOI��
		case 70:	// Core150mm-Recoiler
		case 71:	// Core150mm-Uncoiler
		case 73:	// ��ȸ��-Engraving
		case 74:	// ��ȸ��-Engraving
		case 83:	// TorqueMotor-Engraving
		case 85:	// �����ļ�����-Engrave
		case 86:	// �����ļ�����-AOI_DN
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 12:	// �ǵ�����-Punching
		case 13:	// ��ǰǪ��-Punching
		case 20:	// �ǵ�����-AOI��
		case 21:	// ��ǰǪ��-AOI��
		case 28:	// ��ǰ���ѻ��-Uncoiler
		case 29:	// Ŭ���ѷ����-Uncoiler
		case 31:	// ����ô-Uncoiler
		case 35:	// ��ǰô-Uncoiler
		case 36:	// Ŭ���ѷ��������-Uncoiler
		case 41:	// ��ǰô-Recoiler
		case 42:	// ��ǰ���ѻ��-Recoiler
		case 44:	// ����ô-Recoiler
		case 48:	// ���ѻ��-Punching
		case 51:	// �ǵ�Ŭ����-Punching
		case 52:	// �ټ�Ŭ����-Punching
		case 53:	// �ǵ�Ŭ����-AOI��
		case 54:	// �ټ�Ŭ����-AOI��
		case 58:	// �ǵ�����-AOI��
		case 59:	// ��ǰǪ��-AOI��
		case 62:	// �ǵ�Ŭ����-AOI��
		case 63:	// �ټ�Ŭ����-AOI��
		case 66:	// Rewinder����-Recoiler
		case 67:	// Rewinder��ǰ����-Recoiler
		case 75:	// �ǵ�����-Engraving
		case 76:	// ��ǰǪ��-Engraving
		case 79:	// �ǵ�Ŭ����-Engraving
		case 80:	// �ټ�Ŭ����-Engraving
		case 84:	// ���ѻ��-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_DGN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_DGN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 7:		// ��ǰ�����Ż�¿�-Recoiler
		case 8:		// ��ǰ����������-Recoiler
		case 14:	// ���̺��ο�-Punching
		case 15:	// ���̺�����-Punching
		case 22:	// ���̺��ο�-AOI��
		case 23:	// ���̺�����-AOI��
		case 30:	// ��ǰ�����Ż����-Uncoiler
		case 37:	// ��ǰ�����Ż�¿�-Uncoiler
		case 38:	// ��ǰ����������-Uncoiler
		case 43:	// ��ǰ�����Ż����-Recoiler
		case 60:	// ���̺��ο�-AOI��
		case 61:	// ���̺�����-AOI��
		case 77:	// ���̺��ο�-Engraving
		case 78:	// ���̺�����-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_PNK_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_PNK_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 16:	// ���ǳ��̼�-Punching
		case 24:	// ���ǳ��̼�-AOI��	
		case 64:	// ���ǳ��̼�-AOI��	
		case 81:	// ���ǳ��̼�-Engraving	
			myBtn[i].LoadBkImage(IMG_CIR_ORG_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 68:	// InductionMotor-Recoiler
		case 69:	// InductionMotor-Uncoiler	
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		}
	}

	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[34].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[34].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
}

void CDlgMenu03::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;

	for (i = 0; i < MAX_MENU03_LABEL; i++)
		myLabel[i].DelImgList();

	for(i=0; i<MAX_MENU03_BTN; i++)
		myBtn[i].DelImgList();

	myStcTitle[33].DelImgList();
	myStcTitle[34].DelImgList();
	myStcTitle[35].DelImgList();
}

BOOL CDlgMenu03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStatic();
	InitBtn();
	InitGroup();
	InitLabel();

	GetDlgItem(IDC_STC_12_38)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_47)->ShowWindow(SW_HIDE);	// Recoiler-Stop

	GetDlgItem(IDC_STC_12_31)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_40)->ShowWindow(SW_HIDE);	// Uncoiler-Stop

	m_bTIM_MENU03_DISP = TRUE;
	SetTimer(TIM_MENU03_DISP, 300, NULL);	// Disp();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMenu03::InitGroup()
{
	myGrp[0].SubclassDlgItem(IDC_GRP_00, this);
	myGrp[0].SetHwnd(this->GetSafeHwnd(), IDC_GRP_00);	// Main

	myGrp[1].SubclassDlgItem(IDC_GRP_1, this);
	myGrp[1].SetHwnd(this->GetSafeHwnd(), IDC_GRP_1);	// Recoiler

	myGrp[2].SubclassDlgItem(IDC_GRP_02, this);
	myGrp[2].SetHwnd(this->GetSafeHwnd(), IDC_GRP_02);	// Punching

	myGrp[3].SubclassDlgItem(IDC_GRP_4, this);
	myGrp[3].SetHwnd(this->GetSafeHwnd(), IDC_GRP_4);	// AOI��

	myGrp[4].SubclassDlgItem(IDC_GRP_03, this);
	myGrp[4].SetHwnd(this->GetSafeHwnd(), IDC_GRP_03);	// AOI��

	myGrp[5].SubclassDlgItem(IDC_GRP_04, this);
	myGrp[5].SetHwnd(this->GetSafeHwnd(), IDC_GRP_04);	// Uncoiler

	myGrp[6].SubclassDlgItem(IDC_GRP_05, this);
	myGrp[6].SetHwnd(this->GetSafeHwnd(), IDC_GRP_05);	// TorqueMotor

	myGrp[7].SubclassDlgItem(IDC_GRP_16, this);
	myGrp[7].SetHwnd(this->GetSafeHwnd(), IDC_GRP_16);	// InductionMotor

	myGrp[8].SubclassDlgItem(IDC_GRP_17, this);
	myGrp[8].SetHwnd(this->GetSafeHwnd(), IDC_GRP_17);	// Core150mm

	myGrp[9].SubclassDlgItem(IDC_GRP_3, this);
	myGrp[9].SetHwnd(this->GetSafeHwnd(), IDC_GRP_3);	// Engraving

	for (int i = 0; i < MAX_MENU03_GRP; i++)
	{
		myGrp[i].SetFontName(_T("Arial"));
		myGrp[i].SetFontSize(14);
		myGrp[i].SetFontBold(TRUE);
		myGrp[i].SetTextColor(RGB_NAVY);
		myGrp[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu03::InitBtn()
{
	// Main
	myBtn[0].SubclassDlgItem(IDC_CHK_0, this);			// Run
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_0);
	myBtn[0].SetBoarder(FALSE);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[34].SubclassDlgItem(IDC_CHK_34, this);		// �����غ�
	myBtn[34].SetHwnd(this->GetSafeHwnd(), IDC_CHK_34);
	myBtn[34].SetBoarder(FALSE);
	myBtn[34].SetBtnType(BTN_TYPE_CHECK);

	myBtn[33].SubclassDlgItem(IDC_CHK_33, this);		// Stop
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_CHK_33);
	myBtn[33].SetBoarder(FALSE);
	myBtn[33].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_CHK_1, this);			// Reset
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1);
	myBtn[1].SetBoarder(FALSE);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	// Torque Motor
	myBtn[2].SubclassDlgItem(IDC_CHK_2, this);			// ��Ī��
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2);
	myBtn[2].SetBoarder(FALSE);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_3, this);			// �˻��
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_3);
	myBtn[3].SetBoarder(FALSE);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[83].SubclassDlgItem(IDC_CHK_84, this);		// ���κ�
	myBtn[83].SetHwnd(this->GetSafeHwnd(), IDC_CHK_84);
	myBtn[83].SetBoarder(FALSE);
	myBtn[83].SetBtnType(BTN_TYPE_CHECK);

	// ������ ������ �ӵ�
	myBtn[85].SubclassDlgItem(IDC_CHK_87, this);		// ������ ������ �ӵ� - ���κ�
	myBtn[85].SetHwnd(this->GetSafeHwnd(), IDC_CHK_87);
	myBtn[85].SetBoarder(FALSE);
	myBtn[85].SetBtnType(BTN_TYPE_CHECK);

	myBtn[86].SubclassDlgItem(IDC_CHK_88, this);		// ������ ������ �ӵ� - AOI �ϸ�
	myBtn[86].SetHwnd(this->GetSafeHwnd(), IDC_CHK_88);
	myBtn[86].SetBoarder(FALSE);
	myBtn[86].SetBtnType(BTN_TYPE_CHECK);

	// Induction Motor
	myBtn[68].SubclassDlgItem(IDC_CHK_68, this);		// Induction Motor - Recoiler\r������ ON
	myBtn[68].SetHwnd(this->GetSafeHwnd(), IDC_CHK_68);
	myBtn[68].SetBoarder(FALSE);
	myBtn[68].SetBtnType(BTN_TYPE_CHECK);

	myBtn[69].SubclassDlgItem(IDC_CHK_69, this);		// Induction Motor - Uncoiler\r������ ON
	myBtn[69].SetHwnd(this->GetSafeHwnd(), IDC_CHK_69);
	myBtn[69].SetBoarder(FALSE);
	myBtn[69].SetBtnType(BTN_TYPE_CHECK);

	// Core 150mm
	myBtn[70].SubclassDlgItem(IDC_CHK_70, this);		// Core 150mm - Recoiler
	myBtn[70].SetHwnd(this->GetSafeHwnd(), IDC_CHK_70);
	myBtn[70].SetBoarder(FALSE);
	myBtn[70].SetBtnType(BTN_TYPE_CHECK);

	myBtn[71].SubclassDlgItem(IDC_CHK_71, this);		// Core 150mm - Uncoiler
	myBtn[71].SetHwnd(this->GetSafeHwnd(), IDC_CHK_71);
	myBtn[71].SetBoarder(FALSE);
	myBtn[71].SetBtnType(BTN_TYPE_CHECK);

	// Recoiler
	myBtn[4].SubclassDlgItem(IDC_CHK_4, this);			// ��������
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_4);
	myBtn[4].SetBoarder(FALSE);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_5, this);			// ����ȸ��
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_5);
	myBtn[5].SetBoarder(FALSE);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	myBtn[6].SubclassDlgItem(IDC_CHK_6, this);			// ��ȸ����
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_6);
	myBtn[6].SetBoarder(FALSE);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	myBtn[41].SubclassDlgItem(IDC_CHK_41, this);		// ��ǰô\rŬ����
	myBtn[41].SetHwnd(this->GetSafeHwnd(), IDC_CHK_41);
	myBtn[41].SetBoarder(FALSE);
	myBtn[41].SetBtnType(BTN_TYPE_CHECK);

	myBtn[42].SubclassDlgItem(IDC_CHK_42, this);		// ��ǰ����\r���/�ϰ�
	myBtn[42].SetHwnd(this->GetSafeHwnd(), IDC_CHK_42);
	myBtn[42].SetBoarder(FALSE);
	myBtn[42].SetBtnType(BTN_TYPE_CHECK);

	myBtn[43].SubclassDlgItem(IDC_CHK_43, this);		// ��ǰ������(��)\r���/�ϰ�
	myBtn[43].SetHwnd(this->GetSafeHwnd(), IDC_CHK_43);
	myBtn[43].SetBoarder(FALSE);
	myBtn[43].SetBtnType(BTN_TYPE_CHECK);

	myBtn[7].SubclassDlgItem(IDC_CHK_7, this);			// ��ǰ������(��)\r���/�ϰ�
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_7);
	myBtn[7].SetBoarder(FALSE);
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	myBtn[8].SubclassDlgItem(IDC_CHK_8, this);			// ��ǰ������\r����
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_8);
	myBtn[8].SetBoarder(FALSE);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	myBtn[44].SubclassDlgItem(IDC_CHK_44, this);		// ���� ô\rŬ����
	myBtn[44].SetHwnd(this->GetSafeHwnd(), IDC_CHK_44);
	myBtn[44].SetBoarder(FALSE);
	myBtn[44].SetBtnType(BTN_TYPE_CHECK);

	myBtn[45].SubclassDlgItem(IDC_CHK_45, this);		// ���� ��\r����ȸ��
	myBtn[45].SetHwnd(this->GetSafeHwnd(), IDC_CHK_45);
	myBtn[45].SetBoarder(FALSE);
	myBtn[45].SetBtnType(BTN_TYPE_CHECK);

	myBtn[46].SubclassDlgItem(IDC_CHK_46, this);		// ���� ��\r��ȸ����
	myBtn[46].SetHwnd(this->GetSafeHwnd(), IDC_CHK_46);
	myBtn[46].SetBoarder(FALSE);
	myBtn[46].SetBtnType(BTN_TYPE_DEFAULT);

	myBtn[47].SubclassDlgItem(IDC_CHK_47, this);		// Recoiler-����
	myBtn[47].SetHwnd(this->GetSafeHwnd(), IDC_CHK_47);
	myBtn[47].SetBoarder(FALSE);
	myBtn[47].SetBtnType(BTN_TYPE_CHECK);

	myBtn[66].SubclassDlgItem(IDC_CHK_66, this);		// Rewinder\r����
	myBtn[66].SetHwnd(this->GetSafeHwnd(), IDC_CHK_66);
	myBtn[66].SetBoarder(FALSE);
	myBtn[66].SetBtnType(BTN_TYPE_CHECK);

	myBtn[67].SubclassDlgItem(IDC_CHK_67, this);		// Rewinder\r��ǰ&&����
	myBtn[67].SetHwnd(this->GetSafeHwnd(), IDC_CHK_67);
	myBtn[67].SetBoarder(FALSE);
	myBtn[67].SetBtnType(BTN_TYPE_CHECK);

	// Punching
	myBtn[9].SubclassDlgItem(IDC_CHK_9, this);			// ��������
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_9);
	myBtn[9].SetBoarder(FALSE);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);


	myBtn[10].SubclassDlgItem(IDC_CHK_10, this);		// ����ȸ��
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_10);
	myBtn[10].SetBoarder(FALSE);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);


	myBtn[11].SubclassDlgItem(IDC_CHK_11, this);		// ��ȸ����
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_11);
	myBtn[11].SetBoarder(FALSE);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);


	myBtn[12].SubclassDlgItem(IDC_CHK_12, this);		// �ǵ�����
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_12);
	myBtn[12].SetBoarder(FALSE);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);


	myBtn[13].SubclassDlgItem(IDC_CHK_13, this);		// ��ǰǪ��
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_13);
	myBtn[13].SetBoarder(FALSE);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);


	myBtn[14].SubclassDlgItem(IDC_CHK_14, this);		// ���̺�\r��ο�
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_14);
	myBtn[14].SetBoarder(FALSE);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);


	myBtn[15].SubclassDlgItem(IDC_CHK_15, this);		// ���̺�\r����
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_CHK_15);
	myBtn[15].SetBoarder(FALSE);
	myBtn[15].SetBtnType(BTN_TYPE_CHECK);


	myBtn[51].SubclassDlgItem(IDC_CHK_51, this);		// �ǵ�\rŬ����
	myBtn[51].SetHwnd(this->GetSafeHwnd(), IDC_CHK_51);
	myBtn[51].SetBoarder(FALSE);
	myBtn[51].SetBtnType(BTN_TYPE_CHECK);


	myBtn[52].SubclassDlgItem(IDC_CHK_52, this);		// �ټ�\rŬ����
	myBtn[52].SetHwnd(this->GetSafeHwnd(), IDC_CHK_52);
	myBtn[52].SetBoarder(FALSE);
	myBtn[52].SetBtnType(BTN_TYPE_CHECK);


	myBtn[16].SubclassDlgItem(IDC_CHK_16, this);		// ���ǳ�\r�̼�
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_16);
	myBtn[16].SetBoarder(FALSE);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);


	myBtn[49].SubclassDlgItem(IDC_CHK_49, this);		// ������\r��ũ
	myBtn[49].SetHwnd(this->GetSafeHwnd(), IDC_CHK_49);
	myBtn[49].SetBoarder(FALSE);
	myBtn[49].SetBtnType(BTN_TYPE_CHECK);


	myBtn[48].SubclassDlgItem(IDC_CHK_48, this);		// ����\r��� / �ϰ�
	myBtn[48].SetHwnd(this->GetSafeHwnd(), IDC_CHK_48);
	myBtn[48].SetBoarder(FALSE);
	myBtn[48].SetBtnType(BTN_TYPE_CHECK);


	// AOI ���.	
	myBtn[17].SubclassDlgItem(IDC_CHK_17, this);		// ��������
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_17);
	myBtn[17].SetBoarder(FALSE);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);


	myBtn[18].SubclassDlgItem(IDC_CHK_18, this);		// ����ȸ��
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_18);
	myBtn[18].SetBoarder(FALSE);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);


	myBtn[19].SubclassDlgItem(IDC_CHK_19, this);		// ��ȸ����
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_19);
	myBtn[19].SetBoarder(FALSE);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);


	myBtn[20].SubclassDlgItem(IDC_CHK_20, this);		// �ǵ�����
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_20);
	myBtn[20].SetBoarder(FALSE);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);


	myBtn[21].SubclassDlgItem(IDC_CHK_21, this);		// ��ǰǪ��
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_21);
	myBtn[21].SetBoarder(FALSE);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);


	myBtn[22].SubclassDlgItem(IDC_CHK_22, this);		// ���̺�\r��ο�
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_22);
	myBtn[22].SetBoarder(FALSE);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);


	myBtn[23].SubclassDlgItem(IDC_CHK_23, this);		// ���̺�\r����
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_CHK_23);
	myBtn[23].SetBoarder(FALSE);
	myBtn[23].SetBtnType(BTN_TYPE_CHECK);


	myBtn[53].SubclassDlgItem(IDC_CHK_53, this);		// �ǵ�\rŬ����
	myBtn[53].SetHwnd(this->GetSafeHwnd(), IDC_CHK_53);
	myBtn[53].SetBoarder(FALSE);
	myBtn[53].SetBtnType(BTN_TYPE_CHECK);


	myBtn[54].SubclassDlgItem(IDC_CHK_54, this);		// �ټ�\rŬ����
	myBtn[54].SetHwnd(this->GetSafeHwnd(), IDC_CHK_54);
	myBtn[54].SetBoarder(FALSE);
	myBtn[54].SetBtnType(BTN_TYPE_CHECK);


	myBtn[24].SubclassDlgItem(IDC_CHK_24, this);		// ���ǳ�\r�̼�
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_24);
	myBtn[24].SetBoarder(FALSE);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);


	myBtn[50].SubclassDlgItem(IDC_CHK_50, this);		// ������\r��ũ
	myBtn[50].SetHwnd(this->GetSafeHwnd(), IDC_CHK_50);
	myBtn[50].SetBoarder(FALSE);
	myBtn[50].SetBtnType(BTN_TYPE_CHECK);


	// Uncoiler
	myBtn[25].SubclassDlgItem(IDC_CHK_25, this);		// ��������
	myBtn[25].SetHwnd(this->GetSafeHwnd(), IDC_CHK_25);
	myBtn[25].SetBoarder(FALSE);
	myBtn[25].SetBtnType(BTN_TYPE_CHECK);


	myBtn[26].SubclassDlgItem(IDC_CHK_26, this);		// ����ȸ��
	myBtn[26].SetHwnd(this->GetSafeHwnd(), IDC_CHK_26);
	myBtn[26].SetBoarder(FALSE);
	myBtn[26].SetBtnType(BTN_TYPE_CHECK);


	myBtn[27].SubclassDlgItem(IDC_CHK_27, this);		// ��ȸ����
	myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_CHK_27);
	myBtn[27].SetBoarder(FALSE);
	myBtn[27].SetBtnType(BTN_TYPE_CHECK);


	myBtn[28].SubclassDlgItem(IDC_CHK_28, this);		// ��ǰ����\r���/�ϰ�
	myBtn[28].SetHwnd(this->GetSafeHwnd(), IDC_CHK_28);
	myBtn[28].SetBoarder(FALSE);
	myBtn[28].SetBtnType(BTN_TYPE_CHECK);


	myBtn[29].SubclassDlgItem(IDC_CHK_29, this);		// Ŭ���ѷ�\r���/�ϰ�
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_CHK_29);
	myBtn[29].SetBoarder(FALSE);
	myBtn[29].SetBtnType(BTN_TYPE_CHECK);


	myBtn[30].SubclassDlgItem(IDC_CHK_30, this);		// ��ǰ������(��)\r���/�ϰ�
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_CHK_30);
	myBtn[30].SetBoarder(FALSE);
	myBtn[30].SetBtnType(BTN_TYPE_CHECK);


	myBtn[31].SubclassDlgItem(IDC_CHK_31, this);		// ���� ô\rŬ����
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_CHK_31);
	myBtn[31].SetBoarder(FALSE);
	myBtn[31].SetBtnType(BTN_TYPE_CHECK);


	myBtn[32].SubclassDlgItem(IDC_CHK_32, this);		// ���� ��\r����ȸ��
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_CHK_32);
	myBtn[32].SetBoarder(FALSE);
	myBtn[32].SetBtnType(BTN_TYPE_CHECK);


	myBtn[35].SubclassDlgItem(IDC_CHK_35, this);		// ��ǰô\rŬ����
	myBtn[35].SetHwnd(this->GetSafeHwnd(), IDC_CHK_35);
	myBtn[35].SetBoarder(FALSE);
	myBtn[35].SetBtnType(BTN_TYPE_CHECK);


	myBtn[36].SubclassDlgItem(IDC_CHK_36, this);		// Ŭ���ѷ�����\r���/�ϰ�
	myBtn[36].SetHwnd(this->GetSafeHwnd(), IDC_CHK_36);
	myBtn[36].SetBoarder(FALSE);
	myBtn[36].SetBtnType(BTN_TYPE_CHECK);


	myBtn[37].SubclassDlgItem(IDC_CHK_37, this);		// ��ǰ������(��)\r���/�ϰ�
	myBtn[37].SetHwnd(this->GetSafeHwnd(), IDC_CHK_37);
	myBtn[37].SetBoarder(FALSE);	
	myBtn[37].SetBtnType(BTN_TYPE_CHECK);


	myBtn[38].SubclassDlgItem(IDC_CHK_38, this);		// ��ǰ������\r����
	myBtn[38].SetHwnd(this->GetSafeHwnd(), IDC_CHK_38);
	myBtn[38].SetBoarder(FALSE);
	myBtn[38].SetBtnType(BTN_TYPE_CHECK);


	myBtn[39].SubclassDlgItem(IDC_CHK_39, this);		// ���� ��\r��ȸ����
	myBtn[39].SetHwnd(this->GetSafeHwnd(), IDC_CHK_39);
	myBtn[39].SetBoarder(FALSE);
	myBtn[39].SetBtnType(BTN_TYPE_CHECK);


	myBtn[40].SubclassDlgItem(IDC_CHK_40, this);		// Uncoiler-����
	myBtn[40].SetHwnd(this->GetSafeHwnd(), IDC_CHK_40);
	myBtn[40].SetBoarder(FALSE);
	myBtn[40].SetBtnType(BTN_TYPE_CHECK);


	// AOI �ϸ�.
	myBtn[55].SubclassDlgItem(IDC_CHK_55, this);		// ��������
	myBtn[55].SetHwnd(this->GetSafeHwnd(), IDC_CHK_55);
	myBtn[55].SetBoarder(FALSE);
	myBtn[55].SetBtnType(BTN_TYPE_CHECK);


	myBtn[56].SubclassDlgItem(IDC_CHK_56, this);		// ����ȸ��
	myBtn[56].SetHwnd(this->GetSafeHwnd(), IDC_CHK_56);
	myBtn[56].SetBoarder(FALSE);
	myBtn[56].SetBtnType(BTN_TYPE_CHECK);


	myBtn[57].SubclassDlgItem(IDC_CHK_57, this);		// ��ȸ����
	myBtn[57].SetHwnd(this->GetSafeHwnd(), IDC_CHK_57);
	myBtn[57].SetBoarder(FALSE);
	myBtn[57].SetBtnType(BTN_TYPE_CHECK);


	myBtn[58].SubclassDlgItem(IDC_CHK_58, this);		// �ǵ�����
	myBtn[58].SetHwnd(this->GetSafeHwnd(), IDC_CHK_58);
	myBtn[58].SetBoarder(FALSE);
	myBtn[58].SetBtnType(BTN_TYPE_CHECK);


	myBtn[59].SubclassDlgItem(IDC_CHK_59, this);		// ��ǰǪ��
	myBtn[59].SetHwnd(this->GetSafeHwnd(), IDC_CHK_59);
	myBtn[59].SetBoarder(FALSE);
	myBtn[59].SetBtnType(BTN_TYPE_CHECK);


	myBtn[60].SubclassDlgItem(IDC_CHK_60, this);		// ���̺�\r��ο�
	myBtn[60].SetHwnd(this->GetSafeHwnd(), IDC_CHK_60);
	myBtn[60].SetBoarder(FALSE);
	myBtn[60].SetBtnType(BTN_TYPE_CHECK);


	myBtn[61].SubclassDlgItem(IDC_CHK_61, this);		// ���̺�\r����
	myBtn[61].SetHwnd(this->GetSafeHwnd(), IDC_CHK_61);
	myBtn[61].SetBoarder(FALSE);
	myBtn[61].SetBtnType(BTN_TYPE_CHECK);


	myBtn[62].SubclassDlgItem(IDC_CHK_64, this);		// �ǵ�\rŬ����
	myBtn[62].SetHwnd(this->GetSafeHwnd(), IDC_CHK_64);
	myBtn[62].SetBoarder(FALSE);
	myBtn[62].SetBtnType(BTN_TYPE_CHECK);


	myBtn[63].SubclassDlgItem(IDC_CHK_65, this);		// �ټ�\rŬ����
	myBtn[63].SetHwnd(this->GetSafeHwnd(), IDC_CHK_65);
	myBtn[63].SetBoarder(FALSE);
	myBtn[63].SetBtnType(BTN_TYPE_CHECK);


	myBtn[64].SubclassDlgItem(IDC_CHK_62, this);		// ���ǳ�\r�̼�
	myBtn[64].SetHwnd(this->GetSafeHwnd(), IDC_CHK_62);
	myBtn[64].SetBoarder(FALSE);
	myBtn[64].SetBtnType(BTN_TYPE_CHECK);


	myBtn[65].SubclassDlgItem(IDC_CHK_63, this);		// ������\r��ũ
	myBtn[65].SetHwnd(this->GetSafeHwnd(), IDC_CHK_63);
	myBtn[65].SetBoarder(FALSE);
	myBtn[65].SetBtnType(BTN_TYPE_CHECK);


	// Engraving
	myBtn[72].SubclassDlgItem(IDC_CHK_72, this);		// ��������
	myBtn[72].SetHwnd(this->GetSafeHwnd(), IDC_CHK_72);
	myBtn[72].SetBoarder(FALSE);
	myBtn[72].SetBtnType(BTN_TYPE_CHECK);


	myBtn[73].SubclassDlgItem(IDC_CHK_73, this);		// ����ȸ��
	myBtn[73].SetHwnd(this->GetSafeHwnd(), IDC_CHK_73);
	myBtn[73].SetBoarder(FALSE);
	myBtn[73].SetBtnType(BTN_TYPE_CHECK);


	myBtn[74].SubclassDlgItem(IDC_CHK_74, this);		// ��ȸ����
	myBtn[74].SetHwnd(this->GetSafeHwnd(), IDC_CHK_74);
	myBtn[74].SetBoarder(FALSE);
	myBtn[74].SetBtnType(BTN_TYPE_CHECK);


	myBtn[75].SubclassDlgItem(IDC_CHK_75, this);		// �ǵ�����
	myBtn[75].SetHwnd(this->GetSafeHwnd(), IDC_CHK_75);
	myBtn[75].SetBoarder(FALSE);
	myBtn[75].SetBtnType(BTN_TYPE_CHECK);


	myBtn[76].SubclassDlgItem(IDC_CHK_76, this);		// ��ǰǪ��
	myBtn[76].SetHwnd(this->GetSafeHwnd(), IDC_CHK_76);
	myBtn[76].SetBoarder(FALSE);
	myBtn[76].SetBtnType(BTN_TYPE_CHECK);


	myBtn[77].SubclassDlgItem(IDC_CHK_77, this);		// ���̺�\r��ο�
	myBtn[77].SetHwnd(this->GetSafeHwnd(), IDC_CHK_77);
	myBtn[77].SetBoarder(FALSE);
	myBtn[77].SetBtnType(BTN_TYPE_CHECK);


	myBtn[78].SubclassDlgItem(IDC_CHK_78, this);		// ���̺�\r����
	myBtn[78].SetHwnd(this->GetSafeHwnd(), IDC_CHK_78);
	myBtn[78].SetBoarder(FALSE);
	myBtn[78].SetBtnType(BTN_TYPE_CHECK);


	myBtn[79].SubclassDlgItem(IDC_CHK_82, this);		// �ǵ�\rŬ����
	myBtn[79].SetHwnd(this->GetSafeHwnd(), IDC_CHK_82);
	myBtn[79].SetBoarder(FALSE);
	myBtn[79].SetBtnType(BTN_TYPE_CHECK);


	myBtn[80].SubclassDlgItem(IDC_CHK_83, this);		// �ټ�\rŬ����
	myBtn[80].SetHwnd(this->GetSafeHwnd(), IDC_CHK_83);
	myBtn[80].SetBoarder(FALSE);
	myBtn[80].SetBtnType(BTN_TYPE_CHECK);


	myBtn[81].SubclassDlgItem(IDC_CHK_79, this);		// ���ǳ�\r�̼�
	myBtn[81].SetHwnd(this->GetSafeHwnd(), IDC_CHK_79);
	myBtn[81].SetBoarder(FALSE);
	myBtn[81].SetBtnType(BTN_TYPE_CHECK);


	myBtn[82].SubclassDlgItem(IDC_CHK_81, this);		// ������\r��ũ
	myBtn[82].SetHwnd(this->GetSafeHwnd(), IDC_CHK_81);
	myBtn[82].SetBoarder(FALSE);
	myBtn[82].SetBtnType(BTN_TYPE_CHECK);


	myBtn[84].SubclassDlgItem(IDC_CHK_80, this);		// ����\r���/�ϰ�
	myBtn[84].SetHwnd(this->GetSafeHwnd(), IDC_CHK_80);
	myBtn[84].SetBoarder(FALSE);
	myBtn[84].SetBtnType(BTN_TYPE_CHECK);


	int i;
	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		myBtn[i].SetFont(_T("����ü"), 12, TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
	}
}

void CDlgMenu03::InitLabel()
{
	myLabel[0].SubclassDlgItem(IDC_STC_SIG1, this);
	myLabel[1].SubclassDlgItem(IDC_STC_SIG2, this);
	myLabel[2].SubclassDlgItem(IDC_STC_SIG3, this);
	myLabel[3].SubclassDlgItem(IDC_STC_SIG4, this);
	myLabel[4].SubclassDlgItem(IDC_STC_SIG5, this);

	for (int i = 0; i < MAX_MENU03_LABEL; i++)
	{
		myLabel[i].SetFontName(_T("Arial"));
		myLabel[i].SetFontSize(18);
		myLabel[i].SetFontBold(TRUE);
		myLabel[i].SetTextColor(RGB_DARKRED);
		myLabel[i].SetImageBk(LBL_IMG_UP);
	}
}

void CDlgMenu03::InitStatic()
{
	// Main
	myStcTitle[0].SubclassDlgItem(IDC_STC_000_, this);		// Run
	myStcTitle[1].SubclassDlgItem(IDC_STC_01_, this);		// Reset
	myStcTitle[39].SubclassDlgItem(IDC_STC_000_3, this);	// Stop
	myStcTitle[40].SubclassDlgItem(IDC_STC_000_4, this);	// �����غ�

	myStcTitle[33].SubclassDlgItem(IDC_STC_02, this);		// LED-�ڵ� 
	myStcTitle[34].SubclassDlgItem(IDC_STC_03, this);		// LED-1ȸ����
	myStcTitle[35].SubclassDlgItem(IDC_STC_04, this);		// LED-���� 

	myStcTitle[36].SubclassDlgItem(IDC_STC_02_, this);		// �ڵ�
	myStcTitle[37].SubclassDlgItem(IDC_STC_03_, this);		// 1ȸ����
	myStcTitle[38].SubclassDlgItem(IDC_STC_04_, this);		// ����

	// TorqueMotor
	myStcTitle[2].SubclassDlgItem(IDC_STC_000_2, this);		// ��Ī��
	myStcTitle[3].SubclassDlgItem(IDC_STC_01_2, this);		// �˻��
	myStcTitle[78].SubclassDlgItem(IDC_STC_000_9, this);	// ���κ�

	// InductionMotor
	myStcTitle[74].SubclassDlgItem(IDC_STC_000_6, this);	// Recoiler������
	myStcTitle[75].SubclassDlgItem(IDC_STC_01_3, this);		// Uncoiler������

	// Core150mm
	myStcTitle[76].SubclassDlgItem(IDC_STC_000_7, this);	// Recoiler
	myStcTitle[77].SubclassDlgItem(IDC_STC_01_4, this);		// Uncoiler

	// Recoiler
	myStcTitle[4].SubclassDlgItem(IDC_STC_10_, this);		// ��������
	myStcTitle[5].SubclassDlgItem(IDC_STC_12_2, this);		// ��ȸ��
	myStcTitle[6].SubclassDlgItem(IDC_STC_12_, this);		// ��ȸ��
	myStcTitle[7].SubclassDlgItem(IDC_STC_12_4, this);		// ��ǰ������(��)���
	myStcTitle[8].SubclassDlgItem(IDC_STC_12_3, this);		// ��ǰ����������
	myStcTitle[47].SubclassDlgItem(IDC_STC_12_32, this);	// ��ǰôŬ����
	myStcTitle[48].SubclassDlgItem(IDC_STC_12_33, this);	// ��ǰ���ѻ��
	myStcTitle[49].SubclassDlgItem(IDC_STC_12_34, this);	// ��ǰ������(��)���
	myStcTitle[50].SubclassDlgItem(IDC_STC_12_36, this);	// ����ôŬ����
	myStcTitle[51].SubclassDlgItem(IDC_STC_12_35, this);	// ��������ȸ��
	myStcTitle[52].SubclassDlgItem(IDC_STC_12_37, this);	// �����ٿ�ȸ��
	myStcTitle[53].SubclassDlgItem(IDC_STC_12_38, this);	// ����
	myStcTitle[72].SubclassDlgItem(IDC_STC_12_55, this);	// Rewinder����
	myStcTitle[73].SubclassDlgItem(IDC_STC_12_56, this);	// Rewinder��ǰ����

	// Punching
	myStcTitle[9].SubclassDlgItem(IDC_STC_20_, this);		// ��������
	myStcTitle[10].SubclassDlgItem(IDC_STC_12_6, this);		// ��ȸ��
	myStcTitle[11].SubclassDlgItem(IDC_STC_12_5, this);		// ��ȸ��
	myStcTitle[12].SubclassDlgItem(IDC_STC_12_8, this);		// �ǵ�����
	myStcTitle[13].SubclassDlgItem(IDC_STC_12_7, this);		// ��ǰǪ��
	myStcTitle[14].SubclassDlgItem(IDC_STC_12_10, this);	// ���̺��ο�
	myStcTitle[15].SubclassDlgItem(IDC_STC_12_9, this);		// ���̺�����
	myStcTitle[16].SubclassDlgItem(IDC_STC_12_12, this);	// ���ǳ��̼�
	myStcTitle[54].SubclassDlgItem(IDC_STC_000_5, this);	// ���ѻ��
	myStcTitle[55].SubclassDlgItem(IDC_STC_12_39, this);	// ��������ũ
	myStcTitle[57].SubclassDlgItem(IDC_STC_12_42, this);	// �ǵ�Ŭ����
	myStcTitle[58].SubclassDlgItem(IDC_STC_12_41, this);	// �ټ�Ŭ����

	// AOI��
	myStcTitle[61].SubclassDlgItem(IDC_STC_30_2, this);		// ��������
	myStcTitle[62].SubclassDlgItem(IDC_STC_12_46, this);	// ��ȸ��
	myStcTitle[63].SubclassDlgItem(IDC_STC_12_45, this);	// ��ȸ��
	myStcTitle[64].SubclassDlgItem(IDC_STC_12_48, this);	// �ǵ�����
	myStcTitle[65].SubclassDlgItem(IDC_STC_12_47, this);	// ��ǰǪ��
	myStcTitle[66].SubclassDlgItem(IDC_STC_12_50, this);	// ���̺��ο�
	myStcTitle[67].SubclassDlgItem(IDC_STC_12_49, this);	// ���̺�����
	myStcTitle[68].SubclassDlgItem(IDC_STC_12_54, this);	// �ǵ�Ŭ����
	myStcTitle[69].SubclassDlgItem(IDC_STC_12_53, this);	// �ټ�Ŭ����
	myStcTitle[70].SubclassDlgItem(IDC_STC_12_51, this);	// ���ǳ��̼�
	myStcTitle[71].SubclassDlgItem(IDC_STC_12_52, this);	// ��������ũ

	// AOI��
	myStcTitle[17].SubclassDlgItem(IDC_STC_30_, this);		// ��������
	myStcTitle[18].SubclassDlgItem(IDC_STC_12_13, this);	// ��ȸ��
	myStcTitle[19].SubclassDlgItem(IDC_STC_12_11, this);	// ��ȸ��
	myStcTitle[20].SubclassDlgItem(IDC_STC_12_15, this);	// �ǵ�����
	myStcTitle[21].SubclassDlgItem(IDC_STC_12_14, this);	// ��ǰǪ��
	myStcTitle[22].SubclassDlgItem(IDC_STC_12_17, this);	// ���̺��ο�
	myStcTitle[23].SubclassDlgItem(IDC_STC_12_16, this);	// ���̺�����
	myStcTitle[24].SubclassDlgItem(IDC_STC_12_19, this);	// ���ǳ��̼�
	myStcTitle[56].SubclassDlgItem(IDC_STC_12_40, this);	// ��������ũ
	myStcTitle[59].SubclassDlgItem(IDC_STC_12_44, this);	// �ǵ�Ŭ����
	myStcTitle[60].SubclassDlgItem(IDC_STC_12_43, this);	// �ټ�Ŭ����

	// Uncoiler
	myStcTitle[25].SubclassDlgItem(IDC_STC_40_, this);		// ��������
	myStcTitle[26].SubclassDlgItem(IDC_STC_12_20, this);	// ��ȸ��
	myStcTitle[27].SubclassDlgItem(IDC_STC_12_18, this);	// ��ȸ��
	myStcTitle[28].SubclassDlgItem(IDC_STC_12_22, this);	// ��ǰ���ѻ��
	myStcTitle[29].SubclassDlgItem(IDC_STC_12_21, this);	// Ŭ���ѷ����
	myStcTitle[30].SubclassDlgItem(IDC_STC_12_24, this);	// ��ǰ������(��)���
	myStcTitle[31].SubclassDlgItem(IDC_STC_12_25, this);	// ����ôŬ����
	myStcTitle[32].SubclassDlgItem(IDC_STC_12_23, this);	// ��������ȸ��
	myStcTitle[41].SubclassDlgItem(IDC_STC_12_26, this);	// ��ǰôŬ����
	myStcTitle[42].SubclassDlgItem(IDC_STC_12_27, this);	// Ŭ���ѷ��������
	myStcTitle[43].SubclassDlgItem(IDC_STC_12_28, this);	// ��ǰ������(��)���
	myStcTitle[44].SubclassDlgItem(IDC_STC_12_29, this);	// ��ǰ����������
	myStcTitle[45].SubclassDlgItem(IDC_STC_12_30, this);	// �����ٿ�ȸ��
	myStcTitle[46].SubclassDlgItem(IDC_STC_12_31, this);	// ����

	// Engraving
	myStcTitle[79].SubclassDlgItem(IDC_STC_20_2, this);		// ��������
	myStcTitle[80].SubclassDlgItem(IDC_STC_12_58, this);	// ��ȸ��
	myStcTitle[81].SubclassDlgItem(IDC_STC_12_57, this);	// ��ȸ��
	myStcTitle[82].SubclassDlgItem(IDC_STC_12_60, this);	// �ǵ�����
	myStcTitle[83].SubclassDlgItem(IDC_STC_12_59, this);	// ��ǰǪ��
	myStcTitle[84].SubclassDlgItem(IDC_STC_12_62, this);	// ���̺��ο�
	myStcTitle[85].SubclassDlgItem(IDC_STC_12_61, this);	// ���̺�����
	myStcTitle[86].SubclassDlgItem(IDC_STC_12_63, this);	// ���ǳ��̼�
	myStcTitle[87].SubclassDlgItem(IDC_STC_000_8, this);	// ���ѻ��
	myStcTitle[88].SubclassDlgItem(IDC_STC_12_64, this);	// ��������ũ
	myStcTitle[89].SubclassDlgItem(IDC_STC_12_66, this);	// �ǵ�Ŭ����
	myStcTitle[90].SubclassDlgItem(IDC_STC_12_65, this);	// �ټ�Ŭ����

	// ������
	myStcTitle[91].SubclassDlgItem(IDC_STC_000_10, this);	// ���κ� ������
	myStcTitle[92].SubclassDlgItem(IDC_STC_000_11, this);	// AOI(��) ������

	for (int i = 0; i < MAX_MENU03_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu03::Disp()
{
	DispMain();
	DispRecoiler();
	DispPunch();
	DispAoiDn();
	DispAoiUp();
	DispEngrave();
	DispUncoiler();
}

void CDlgMenu03::DispMain()
{
	BOOL bOn;

	// [Main]
	bOn = pDoc->BtnStatus.Main.Ready;	// ��ŷ�� �����غ� ����ġ ����
	if (myBtn[34].GetCheck() != bOn)
		myBtn[34].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Run;		// ��ŷ�� ���� ����ġ ����
	if (myBtn[0].GetCheck() != bOn)
		myBtn[0].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Reset;	// ��ŷ�� ���� ����ġ ����
	if (myBtn[1].GetCheck() != bOn)
		myBtn[1].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Stop;	// ��ŷ�� ���� ����ġ ����
	if (myBtn[33].GetCheck() != bOn)
		myBtn[33].SetCheck(bOn);

	pDoc->Status.bAuto = pDoc->BtnStatus.Main.Auto;
	pDoc->Status.bManual = pDoc->BtnStatus.Main.Manual;
	if (pDoc->Status.bAuto)	// Auto
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_DN)
			myStcTitle[33].SetImageBk(LBL_IMG_DN);	// �� ��
		if (myStcTitle[34].GetImageBk() != LBL_IMG_UP)
			myStcTitle[34].SetImageBk(LBL_IMG_UP);	// 1ȸ ����
		if (myStcTitle[35].GetImageBk() != LBL_IMG_UP)
			myStcTitle[35].SetImageBk(LBL_IMG_UP);	// �� �� 
	}
	else if (pDoc->Status.bManual)	// Manual
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_UP)
			myStcTitle[33].SetImageBk(LBL_IMG_UP);	// �� ��
		if (myStcTitle[34].GetImageBk() != LBL_IMG_UP)
			myStcTitle[34].SetImageBk(LBL_IMG_UP);	// 1ȸ ����
		if (myStcTitle[35].GetImageBk() != LBL_IMG_DN)
			myStcTitle[35].SetImageBk(LBL_IMG_DN);	// �� �� 
	}
	else if (pDoc->Status.bOneCycle)	// OneCycl
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_UP)
			myStcTitle[33].SetImageBk(LBL_IMG_UP);	// �� ��
		if (myStcTitle[34].GetImageBk() != LBL_IMG_DN)
			myStcTitle[34].SetImageBk(LBL_IMG_DN);	// 1ȸ ����
		if (myStcTitle[35].GetImageBk() != LBL_IMG_UP)
			myStcTitle[35].SetImageBk(LBL_IMG_UP);	// �� �� 
	}
	else
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_UP)
			myStcTitle[33].SetImageBk(LBL_IMG_UP);	// �� ��
		if (myStcTitle[34].GetImageBk() != LBL_IMG_UP)
			myStcTitle[34].SetImageBk(LBL_IMG_UP);	// 1ȸ ����
		if (myStcTitle[35].GetImageBk() != LBL_IMG_UP)
			myStcTitle[35].SetImageBk(LBL_IMG_UP);	// �� �� 
	}

	// [Torque Motor]
	bOn = pDoc->WorkingInfo.Motion.bMkTq = pDoc->BtnStatus.Tq.Mk;
	if (myBtn[2].GetImageBk() != bOn)
		myBtn[2].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.Motion.bAoiTq = pDoc->BtnStatus.Tq.Aoi;
	if (myBtn[3].GetImageBk() != bOn)
		myBtn[3].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.Motion.bEngraveTq = pDoc->BtnStatus.Tq.Eng;
	if (myBtn[83].GetImageBk() != bOn)
		myBtn[83].SetCheck(bOn);

	// [InductionMotor]
	CString sCap;
	bOn = pDoc->WorkingInfo.LastJob.bOneMetal = pDoc->BtnStatus.Induct.Rc;	// [One Metal]
	if (bOn)
	{
		sCap = myStcTitle[74].GetText();
		if (sCap != _T("Recoiler\r������ CCW"))
		{
			myStcTitle[74].ShowWindow(SW_HIDE);
			myStcTitle[74].SetTextColor(RGB_RED);
			myStcTitle[74].SetText(_T("Recoiler\r������ CCW"));
			myStcTitle[74].ShowWindow(SW_SHOW);
		}
	}
	else
	{
		sCap = myStcTitle[74].GetText();
		if (sCap != _T("Recoiler\r������ CW"))
		{
			myStcTitle[74].ShowWindow(SW_HIDE);
			myStcTitle[74].SetTextColor(RGB_NAVY);
			myStcTitle[74].SetText(_T("Recoiler\r������ CW"));
			myStcTitle[74].ShowWindow(SW_SHOW);
		}
	}

	if (myBtn[68].GetImageBk() != bOn) // Induction Motor Recoiler\r������ ON
	{
		myBtn[68].SetCheck(bOn);
	}

	bOn = pDoc->WorkingInfo.LastJob.bTwoMetal = pDoc->BtnStatus.Induct.Uc;	// [Two Metal]
	if (bOn)
	{
		sCap = myStcTitle[75].GetText();
		if (sCap != _T("Uncoiler\r������"))
		{
			myStcTitle[75].ShowWindow(SW_HIDE);
			myStcTitle[75].SetTextColor(RGB_RED);
			myStcTitle[75].SetText(_T("Uncoiler\r������"));
			myStcTitle[75].ShowWindow(SW_SHOW);
		}
	}
	else
	{
		sCap = myStcTitle[75].GetText();
		if (sCap != _T("Uncoiler\r������"))
		{
			myStcTitle[75].ShowWindow(SW_HIDE);
			myStcTitle[75].SetTextColor(RGB_NAVY);
			myStcTitle[75].SetText(_T("Uncoiler\r������"));
			myStcTitle[75].ShowWindow(SW_SHOW);
		}
	}

	if (myBtn[69].GetImageBk() != bOn) // Induction Motor Uncoiler\r������ ON
	{
		myBtn[69].SetCheck(bOn);
	}

	// [Core 150mm]
	bOn = pDoc->WorkingInfo.LastJob.bCore150Recoiler = pDoc->BtnStatus.Core150.Rc;
	if (myBtn[70].GetImageBk() != bOn)
		myBtn[70].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bCore150Uncoiler = pDoc->BtnStatus.Core150.Uc;
	if (myBtn[71].GetImageBk() != bOn)
		myBtn[71].SetCheck(bOn);
}

void CDlgMenu03::DispRecoiler()
{
	BOOL bOn;

	// [Recoiler]
	bOn = pDoc->BtnStatus.Rc.Relation;	// �����Ϸ� ���� ��/���� ����ġ ����
	if (myBtn[4].GetImageBk() != bOn)
		myBtn[4].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.FdCw;	// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ ����
	if (myBtn[5].GetImageBk() != bOn)
		myBtn[5].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.FdCcw;	// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ ����
	if (myBtn[6].GetImageBk() != bOn)
		myBtn[6].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ReelChuck;	// �����Ϸ� ��ǰô Ŭ���� ����ġ ����
	if (myBtn[41].GetImageBk() != bOn)
		myBtn[41].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.DcRlUpDn;	// �����Ϸ� ���� ���/�ϰ� ����ġ ����
	if (myBtn[42].GetImageBk() != bOn)
		myBtn[42].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ReelJoinL;	// �����Ϸ� ��ǰ ������(��) ����ġ ����
	if (myBtn[43].GetImageBk() != bOn)
		myBtn[43].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ReelJoinR;	// �����Ϸ� ��ǰ ������(��) ����ġ ����
	if (myBtn[7].GetImageBk() != bOn)
		myBtn[7].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ReelJoinVac;	// �����Ϸ� ��ǰ ������ ���� ����ġ ����
	if (myBtn[8].GetImageBk() != bOn)
		myBtn[8].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PprChuck;	// �����Ϸ� ����ô Ŭ���� ����ġ ����
	if (myBtn[44].GetImageBk() != bOn)
		myBtn[44].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PprCw;	// �����Ϸ� ������ ��ȸ�� ����ġ ����
	if (myBtn[45].GetImageBk() != bOn)
		myBtn[45].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PprCcw;	// �����Ϸ� ������ ��ȸ�� ����ġ ����
	if (myBtn[46].GetImageBk() != bOn)
		myBtn[46].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.Rewine;	// �����Ϸ� Rewinder ���� ����ġ ����
	if (myBtn[66].GetImageBk() != bOn)
		myBtn[66].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.RewineReelPpr;	// �����Ϸ� Rewinder ��ǰ & ���� ����ġ ����
	if (myBtn[67].GetImageBk() != bOn)
		myBtn[67].SetCheck(bOn);
}

void CDlgMenu03::DispPunch()
{
	BOOL bOn;

	// [Marking]
	bOn = pDoc->BtnStatus.Mk.Relation;	// ��ŷ�� ���� ��/���� ����ġ ����
	if (myBtn[9].GetImageBk() != bOn)
		myBtn[9].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FdCw;	// ��ŷ�� �ǵ� ��ȸ�� ����ġ ����
	if (myBtn[10].GetImageBk() != bOn)
		myBtn[10].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FdCcw;	// ��ŷ�� �ǵ� ��ȸ�� ����ġ ����
	if (myBtn[11].GetImageBk() != bOn)
		myBtn[11].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FdVac;	// ��ŷ�� �ǵ� ���� ����ġ ����
	if (myBtn[12].GetImageBk() != bOn)
		myBtn[12].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.PushUp;	// ��ŷ�� ��ǰǪ�� ����ġ ���� // ��ŷ�� ��ũ ���� ����ġ ���� - X
	if (myBtn[13].GetImageBk() != bOn)
		myBtn[13].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TblBlw;	// ��ŷ�� ���̺� ��ο� ����ġ ����
	if (myBtn[14].GetImageBk() != bOn)
		myBtn[14].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TblVac;	// ��ŷ�� ���̺� ���� ����ġ ����
	if (myBtn[15].GetImageBk() != bOn)
		myBtn[15].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FdClp;	// ��ŷ�� �ǵ� Ŭ���� ����ġ ����
	if (myBtn[51].GetImageBk() != bOn)
		myBtn[51].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TqClp;	// ��ŷ�� ��ũ Ŭ���� ����ġ ����
	if (myBtn[52].GetImageBk() != bOn)
		myBtn[52].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bMkOnePnl = pDoc->BtnStatus.Mk.MvOne; // ��ŷ�� ���ǳ� �̼�
	if (myBtn[16].GetImageBk() != bOn)
		myBtn[16].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.LsrPt;	// ��ŷ�� ������ ������ ����ġ ����
	if (myBtn[49].GetImageBk() != bOn)
		myBtn[49].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.DcRSol;	// ��ŷ�� ���� ���/�ϰ� ����ġ ����
	if (myBtn[48].GetCheck() != bOn)
		myBtn[48].SetCheck(bOn);
}

void CDlgMenu03::DispAoiDn()
{
	BOOL bOn;

	// [�ϸ� AOI]
	bOn = pDoc->BtnStatus.AoiDn.Relation;	// �˻�� �� ���� ��/���� ����ġ ����
	if (myBtn[55].GetImageBk() != bOn)
		myBtn[55].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FdCw;	// �˻�� �� �ǵ� ��ȸ�� ����ġ ����
	if (myBtn[56].GetImageBk() != bOn)
		myBtn[56].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FdCcw;	// �˻�� �� �ǵ� ��ȸ�� ����ġ ����
	if (myBtn[57].GetImageBk() != bOn)
		myBtn[57].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FdVac;	// �˻�� �� �ǵ� ���� ����ġ ����
	if (myBtn[58].GetImageBk() != bOn)
		myBtn[58].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.PushUp;	// �˻�� �� ��ǰǪ�� ����ġ ���� // (��ũ ���� ����ġ ����) - X
	if (myBtn[59].GetImageBk() != bOn)
		myBtn[59].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TblBlw;	// �˻�� �� ���̺� ��ο� ����ġ ����
	if (myBtn[60].GetImageBk() != bOn)
		myBtn[60].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TblVac;	// �˻�� �� ���̺� ���� ����ġ ����
	if (myBtn[61].GetImageBk() != bOn)
		myBtn[61].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FdClp;	// �˻�� �� �ǵ� Ŭ���� ����ġ ����
	if (myBtn[62].GetImageBk() != bOn)
		myBtn[62].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TqClp;	// �˻�� �� �ټ� Ŭ���� ����ġ ���� 
	if (myBtn[63].GetImageBk() != bOn)
		myBtn[63].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl = pDoc->BtnStatus.AoiDn.MvOne; // �˻�� �� ���ǳ� �̼�
	if (myBtn[64].GetImageBk() != bOn)
		myBtn[64].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.LsrPt;	// �˻�� �� ������ ������ ����ġ ����
	if (myBtn[65].GetImageBk() != bOn)
		myBtn[65].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.VelSonicBlw;	// �˻�� �� ������ ������ ����ġ ����
	if (myBtn[86].GetImageBk() != bOn)
		myBtn[86].SetCheck(bOn);
}

void CDlgMenu03::DispAoiUp()
{
	BOOL bOn;

	// [��� AOI]
	bOn = pDoc->BtnStatus.AoiUp.Relation;	// �˻�� �� ���� ��/���� ����ġ ����
	if (myBtn[17].GetImageBk() != bOn)
		myBtn[17].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FdCw;	// �˻�� �� �ǵ� ��ȸ�� ����ġ ����
	if (myBtn[18].GetImageBk() != bOn)
		myBtn[18].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FdCcw;	// �˻�� �� �ǵ� ��ȸ�� ����ġ ����
	if (myBtn[19].GetImageBk() != bOn)
		myBtn[19].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FdVac;	// �˻�� �� �ǵ� ���� ����ġ ����
	if (myBtn[20].GetImageBk() != bOn)
		myBtn[20].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.PushUp;	// �˻�� �� ��ǰǪ�� ����ġ ���� // (��ũ ���� ����ġ ����) - X
	if (myBtn[21].GetImageBk() != bOn)
		myBtn[21].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TblBlw;	// �˻�� �� ���̺� ��ο� ����ġ ����
	if (myBtn[22].GetImageBk() != bOn)
		myBtn[22].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TblVac;	// �˻�� �� ���̺� ���� ����ġ ����
	if (myBtn[23].GetImageBk() != bOn)
		myBtn[23].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FdClp;	// �˻�� �� �ǵ� Ŭ���� ����ġ ����
	if (myBtn[53].GetImageBk() != bOn)
		myBtn[53].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TqClp;	// �˻�� �� �ټ� Ŭ���� ����ġ ���� 
	if (myBtn[54].GetImageBk() != bOn)
		myBtn[54].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl = pDoc->BtnStatus.AoiUp.MvOne; // �˻�� �� ���ǳ� �̼�
	if (myBtn[24].GetImageBk() != bOn)
		myBtn[24].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.LsrPt;	// �˻�� �� ������ ������ ����ġ ����
	if (myBtn[50].GetImageBk() != bOn)
		myBtn[50].SetCheck(bOn);
}

void CDlgMenu03::DispEngrave()
{
	BOOL bOn;

	bOn = pDoc->BtnStatus.Eng.Relation;	// ���κ� ���� ��/���� ����ġ ���� IDC_CHK_72
	if (myBtn[72].GetImageBk() != bOn)
		myBtn[72].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FdCw;	// ���κ� �ǵ� ��ȸ�� ����ġ ���� IDC_CHK_73
	if (myBtn[73].GetImageBk() != bOn)
		myBtn[73].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FdCcw;	// ���κ� �ǵ� ��ȸ�� ����ġ ���� IDC_CHK_74
	if (myBtn[74].GetImageBk() != bOn)
		myBtn[74].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FdVac;	// ���κ� �ǵ� ���� ����ġ ���� IDC_CHK_75
	if (myBtn[75].GetImageBk() != bOn)
		myBtn[75].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.PushUp;	// ���κ� ��ǰǪ�� ����ġ ���� IDC_CHK_76 // ��ŷ�� ��ũ ���� ����ġ ���� - X
	if (myBtn[76].GetImageBk() != bOn)
		myBtn[76].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TblBlw;	// ���κ� ���̺� ��ο� ����ġ ���� IDC_CHK_77
	if (myBtn[77].GetImageBk() != bOn)
		myBtn[77].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TblVac;	// ���κ� ���̺� ���� ����ġ ���� IDC_CHK_78
	if (myBtn[78].GetImageBk() != bOn)
		myBtn[78].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FdClp;	// ���κ� �ǵ� Ŭ���� ����ġ ���� IDC_CHK_82
	if (myBtn[79].GetImageBk() != bOn)
		myBtn[79].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TqClp;	// ���κ� ��ũ Ŭ���� ����ġ ���� IDC_CHK_83
	if (myBtn[80].GetImageBk() != bOn)
		myBtn[80].SetCheck(bOn);

	bOn = pDoc->WorkingInfo.LastJob.bEngraveOnePnl = pDoc->BtnStatus.Eng.MvOne; // ���κ� ���ǳ� �̼� IDC_CHK_79
	if (myBtn[81].GetImageBk() != bOn)
		myBtn[81].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.LsrPt;	// ���κ� ������ ������ ����ġ ���� IDC_CHK_81
	if (myBtn[82].GetImageBk() != bOn)
		myBtn[82].SetCheck(bOn);


	bOn = pDoc->BtnStatus.Eng.VelSonicBlw;	// ���κ� ������ ������ ����ġ ���� IDC_CHK_87
	if (myBtn[85].GetImageBk() != bOn)
		myBtn[85].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.DcRSol;	// ���κ� ���� ���/�ϰ� ����ġ ���� IDC_CHK_80
	if (myBtn[84].GetImageBk() != bOn)
		myBtn[84].SetCheck(bOn);
}

void CDlgMenu03::DispUncoiler()
{
	BOOL bOn;

	// [Uncoiler]
	bOn = pDoc->BtnStatus.Uc.Relation;		//[24] �����Ϸ� ���� ��/���� ����ġ ����
	if (myBtn[25].GetImageBk() != bOn)
		myBtn[25].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.FdCw;		// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ ����
	if (myBtn[26].GetImageBk() != bOn)
		myBtn[26].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.FdCcw;		// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ ����
	if (myBtn[27].GetImageBk() != bOn)
		myBtn[27].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ReelChuck;		// �����Ϸ� ��ǰô Ŭ���� ����ġ ����
	if (myBtn[35].GetImageBk() != bOn)
		myBtn[35].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.DcRlUpDn;		// �����Ϸ� ���� ���/�ϰ� ����ġ ����
	if (myBtn[28].GetImageBk() != bOn)
		myBtn[28].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ReelJoinL;		// �����Ϸ� ��ǰ ������(��) ����ġ ����
	if (myBtn[30].GetImageBk() != bOn)
		myBtn[30].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ReelJoinR;		// �����Ϸ� ��ǰ ������(��) ����ġ ����
	if (myBtn[37].GetImageBk() != bOn)
		myBtn[37].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ReelJoinVac;		// �����Ϸ� ��ǰ ������ ���� ����ġ ����
	if (myBtn[38].GetImageBk() != bOn)
		myBtn[38].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PprChuck;		// �����Ϸ� ����ô Ŭ���� ����ġ ����
	if (myBtn[31].GetImageBk() != bOn)
		myBtn[31].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PprCw;		// �����Ϸ� ������ ��ȸ�� ����ġ ����
	if (myBtn[32].GetImageBk() != bOn)
		myBtn[32].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PprCcw;		// �����Ϸ� ������ ��ȸ�� ����ġ ����
	if (myBtn[39].GetImageBk() != bOn)
		myBtn[39].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ClRlUpDn;		// �����Ϸ� Ŭ���ѷ� ���/�ϰ� ����ġ ����
	if (myBtn[29].GetImageBk() != bOn)
		myBtn[29].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ClRlPshUpDn;		// �����Ϸ� Ŭ���ѷ����� ���/�ϰ� ����ġ ����
	if (myBtn[36].GetImageBk() != bOn)
		myBtn[36].SetCheck(bOn);
}

void CDlgMenu03::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	CString strMsg, strTitle;

	if (nIDEvent == TIM_MENU03_DISP)
	{
		KillTimer(TIM_MENU03_DISP);
		UpdateSignal();
		if (m_bTIM_MENU03_DISP)
			SetTimer(TIM_MENU03_DISP, 300, NULL);
	}

	if(nIDEvent == TIM_CHK_DONE_MKAOI)
	{
		KillTimer(TIM_CHK_DONE_MKAOI);

		if(!(pDoc->m_pMpeSignal[5] & (0x01<<1)) && !(pDoc->m_pMpeSignal[5] & (0x01<<0)))	// ��ŷ�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF) && �˻�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
		{
			m_bTIM_CHK_DONE_MKAOI = FALSE;
		}
		if(m_bTIM_CHK_DONE_MKAOI)
			SetTimer(TIM_CHK_DONE_MKAOI, 100, NULL);
	}
	if(nIDEvent == TIM_CHK_DONE_MK)
	{
		KillTimer(TIM_CHK_DONE_MK);

		if(!(pDoc->m_pMpeSignal[5] & (0x01<<1)))	// ��ŷ�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
			m_bTIM_CHK_DONE_MK = FALSE;
		if(m_bTIM_CHK_DONE_MK)
			SetTimer(TIM_CHK_DONE_MK, 100, NULL);
	}
	if(nIDEvent == TIM_CHK_DONE_AOI)
	{
		KillTimer(TIM_CHK_DONE_AOI);

		if(!(pDoc->m_pMpeSignal[5] & (0x01<<0)))	// �˻�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
			m_bTIM_CHK_DONE_AOI = FALSE;
		if(m_bTIM_CHK_DONE_AOI)
			SetTimer(TIM_CHK_DONE_AOI, 100, NULL);
	}

	if(nIDEvent == TIM_CHK_MREG)
	{
		KillTimer(TIM_CHK_MREG);
		pView->ChkMRegOut();
	}
	
	if(nIDEvent == TIM_CHK_DONE_BUF_HOME)
	{
		KillTimer(TIM_CHK_DONE_BUF_HOME);

		if(pDoc->m_pMpeSignal[4] & (0x01<<2))	// ��ŷ�� ���۷ѷ� Ȩ���� ON (PLC�� Ȩ���� �Ϸ� �� OFF)
		{
			pView->GetDispMsg(strMsg, strTitle);
			if(strMsg != _T("Searching Buffer Home Position...") || strTitle != _T("Homming"))
				pView->DispMsg(_T("Searching Buffer Home Position..."), _T("Homming"), RGB_GREEN, DELAY_TIME_MSG);
		}
		else
		{
			m_bTIM_CHK_DONE_BUF_HOME = FALSE;
			pView->m_bBufHomeDone = TRUE;
			pView->ClrDispMsg();
		}
		if(m_bTIM_CHK_DONE_BUF_HOME)
			SetTimer(TIM_CHK_DONE_BUF_HOME, 100, NULL);
	}
	
	if(nIDEvent == TIM_CHK_DONE_BUF_INIT)
	{
		KillTimer(TIM_CHK_DONE_BUF_INIT);

		if(pDoc->m_pMpeSignal[4] & (0x01<<10))	// ��ŷ�� ���� �ʱ���ġ �̵�(PC�� ON, PLC�� OFF)
		{
			pView->GetDispMsg(strMsg, strTitle);
			if(strMsg != _T("Searching Buffer Initial Position...") || strTitle != _T("Moving"))
				pView->DispMsg(_T("Searching Buffer Initial Position..."), _T("Moving"), RGB_GREEN, DELAY_TIME_MSG);
		}
		else
		{
			m_bTIM_CHK_DONE_BUF_INIT = FALSE;
			pView->ClrDispMsg();
		}
		if(m_bTIM_CHK_DONE_BUF_INIT)
			SetTimer(TIM_CHK_DONE_BUF_INIT, 100, NULL);
	}
	
	if(nIDEvent == TIM_CHK_DONE_READY)
	{
		KillTimer(TIM_CHK_DONE_READY);

		if(pDoc->m_pMpeSignal[0] & (0x01<<0))	// PLC �����غ� �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
		{
			m_bTIM_CHK_DONE_READY = FALSE;
			pView->m_bReadyDone = TRUE; pDoc->SetStatus(_T("General"), _T("bReadyDone"), pView->m_bReadyDone);
			pView->MpeWrite(_T("MB440100"), 0);	// PLC �����غ� �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
		}
		else
		{
			pView->GetDispMsg(strMsg, strTitle);
			if(strMsg != _T("Searching Buffer Home Position...") || strTitle != _T("Homming"))
			{
				pView->m_bReadyDone = FALSE; pDoc->SetStatus(_T("General"), _T("bReadyDone"), pView->m_bReadyDone);
			}
		}
		if(m_bTIM_CHK_DONE_READY)
			SetTimer(TIM_CHK_DONE_READY, 100, NULL);
	}

	if(nIDEvent == TIM_AOI_RESET_OFF)
	{
		KillTimer(TIM_AOI_RESET_OFF);
		SwAoiReset(FALSE);
	}

	if(nIDEvent == TIM_AOI_LOTEND_OFF)
	{
		KillTimer(TIM_AOI_LOTEND_OFF);
	}

	if (nIDEvent == TIM_CHK_DONE_ENGRAVE)
	{
		KillTimer(TIM_CHK_DONE_ENGRAVE);

		if (!(pDoc->m_pMpeSignal[6] & (0x01 << 12)))	// ���κ� �ǵ� CW ON (PLC�� �ǵ��Ϸ� �� OFF) - MB44017C
			m_bTIM_CHK_DONE_ENGRAVE = FALSE;
		if (m_bTIM_CHK_DONE_ENGRAVE)
			SetTimer(TIM_CHK_DONE_ENGRAVE, 100, NULL);
	}



	if (nIDEvent == TIM_CHK_DONE_ENG_BUF_HOME)
	{
		KillTimer(TIM_CHK_DONE_ENG_BUF_HOME);

		if (pDoc->m_pMpeSignal[6] & (0x01 << 2))	// ���κ� ���۷ѷ� Ȩ���� ON (PLC�� Ȩ���� �Ϸ� �� OFF)
		{
			pView->GetDispMsg(strMsg, strTitle);
			if (strMsg != _T("Searching Engrave Buffer Home Position...") || strTitle != _T("Homming"))
				pView->DispMsg(_T("Searching Engrave Buffer Home Position..."), _T("Homming"), RGB_GREEN, DELAY_TIME_MSG);
		}
		else
		{
			m_bTIM_CHK_DONE_ENG_BUF_HOME = FALSE;
			pView->m_bEngBufHomeDone = TRUE;
			pView->ClrDispMsg();

#ifdef USE_ENGRAVE
			if (pView && pView->m_pEngrave)
			{
				pDoc->WorkingInfo.Motion.bEngBuffHommingDone = TRUE;
				pView->m_pEngrave->SetEngBuffHomeDone();	//_SigInx::_EngBuffOrgMvDone // ���κ� ���۷ѷ� Ȩ���� ON (PLC�� Ȩ���� �Ϸ� �� OFF)
			}
#endif
		}
		if (m_bTIM_CHK_DONE_ENG_BUF_HOME)
			SetTimer(TIM_CHK_DONE_ENG_BUF_HOME, 100, NULL);
	}

	if (nIDEvent == TIM_CHK_DONE_ENG_BUF_INIT)
	{
		KillTimer(TIM_CHK_DONE_ENG_BUF_INIT);

		if (pDoc->m_pMpeSignal[6] & (0x01 << 10))	// ���κ� ���� �ʱ���ġ �̵�(PC�� ON, PLC�� OFF)
		{
			pView->GetDispMsg(strMsg, strTitle);
			if (strMsg != _T("Searching Engrave Buffer Initial Position...") || strTitle != _T("Moving"))
				pView->DispMsg(_T("Searching Engrave Buffer Initial Position..."), _T("Moving"), RGB_GREEN, DELAY_TIME_MSG);
		}
		else
		{
			m_bTIM_CHK_DONE_ENG_BUF_INIT = FALSE;
			pView->ClrDispMsg();

#ifdef USE_ENGRAVE
			if (pView && pView->m_pEngrave)
			{
				pDoc->WorkingInfo.Motion.bEngBuffInitMvDone = TRUE;
				pView->m_pEngrave->SetEngBuffInitMoveDone();	//_SigInx::_EngBuffInitPosMvDone // ���κ� ���� �ʱ���ġ �̵�(PC�� ON, PLC�� OFF)
			}
#endif
		}
		if (m_bTIM_CHK_DONE_ENG_BUF_INIT)
			SetTimer(TIM_CHK_DONE_ENG_BUF_INIT, 100, NULL);
	}

	if (nIDEvent == TIM_CHK_DONE_AOI)
	{
		KillTimer(TIM_CHK_DONE_AOI);

		if (!(pDoc->m_pMpeSignal[5] & (0x01 << 0)))	// �˻�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
			m_bTIM_CHK_DONE_AOI = FALSE;
		if (m_bTIM_CHK_DONE_AOI)
			SetTimer(TIM_CHK_DONE_AOI, 100, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CDlgMenu03::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{

	if(pView->m_bProbDn[0])
	{
		if(pView->m_pVoiceCoil[0])
		{
			pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
			pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);
			pView->m_bProbDn[0] = FALSE;
		}
	}

	if(pView->m_bProbDn[1])
	{
		if(pView->m_pVoiceCoil[1])
		{
			pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
			pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);
			pView->m_bProbDn[1] = FALSE;
		}
	}
	
	int nCtrlID = (int)lPara;

 	SwMyBtnDown(nCtrlID);
	return 0L;
}

LRESULT CDlgMenu03::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;

	SwMyBtnUp(nCtrlID);
	return 0L;
}

void CDlgMenu03::SwMyBtnDown(int nCtrlID)
{
	if (!pView)
		return;

	long lData = 1; BOOL bData = TRUE;

	if (nCtrlID != IDC_CHK_33 && nCtrlID != IDC_CHK_0)		// ��ŷ�� ���� ����ġ , ��ŷ�� ���� ����ġ
	{
		if (!pDoc->Status.bManual)
		{
			pView->DispMsg(_T("���� ��尡 �ƴմϴ�."), _T("����"), RGB_YELLOW);
			return;
		}
	}

#ifdef USE_ENGRAVE
	if (!pView->m_pEngrave)
		return;
#endif

	if (!pView->m_pMpe)
		return;

	SwMpeBtn(nCtrlID, lData);
}

void CDlgMenu03::SwMyBtnUp(int nCtrlID)
{
	if (!pView)
		return;

	long lData = 0; BOOL bData = FALSE;

	if (nCtrlID != IDC_CHK_33 && nCtrlID != IDC_CHK_0)		// ��ŷ�� ���� ����ġ , ��ŷ�� ���� ����ġ
	{
		if (!pDoc->Status.bManual)
		{
			pView->DispMsg(_T("���� ��尡 �ƴմϴ�."), _T("����"), RGB_YELLOW);
			return;
		}
	}

#ifdef USE_ENGRAVE
	if (!pView->m_pEngrave)
		return;
#endif

	if (!pView->m_pMpe)
		return;

	SwMpeBtn(nCtrlID, lData);
}

void CDlgMenu03::SwMpeBtn(int nCtrlID, long lData)
{
	switch (nCtrlID)
	{
	// Main
	case IDC_CHK_34:		// ��ŷ�� �����غ� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.Ready, lData);
		break;
	case IDC_CHK_0:			// ��ŷ�� ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.Run, lData);
		break;
	case IDC_CHK_1:			// ��ŷ�� ���� ����ġ	
		pView->MpeWrite(pView->Plc.DlgMenu03.Reset, lData);
		break;
	case IDC_CHK_33:		// ��ŷ�� ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.Stop, lData);
		break;

		// Recoiler
	case IDC_CHK_4:			// �����Ϸ� ���� ��/���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModuleRecoiler, 1);		// �����Ϸ� ���� ��/���� ����ġ
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModuleRecoiler, 0);		// �����Ϸ� ���� ��/���� ����ġ
		break;
	case IDC_CHK_5:			// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwRecoiler, 0);
		break;
	case IDC_CHK_6:			// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwRecoiler, 0);
		break;
	case IDC_CHK_41:		// �����Ϸ� ��ǰô Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPcbRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPcbRecoiler, 0);
		break;
	case IDC_CHK_42:		// �����Ϸ� ���� ���/�ϰ� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.DancerRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.DancerRecoiler, 0);
		break;
	case IDC_CHK_43:		// �����Ϸ� ��ǰ ������(��/��) ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinUpLfRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinUpLfRecoiler, 0);
		break;
	case IDC_CHK_7:			// �����Ϸ� ��ǰ ������(��/��) ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinDnRtRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinDnRtRecoiler, 0);
		break;
	case IDC_CHK_8:			// �����Ϸ� ��ǰ ������ ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinVacuumRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinVacuumRecoiler, 0);
		break;
	case IDC_CHK_44:		// �����Ϸ� ����ô Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPaperRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPaperRecoiler, 0);
		break;
	case IDC_CHK_45:		// �����Ϸ� ������ ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperCwRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperCwRecoiler, 0);
		break;
	case IDC_CHK_46:		// �����Ϸ� ������ ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperCcwRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperCcwRecoiler, 0);
		break;
	case IDC_CHK_66:		// �����Ϸ� Rewinder ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.RewinderRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.RewinderRecoiler, 0);
		break;
	case IDC_CHK_67:		// �����Ϸ� Rewinder ��ǰ & ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.RewinderPcbPaperRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.RewinderPcbPaperRecoiler, 0);
		break;

		// Marking
	case IDC_CHK_9:			// ��ŷ�� ���� ��/���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModulePunch, 1);		// ��ŷ�� ���� ��/���� ����ġ
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModulePunch, 0);		// ��ŷ�� ���� ��/���� ����ġ
		break;
	case IDC_CHK_10:		// ��ŷ�� �ǵ� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwPunch, 0);
		break;
	case IDC_CHK_11:		// ��ŷ�� �ǵ� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwPunch, 0);
		break;
	case IDC_CHK_12:		// ��ŷ�� �ǵ� ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumPunch, 0);
		break;
	case IDC_CHK_13:		// ��ŷ�� ��ǰǪ�� ����ġ // (��ũ ���� ����ġ) - X
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbPushPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbPushPunch, 0);
		break;
	case IDC_CHK_14:		// ��ŷ�� ���̺� ��ο� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerPunch, 0);
		break;
	case IDC_CHK_15:		// ��ŷ�� ���̺� ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumPunch, 0);
		break;
	case IDC_CHK_51:		// ��ŷ�� �ǵ� Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampPunch, 0);
		break;
	case IDC_CHK_52:		// ��ŷ�� �ټ� Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampPunch, 0);
		break;

		// AOI �ϸ� �˻�	
	case IDC_CHK_55:		// �˻�� �� ���� ��/���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModuleAoiDn, 1);		// �˻�� �� ���� ��/���� ����ġ
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModuleAoiDn, 0);		// �˻�� �� ���� ��/���� ����ġ
		break;
	case IDC_CHK_56:		// �˻�� �� �ǵ� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwAoiDn, 0);
		break;
	case IDC_CHK_57:		// �˻�� �� �ǵ� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiDn, 0);
		break;
	case IDC_CHK_58:		// �˻�� �� �ǵ� ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumAoiDn, 0);
		break;
	case IDC_CHK_59:		// �˻�� �� ��ǰǪ�� ����ġ // (��ũ ���� ����ġ) - X
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbPushAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbPushAoiDn, 0);
		break;
	case IDC_CHK_60:		// �˻�� �� ���̺� ��ο� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerAoiDn, 0);
		break;
	case IDC_CHK_61:		// �˻�� �� ���̺� ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumAoiDn, 0);
		break;
	case IDC_CHK_64:		// �˻�� �� �ǵ� Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampAoiDn, 0);
		break;
	case IDC_CHK_65:		// �˻�� �� �ټ� Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampAoiDn, 0);
		break;

		// AOI ��� �˻�
	case IDC_CHK_17:		// �˻�� �� ���� ��/���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModuleAoiUp, 1);		// �˻�� �� ���� ��/���� ����ġ
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModuleAoiUp, 0);		// �˻�� �� ���� ��/���� ����ġ
		break;
	case IDC_CHK_18:		// �˻�� �� �ǵ� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwAoiUp, 0);
		break;
	case IDC_CHK_19:		// �˻�� �� �ǵ� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiUp, 0);
		break;
	case IDC_CHK_20:		// �˻�� �� �ǵ� ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumAoiUp, 0);
		break;
	case IDC_CHK_21:		// �˻�� �� ��ǰǪ�� ����ġ // (��ũ ���� ����ġ) - X
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbPushAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbPushAoiUp, 0);
		break;
	case IDC_CHK_22:		// �˻�� �� ���̺� ��ο� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerAoiUp, 0);
		break;
	case IDC_CHK_23:		// �˻�� �� ���̺� ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumAoiUp, 0);
		break;
	case IDC_CHK_53:		// �˻�� �� �ǵ� Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampAoiUp, 0);
		break;
	case IDC_CHK_54:		// �˻�� �� �ټ� Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampAoiUp, 0);
		break;

		// Uncoiler
	case IDC_CHK_25:		// �����Ϸ� ���� ��/���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModuleUncoiler, 1);		// �����Ϸ� ���� ��/���� ����ġ
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModuleUncoiler, 0);		// �����Ϸ� ���� ��/���� ����ġ
		break;
	case IDC_CHK_26:		// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwUncoiler, 0);
		break;
	case IDC_CHK_27:		// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwUncoiler, 0);
		break;
	case IDC_CHK_35:		// �����Ϸ� ��ǰô Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPcbUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPcbUncoiler, 0);
		break;
	case IDC_CHK_28:		// �����Ϸ� ���� ���/�ϰ� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.DancerUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.DancerUncoiler, 0);
		break;
	case IDC_CHK_30:		// �����Ϸ� ��ǰ ������(��/��) ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinUpLfUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinUpLfUncoiler, 0);
		break;
	case IDC_CHK_37:		// �����Ϸ� ��ǰ ������(��/��) ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinDnRtUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinDnRtUncoiler, 0);
		break;
	case IDC_CHK_38:		// �����Ϸ� ��ǰ ������ ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinVacuumUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinVacuumUncoiler, 0);
		break;
	case IDC_CHK_31:		// �����Ϸ� ����ô Ŭ���� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPaperUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPaperUncoiler, 0);
		break;
	case IDC_CHK_32:		// �����Ϸ� ������ ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperCwUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperCwUncoiler, 0);
		break;
	case IDC_CHK_39:		// �����Ϸ� ������ ��ȸ�� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperCcwUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperCcwUncoiler, 0);
	break;
	case IDC_CHK_29:		// �����Ϸ� Ŭ���ѷ� ���/�ϰ� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.CleannerUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.CleannerUncoiler, 0);
		break;
	case IDC_CHK_36:		// �����Ϸ� Ŭ���ѷ����� ���/�ϰ� ����ġ
		pView->MpeWrite(pView->Plc.DlgMenu03.CleannerPushUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.CleannerPushUncoiler, 0);
		break;

	// Torque Motor
	case IDC_CHK_2:		// ��ŷ�� �ټ� ON (PC�� ON/OFF��Ŵ)
		if (!(pDoc->m_pMpeSignal[4] & (0x01 << 5)))
		{
			pDoc->WorkingInfo.Motion.bMkTq = TRUE;
			pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnPunch, 1);
		}
		else
		{
			pDoc->WorkingInfo.Motion.bMkTq = FALSE;
			pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnPunch, 0);
		}
		break;
	case IDC_CHK_3:		// �˻�� �ټ� ON (PC�� ON/OFF��Ŵ)
		if (!(pDoc->m_pMpeSignal[4] & (0x01 << 6)))
		{
			pDoc->WorkingInfo.Motion.bAoiTq = TRUE;
			pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnAoi, 1);
		}
		else
		{
			pDoc->WorkingInfo.Motion.bAoiTq = FALSE;
			pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnAoi, 0);
		}
		break;
	case IDC_CHK_84:	// ���κ� �ټ� ON (PC�� ON/OFF��Ŵ)
		if (!(pDoc->m_pMpeSignal[4] & (0x01 << 4)))
		{
			pDoc->WorkingInfo.Motion.bEngraveTq = TRUE;
			pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnEngrave, 1);
		}
		else
		{
			pDoc->WorkingInfo.Motion.bEngraveTq = FALSE;
			pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnEngrave, 0);
		}
		break;

		// [���κ� ������ ������ �ӵ�]
	case IDC_CHK_87:	// ���κ� �����ļ����� �ӵ� ON (PC�� ON/OFF��Ŵ)
		if (!(pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic))
		{
			pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic = TRUE;
			pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedEngrave, 1);
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic = FALSE;
			pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedEngrave, 0);
		}
		break;
		// [AOI(��) ������ ������ �ӵ�]
	case IDC_CHK_88:	// AOI(��) �����ļ����� �ӵ� ON (PC�� ON/OFF��Ŵ) 
		if (!(pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic))
		{
			pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic = TRUE;
			pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedAoiDn, 1);
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic = FALSE;
			pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedAoiDn, 0);
		}
		break;


		// [One Metal]
	case IDC_CHK_68:
		pView->SetTwoMetal(FALSE, TRUE);
#ifdef USE_ENGRAVE
		if (pView && pView->m_pEngrave)
			pView->m_pEngrave->SetRecoilerCcw();	//_stSigInx::_RecoilerCcw
#endif
		break;

		// [Two Metal]
	case IDC_CHK_69:
		pView->SetTwoMetal(TRUE, TRUE);
#ifdef USE_ENGRAVE
		if (pView && pView->m_pEngrave)
			pView->m_pEngrave->SetUncoilerCcw();	//_stSigInx::_UncoilerCcw
#endif
		break;

		// [Core 150mm] - Recoiler
	case IDC_CHK_70:
		SetCore150mmRecoiler(TRUE);
		break;

		// [Core 150mm] - Uncoiler
	case IDC_CHK_71:
		SetCore150mmUncoiler(TRUE);
		break;
	}

}

void CDlgMenu03::SwEngraveBtn(int nCtrlID, BOOL bOn)
{
#ifdef USE_ENGRAVE
	switch (nCtrlID)
	{
	// [Main]
	case IDC_CHK_34:		// ��ŷ�� �����غ� ����ġ "MB005503"
		pDoc->BtnStatus.Main.Ready = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Ready"), bOn);
		pView->m_pEngrave->SwReady(bOn);
		break;
	case IDC_CHK_0:			// ��ŷ�� ���� ����ġ "MB005501"
		pDoc->BtnStatus.Main.Run = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Run"), bOn);
		pView->m_pEngrave->SwRun(bOn);
		break;
	case IDC_CHK_1:			// ��ŷ�� ���� ����ġ "MB005504"
		pDoc->BtnStatus.Main.Reset = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Reset"), bOn);
		pView->m_pEngrave->SwReset(bOn);
		break;
	case IDC_CHK_33:		// ��ŷ�� ���� ����ġ "MB005502"
		pDoc->BtnStatus.Main.Stop = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Stop"), bOn);
		pView->m_pEngrave->SwStop(bOn);
		break;

	// [Torque Motor]
	case IDC_CHK_2:		// ��ŷ�� �ټ� ON (PC�� ON/OFF��Ŵ) "MB440155"
		pDoc->WorkingInfo.Motion.bMkTq = pDoc->BtnStatus.Tq.Mk = bOn;
		pDoc->SetMkMenu03(_T("TqMotor"), _T("MkTq"), bOn);
		pView->m_pEngrave->SwMkTq(bOn); // pDoc->WorkingInfo.Motion.bMkTq
		break;
	case IDC_CHK_3:		// �˻�� �ټ� ON (PC�� ON/OFF��Ŵ) "MB440156"
		pDoc->WorkingInfo.Motion.bAoiTq = pDoc->BtnStatus.Tq.Aoi = bOn;
		pDoc->SetMkMenu03(_T("TqMotor"), _T("AoiTq"), bOn);
		pView->m_pEngrave->SwAoiTq(bOn); // pDoc->WorkingInfo.Motion.bAoiTq
		break;
	case IDC_CHK_84:	// ���κ� �ټ� ON (PC�� ON/OFF��Ŵ) "MB440154"
		pDoc->WorkingInfo.Motion.bEngraveTq = pDoc->BtnStatus.Tq.Eng = bOn;
		pDoc->SetMkMenu03(_T("TqMotor"), _T("EngTq"), bOn);
		pView->m_pEngrave->SwEngTq(bOn); // pDoc->WorkingInfo.Motion.bEngraveTq
		break;

	// [Induction Motor}
	case IDC_CHK_68:	// One Metal		//pView->SetTwoMetal(FALSE, TRUE);
		pDoc->WorkingInfo.LastJob.bOneMetal = pDoc->BtnStatus.Induct.Rc = bOn;
		pDoc->SetMkMenu03(_T("Induction"), _T("RcCcw"), bOn);
		pView->m_pEngrave->SwRcInductionCcw(bOn);
		break;

	case IDC_CHK_69:	// Two Metal		//pView->SetTwoMetal(TRUE, TRUE);
		pDoc->WorkingInfo.LastJob.bTwoMetal = pDoc->BtnStatus.Induct.Uc = bOn;
		pDoc->SetMkMenu03(_T("Induction"), _T("UcCcw"), bOn);
		pView->m_pEngrave->SwUcInductionCcw(bOn);
		break;

	// [Core 150mm]
	case IDC_CHK_70:	// Recoiler		//SetCore150mmRecoiler(TRUE);
		pDoc->BtnStatus.Core150.Rc = bOn;
		pDoc->SetMkMenu03(_T("Core"), _T("Rc150"), bOn);
		pView->m_pEngrave->SwRcCore150mm(bOn);
		break;

	case IDC_CHK_71:	// Uncoiler		//SetCore150mmUncoiler(TRUE);
		pDoc->BtnStatus.Core150.Uc = bOn;
		pDoc->SetMkMenu03(_T("Core"), _T("Uc150"), bOn);
		pView->m_pEngrave->SwUcCore150mm(bOn);
		break;

	// Recoiler
	case IDC_CHK_4:			// �����Ϸ� ���� ��/���� ����ġ "MB005801"
		pDoc->BtnStatus.Rc.Relation = bOn;
		pDoc->BtnStatus.Mk.Relation = bOn;
		pDoc->BtnStatus.AoiDn.Relation = bOn;
		pDoc->BtnStatus.AoiUp.Relation = bOn;
		pDoc->BtnStatus.Eng.Relation = bOn;
		pDoc->BtnStatus.Uc.Relation = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Relation"), bOn);
		pView->m_pEngrave->SwRcRelation(bOn);
		break;
	case IDC_CHK_5:			// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ "MB00580C"
		pDoc->BtnStatus.Rc.FdCw = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("MvCw"), bOn);
		pView->m_pEngrave->SwRcFdCw(bOn);
		break;
	case IDC_CHK_6:			// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ "MB00580D"
		pDoc->BtnStatus.Rc.FdCcw = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("MvCcw"), bOn);
		pView->m_pEngrave->SwRcFdCcw(bOn);
		break;
	case IDC_CHK_41:		// �����Ϸ� ��ǰô Ŭ���� ����ġ "MB00580B"
		pDoc->BtnStatus.Rc.ReelChuck = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("PrdChuck"), bOn);
		pView->m_pEngrave->SwRcReelChuck(bOn);
		break;
	case IDC_CHK_42:		// �����Ϸ� ���� ���/�ϰ� ����ġ "MB005802"
		pDoc->BtnStatus.Rc.DcRlUpDn = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("DancerUp"), bOn);
		pView->m_pEngrave->SwRcDcRlUpDn(bOn);
		break;
	case IDC_CHK_43:		// �����Ϸ� ��ǰ ������(��/��) ����ġ "MB005805"
		pDoc->BtnStatus.Rc.ReelJoinL = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteUpLf"), bOn);
		pView->m_pEngrave->SwRcReelJoinL(bOn);
		break;
	case IDC_CHK_7:			// �����Ϸ� ��ǰ ������(��/��) ����ġ "MB005806"
		pDoc->BtnStatus.Rc.ReelJoinR = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteUpRt"), bOn);
		pView->m_pEngrave->SwRcReelJoinR(bOn);
		break;
	case IDC_CHK_8:			// �����Ϸ� ��ǰ ������ ���� ����ġ "MB00580F"
		pDoc->BtnStatus.Rc.ReelJoinVac = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteVac"), bOn);
		pView->m_pEngrave->SwRcReelJoinVac(bOn);
		break;
	case IDC_CHK_44:		// �����Ϸ� ����ô Ŭ���� ����ġ "MB005808"
		pDoc->BtnStatus.Rc.PprChuck = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("PprChuck"), bOn);
		pView->m_pEngrave->SwRcPaperChuck(bOn);
		break;
	case IDC_CHK_45:		// �����Ϸ� ������ ��ȸ�� ����ġ "MB005809"
		pDoc->BtnStatus.Rc.PprCw = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("PprCw"), bOn);
		pView->m_pEngrave->SwRcPaperCw(bOn);
		break;
	case IDC_CHK_46:		// �����Ϸ� ������ ��ȸ�� ����ġ "MB00580A"
		pDoc->BtnStatus.Rc.PprCcw = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("PprCcw"), bOn);
		pView->m_pEngrave->SwRcPaperCcw(bOn);
		break;
	case IDC_CHK_66:		// �����Ϸ� Rewinder ���� ����ġ "MB005803"
		pDoc->BtnStatus.Rc.Rewine = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("DoRewind"), bOn);
		pView->m_pEngrave->SwRcRewinder(bOn);
		break;
	case IDC_CHK_67:		// �����Ϸ� Rewinder ��ǰ & ���� ����ġ "MB005804"
		pDoc->BtnStatus.Rc.RewineReelPpr = bOn;
		pDoc->SetMkMenu03(_T("Recoiler"), _T("PrdPprRewind"), bOn);
		pView->m_pEngrave->SwRcRewinderReelPaper(bOn);
		break;

	// Punching
	case IDC_CHK_9:			// ��ŷ�� ���� ��/���� ����ġ "MB005511"
		pDoc->BtnStatus.Rc.Relation = bOn;
		pDoc->BtnStatus.Mk.Relation = bOn;
		pDoc->BtnStatus.AoiDn.Relation = bOn;
		pDoc->BtnStatus.AoiUp.Relation = bOn;
		pDoc->BtnStatus.Eng.Relation = bOn;
		pDoc->BtnStatus.Uc.Relation = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Relation"), bOn);
		pView->m_pEngrave->SwMkRelation(bOn);
		break;
	case IDC_CHK_10:		// ��ŷ�� �ǵ� ��ȸ�� ����ġ "MB005513"
		pDoc->BtnStatus.Mk.FdCw = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("MvCw"), bOn);
		pView->m_pEngrave->SwMkFdCw(bOn);
		break;
	case IDC_CHK_11:		// ��ŷ�� �ǵ� ��ȸ�� ����ġ "MB005514"
		pDoc->BtnStatus.Mk.FdCcw = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("MvCcw"), bOn);
		pView->m_pEngrave->SwMkFdCcw(bOn);
		break;
	case IDC_CHK_12:		// ��ŷ�� �ǵ� ���� ����ġ "MB005515"
		pDoc->BtnStatus.Mk.FdVac = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("FdVac"), bOn);
		pView->m_pEngrave->SwMkFdVac(bOn);
		break;
	case IDC_CHK_13:		// ��ŷ�� ��ǰǪ�� ����ġ "MB005516" // (��ũ ���� ����ġ) - X
		pDoc->BtnStatus.Mk.PushUp = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("PushUp"), bOn);
		pView->m_pEngrave->SwMkPushUp(bOn);
		break;
	case IDC_CHK_14:		// ��ŷ�� ���̺� ��ο� ����ġ "MB005512"
		pDoc->BtnStatus.Mk.TblBlw = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("TblBlw"), bOn);
		pView->m_pEngrave->SwMkTblBlw(bOn);
		break;
	case IDC_CHK_15:		// ��ŷ�� ���̺� ���� ����ġ "MB005517"
		pDoc->BtnStatus.Mk.TblVac = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("TblVac"), bOn);
		pView->m_pEngrave->SwMkTblVac(bOn);
		break;
	case IDC_CHK_51:		// ��ŷ�� �ǵ� Ŭ���� ����ġ "MB005519"
		pDoc->BtnStatus.Mk.FdClp = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("FdClamp"), bOn);
		pView->m_pEngrave->SwMkFdClp(bOn);
		break;
	case IDC_CHK_52:		// ��ŷ�� �ټ� Ŭ���� ����ġ "MB00551A"
		pDoc->BtnStatus.Mk.TqClp = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("TensClamp"), bOn);
		pView->m_pEngrave->SwMkTqClp(bOn);
		break;
	case IDC_CHK_16:		// ��ŷ�� ���ǳ� �̼� ����ġ "MB440151"
		pDoc->BtnStatus.Mk.MvOne = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("OnePnl"), bOn);
		pView->m_pEngrave->SwMkMvOne(bOn);
		break;
	case IDC_CHK_49:		// ��ŷ�� ��������ũ ����ġ "MB005518"
		pDoc->BtnStatus.Mk.LsrPt = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("Lsr"), bOn);
		pView->m_pEngrave->SwMkLsrPt(bOn);
		break;
	case IDC_CHK_48:		// ��ŷ�� ���� ���/�ϰ� ����ġ "MB00551B", "X421B"
		pDoc->BtnStatus.Mk.DcRSol = bOn;
		pDoc->SetMkMenu03(_T("Punching"), _T("DancerUp"), bOn);
		pView->m_pEngrave->SwMkDcRSol(bOn);
		break;

	// AOI �ϸ� �˻�	
	case IDC_CHK_55:		// �˻�� �� ���� ��/���� ����ġ "MB005701"
		pDoc->BtnStatus.Rc.Relation = bOn;
		pDoc->BtnStatus.Mk.Relation = bOn;
		pDoc->BtnStatus.AoiDn.Relation = bOn;
		pDoc->BtnStatus.AoiUp.Relation = bOn;
		pDoc->BtnStatus.Eng.Relation = bOn;
		pDoc->BtnStatus.Uc.Relation = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Relation"), bOn);
		pView->m_pEngrave->SwAoiDnRelation(bOn);
		break;
	case IDC_CHK_56:		// �˻�� �� �ǵ� ��ȸ�� ����ġ "MB005703"
		pDoc->BtnStatus.AoiDn.FdCw = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("MvCw"), bOn);
		pView->m_pEngrave->SwAoiDnFdCw(bOn);
		break;
	case IDC_CHK_57:		// �˻�� �� �ǵ� ��ȸ�� ����ġ "MB005704"
		pDoc->BtnStatus.AoiDn.FdCcw = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("MvCcw"), bOn);
		pView->m_pEngrave->SwAoiDnFdCcw(bOn);
		break;
	case IDC_CHK_58:		// �˻�� �� �ǵ� ���� ����ġ "MB005705"
		pDoc->BtnStatus.AoiDn.FdVac = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("FdVac"), bOn);
		pView->m_pEngrave->SwAoiDnFdVac(bOn);
		break;
	case IDC_CHK_59:		// �˻�� �� ��ǰǪ�� ����ġ "MB005706" // (��ũ ���� ����ġ) - X
		pDoc->BtnStatus.AoiDn.PushUp = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("PushUp"), bOn);
		pView->m_pEngrave->SwAoiDnPushUp(bOn);
		break;
	case IDC_CHK_60:		// �˻�� �� ���̺� ��ο� ����ġ "MB005702"
		pDoc->BtnStatus.AoiDn.TblBlw = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("TblBlw"), bOn);
		pView->m_pEngrave->SwAoiDnTblBlw(bOn);
		break;
	case IDC_CHK_61:		// �˻�� �� ���̺� ���� ����ġ "MB005707"
		pDoc->BtnStatus.AoiDn.TblVac = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("TblVac"), bOn);
		pView->m_pEngrave->SwAoiDnTblVac(bOn);
		break;
	case IDC_CHK_64:		// �˻�� �� �ǵ� Ŭ���� ����ġ "MB005709"
		pDoc->BtnStatus.AoiDn.FdClp = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("FdClamp"), bOn);
		pView->m_pEngrave->SwAoiDnFdClp(bOn);
		break;
	case IDC_CHK_65:		// �˻�� �� �ټ� Ŭ���� ����ġ "MB00570A"
		pDoc->BtnStatus.AoiDn.TqClp = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("TensClamp"), bOn);
		pView->m_pEngrave->SwAoiDnTqClp(bOn);
		break;
	case IDC_CHK_62:		// �˻�� �� ���ǳ� �̼� ����ġ "MB440151"
		pDoc->BtnStatus.AoiDn.MvOne = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("OnePnl"), bOn);
		pView->m_pEngrave->SwAoiDnMvOne(bOn);
		break;
	case IDC_CHK_63:		// �˻�� �� ��������ũ ����ġ "MB005708"
		pDoc->BtnStatus.AoiDn.LsrPt = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("Lsr"), bOn);
		pView->m_pEngrave->SwAoiDnLsrPt(bOn);
		break;
	case IDC_CHK_88:		// �˻�� �� ������ ������ �ӵ� ����ġ "MB44014F" //pDoc->WorkingInfo.LastJob.bAoiDnCleanner
		pDoc->BtnStatus.AoiDn.VelSonicBlw = bOn;
		pDoc->SetMkMenu03(_T("AoiDn"), _T("VelClrSonic"), bOn);
		pView->m_pEngrave->SwAoiDnVelSonicBlw(bOn);
		break;
		

	// AOI ��� �˻�
	case IDC_CHK_17:		// �˻�� �� ���� ��/���� ����ġ "MB005601"
		pDoc->BtnStatus.Rc.Relation = bOn;
		pDoc->BtnStatus.Mk.Relation = bOn;
		pDoc->BtnStatus.AoiDn.Relation = bOn;
		pDoc->BtnStatus.AoiUp.Relation = bOn;
		pDoc->BtnStatus.Eng.Relation = bOn;
		pDoc->BtnStatus.Uc.Relation = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Relation"), bOn);
		pView->m_pEngrave->SwAoiUpRelation(bOn);
		break;
	case IDC_CHK_18:		// �˻�� �� �ǵ� ��ȸ�� ����ġ "MB005603"
		pDoc->BtnStatus.AoiUp.FdCw = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("MvCw"), bOn);
		pView->m_pEngrave->SwAoiUpFdCw(bOn);
		break;
	case IDC_CHK_19:		// �˻�� �� �ǵ� ��ȸ�� ����ġ "MB005604"
		pDoc->BtnStatus.AoiUp.FdCcw = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("MvCcw"), bOn);
		pView->m_pEngrave->SwAoiUpFdCcw(bOn);
		break;
	case IDC_CHK_20:		// �˻�� �� �ǵ� ���� ����ġ "MB005605"
		pDoc->BtnStatus.AoiUp.FdVac = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("FdVac"), bOn);
		pView->m_pEngrave->SwAoiUpFdVac(bOn);
		break;
	case IDC_CHK_21:		// �˻�� �� ��ǰǪ�� ����ġ "MB005606" // (��ũ ���� ����ġ) - X
		pDoc->BtnStatus.AoiUp.PushUp = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("PushUp"), bOn);
		pView->m_pEngrave->SwAoiUpPushUp(bOn);
		break;
	case IDC_CHK_22:		// �˻�� �� ���̺� ��ο� ����ġ "MB005602"
		pDoc->BtnStatus.AoiUp.TblBlw = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("TblBlw"), bOn);
		pView->m_pEngrave->SwAoiUpTblBlw(bOn);
		break;
	case IDC_CHK_23:		// �˻�� �� ���̺� ���� ����ġ "MB005607"
		pDoc->BtnStatus.AoiUp.TblVac = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("TblVac"), bOn);
		pView->m_pEngrave->SwAoiUpTblVac(bOn);
		break;
	case IDC_CHK_53:		// �˻�� �� �ǵ� Ŭ���� ����ġ "MB005609"
		pDoc->BtnStatus.AoiUp.FdClp = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("FdClamp"), bOn);
		pView->m_pEngrave->SwAoiUpFdClp(bOn);
		break;
	case IDC_CHK_54:		// �˻�� �� �ټ� Ŭ���� ����ġ "MB00560A"
		pDoc->BtnStatus.AoiUp.TqClp = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("TensClamp"), bOn);
		pView->m_pEngrave->SwAoiUpTqClp(bOn);
		break;
	case IDC_CHK_24:		// �˻�� �� ���ǳ� �̼� ����ġ  "MB440151"
		pDoc->BtnStatus.AoiUp.MvOne = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("OnePnl"), bOn);
		pView->m_pEngrave->SwAoiUpMvOne(bOn);
		break;
	case IDC_CHK_50:		// �˻�� �� ��������ũ ����ġ "MB005608"
		pDoc->BtnStatus.AoiUp.LsrPt = bOn;
		pDoc->SetMkMenu03(_T("AoiUp"), _T("Lsr"), bOn);
		pView->m_pEngrave->SwAoiUpLsrPt(bOn);
		break;

	// ���κ�
	case IDC_CHK_72:			// ���κ� ���� ��/���� ����ġ
		pDoc->BtnStatus.Rc.Relation = bOn;
		pDoc->BtnStatus.Mk.Relation = bOn;
		pDoc->BtnStatus.AoiDn.Relation = bOn;
		pDoc->BtnStatus.AoiUp.Relation = bOn;
		pDoc->BtnStatus.Eng.Relation = bOn;
		pDoc->BtnStatus.Uc.Relation = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Relation"), bOn);
		pView->m_pEngrave->SwEngRelation(bOn);
		break;
	case IDC_CHK_73:		// ���κ� �ǵ� ��ȸ�� ����ġ
		pDoc->BtnStatus.Eng.FdCw = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("MvCw"), bOn);
		pView->m_pEngrave->SwEngFdCw(bOn);
		break;
	case IDC_CHK_74:		// ���κ� �ǵ� ��ȸ�� ����ġ
		pDoc->BtnStatus.Eng.FdCcw = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("MvCcw"), bOn);
		pView->m_pEngrave->SwEngFdCcw(bOn);
		break;
	case IDC_CHK_75:		// ���κ� �ǵ� ���� ����ġ
		pDoc->BtnStatus.Eng.FdVac = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("FdVac"), bOn);
		pView->m_pEngrave->SwEngFdVac(bOn);
		break;
	case IDC_CHK_76:		// ���κ� ��ǰǪ�� ����ġ // (��ũ ���� ����ġ) - X
		pDoc->BtnStatus.Eng.PushUp = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("PushUp"), bOn);
		pView->m_pEngrave->SwEngPushUp(bOn);
		break;
	case IDC_CHK_77:		// ���κ� ���̺� ��ο� ����ġ
		pDoc->BtnStatus.Eng.TblBlw = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("TblBlw"), bOn);
		pView->m_pEngrave->SwEngTblBlw(bOn);
		break;
	case IDC_CHK_78:		// ���κ� ���̺� ���� ����ġ
		pDoc->BtnStatus.Eng.TblVac = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("TblVac"), bOn);
		pView->m_pEngrave->SwEngTblVac(bOn);
		break;
	case IDC_CHK_82:		// ���κ� �ǵ� Ŭ���� ����ġ
		pDoc->BtnStatus.Eng.FdClp = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("FdClamp"), bOn);
		pView->m_pEngrave->SwEngFdClp(bOn);
		break;
	case IDC_CHK_83:		// ���κ� �ټ� Ŭ���� ����ġ
		pDoc->BtnStatus.Eng.TqClp = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("TensClamp"), bOn);
		pView->m_pEngrave->SwEngTqClp(bOn);
		break;
	case IDC_CHK_79:		// ���κ� ���ǳ� �̼� ����ġ  "MB440151"
		pDoc->BtnStatus.Eng.MvOne = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("OnePnl"), bOn);
		pView->m_pEngrave->SwEngMvOne(bOn);
		break;
	case IDC_CHK_81:		// ���κ� ��������ũ ����ġ "" IDC_CHK_81
		pDoc->BtnStatus.Eng.LsrPt = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("Lsr"), bOn);
		pView->m_pEngrave->SwEngLsrPt(bOn);
		break;
	case IDC_CHK_80:		// ���κ� ���� ���/�ϰ� ����ġ
		pDoc->BtnStatus.Eng.DcRSol = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("VelClrSonic"), bOn);
		pView->m_pEngrave->SwEngDcRSol(bOn);
		break;
	case IDC_CHK_87:		// ���κ� ������ ������ ����ġ "MB44014E" pDoc->WorkingInfo.LastJob.bEngraveCleanner
		pDoc->BtnStatus.Eng.VelSonicBlw = bOn;
		pDoc->SetMkMenu03(_T("Engraving"), _T("DancerUp"), bOn);
		pView->m_pEngrave->SwEngVelSonicBlw(bOn);
		break;

	// Uncoiler
	case IDC_CHK_25:		// �����Ϸ� ���� ��/���� ����ġ "MB005401"
		pDoc->BtnStatus.Rc.Relation = bOn;
		pDoc->BtnStatus.Mk.Relation = bOn;
		pDoc->BtnStatus.AoiDn.Relation = bOn;
		pDoc->BtnStatus.AoiUp.Relation = bOn;
		pDoc->BtnStatus.Eng.Relation = bOn;
		pDoc->BtnStatus.Uc.Relation = bOn;
		pDoc->SetMkMenu03(_T("Main"), _T("Relation"), bOn);
		pView->m_pEngrave->SwUcRelation(bOn);
		break;
	case IDC_CHK_26:		// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ "MB00540C"
		pDoc->BtnStatus.Uc.FdCw = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("MvCw"), bOn);
		pView->m_pEngrave->SwUcFdCw(bOn);
		break;
	case IDC_CHK_27:		// �����Ϸ� ��ǰ�� ��ȸ�� ����ġ "MB00540D"
		pDoc->BtnStatus.Uc.FdCcw = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("MvCcw"), bOn);
		pView->m_pEngrave->SwUcFdCcw(bOn);
		break;
	case IDC_CHK_35:		// �����Ϸ� ��ǰô Ŭ���� ����ġ "MB00540B"
		pDoc->BtnStatus.Uc.ReelChuck = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("PrdChuck"), bOn);
		pView->m_pEngrave->SwUcReelChuck(bOn);
		break;
	case IDC_CHK_28:		// �����Ϸ� ���� ���/�ϰ� ����ġ "MB005402"
		pDoc->BtnStatus.Uc.DcRlUpDn = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("DancerUp"), bOn);
		pView->m_pEngrave->SwUcDcRlUpDn(bOn);
		break;
	case IDC_CHK_30:		// �����Ϸ� ��ǰ ������(��/��) ����ġ "MB005405"
		pDoc->BtnStatus.Uc.ReelJoinL = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("PasteUpLf"), bOn);
		pView->m_pEngrave->SwUcReelJoinL(bOn);
		break;
	case IDC_CHK_37:		// �����Ϸ� ��ǰ ������(��/��) ����ġ "MB005406"
		pDoc->BtnStatus.Uc.ReelJoinR = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("PasteUpRt"), bOn);
		pView->m_pEngrave->SwUcReelJoinR(bOn);
		break;
	case IDC_CHK_38:		// �����Ϸ� ��ǰ ������ ���� ����ġ "MB00540F"
		pDoc->BtnStatus.Uc.ReelJoinVac = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("PasteVac"), bOn);
		pView->m_pEngrave->SwUcReelJoinVac(bOn);
		break;
	case IDC_CHK_31:		// �����Ϸ� ����ô Ŭ���� ����ġ "MB005408"
		pDoc->BtnStatus.Uc.PprChuck = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("PprChuck"), bOn);
		pView->m_pEngrave->SwUcPaperChuck(bOn);
		break;
	case IDC_CHK_32:		// �����Ϸ� ������ ��ȸ�� ����ġ "MB005409"
		pDoc->BtnStatus.Uc.PprCw = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("PprCw"), bOn);
		pView->m_pEngrave->SwUcPaperCw(bOn);
		break;
	case IDC_CHK_39:		// �����Ϸ� ������ ��ȸ�� ����ġ "MB00540A"
		pDoc->BtnStatus.Uc.PprCcw = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("PprCcw"), bOn);
		pView->m_pEngrave->SwUcPaperCcw(bOn);
		break;
	case IDC_CHK_29:		// �����Ϸ� Ŭ���ѷ� ���/�ϰ� ����ġ "MB005403"
		pDoc->BtnStatus.Uc.ClRlUpDn = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("ClrRollUp"), bOn);
		pView->m_pEngrave->SwUcClRlUpDn(bOn);
		break;
	case IDC_CHK_36:		// �����Ϸ� Ŭ���ѷ����� ���/�ϰ� ����ġ "MB005404"
		pDoc->BtnStatus.Uc.ClRlPshUpDn = bOn;
		pDoc->SetMkMenu03(_T("Uncoiler"), _T("ClrRollPush"), bOn);
		pView->m_pEngrave->SwUcClRlPshUpDn(bOn);
		break;
	}
#endif
}

BOOL CDlgMenu03::GetCw() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	if(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<3))		//[29] ��ŷ�� �ǵ� ��ȸ�� ����ġ ����
		return TRUE;
	if(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<3))		//[32] �˻�� �ǵ� ��ȸ�� ����ġ ����
		return TRUE;
	return FALSE;
}

void CDlgMenu03::SetCw(BOOL bOn) 
{
	long lData = 1;
	pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwPunch, lData);	// ��ŷ�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
	pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwAoi, lData);	// �˻�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
}

BOOL CDlgMenu03::GetCcw() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	if(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<4))		//[29] ��ŷ�� �ǵ� ��ȸ�� ����ġ ���� 
		return TRUE;
	if(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<4))		//[32] �˻�� �ǵ� ��ȸ�� ����ġ ����
		return TRUE;
	return FALSE;
}

void CDlgMenu03::SetCcw(BOOL bOn) 
{
	long lData = 1;
	pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwPunch, lData);	// ��ŷ�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
	pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwAoi, lData);	// �˻�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
}

void CDlgMenu03::MoveMk(double dOffset)
{
	double fLen, fVel, fAcc, fTarget;

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = dOffset;// fLen + dOffset;

	long lData = (long)(fTarget * 1000.0);
	pView->MpeWrite(pView->Plc.DlgFrameHigh.FeedOffsetPunch, lData);	// ��ŷ�� Feeding �ѷ� Offset(*1000, +:�� ����, -�� ����)

	MoveMk();
} 

void CDlgMenu03::MoveMk(int nDir) 
{
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl)
	{
		pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwPunch, 1);	// ��ŷ�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
	}
}

void CDlgMenu03::MoveAoi(double dOffset)
{
 	double fLen, fVel, fAcc, fTarget;

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = dOffset;// fLen - dOffset;

 	long lData = (long)(fTarget * 1000.0);
	pView->MpeWrite(pView->Plc.DlgFrameHigh.FeedOffsetAoi, lData);	// �˻�� Feeding �ѷ� Offset(*1000, +:�� ����, -�� ����)

	MoveAoi();
} 

void CDlgMenu03::MoveAoi(int nDir) 
{
	if(pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
		pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwAoi, 1);	// �˻�� �ǵ� ON (PLC�� �ǵ��Ϸ� �� OFF)
	}
}

void CDlgMenu03::ChkDoneMk() 
{
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl)
	{
		if(!m_bTIM_CHK_DONE_MK)
		{
			m_bTIM_CHK_DONE_MK = TRUE;
			SetTimer(TIM_CHK_DONE_MK, 100, NULL);
		}
	}
}

void CDlgMenu03::ChkDoneAoi() 
{
	if(pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
		if(!m_bTIM_CHK_DONE_AOI)
		{
			m_bTIM_CHK_DONE_AOI = TRUE;
			SetTimer(TIM_CHK_DONE_AOI, 100, NULL);
		}
	}
}

void CDlgMenu03::ChkDoneMkAoi() 
{
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl && pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
		if(!m_bTIM_CHK_DONE_MKAOI)
		{
			m_bTIM_CHK_DONE_MKAOI = TRUE;
			SetTimer(TIM_CHK_DONE_MKAOI, 100, NULL);
		}
	}
}

BOOL CDlgMenu03::GetRelation() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	if(!(pDoc->m_pMpeIo[nInSeg + 16] & (0x01<<1)))	//[40] �����Ϸ� ���� ��/���� ����ġ ����
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg] & (0x01<<1)))		//[24] �����Ϸ� ���� ��/���� ����ġ ����
		return FALSE;

	if(!(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<1)))	//[29] ��ŷ�� ���� ��/���� ����ġ ����
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<1)))	//[32] �˻�� �� ���� ��/���� ����ġ ����
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg + 12] & (0x01<<1)))	//[36] �˻�� �� ���� ��/���� ����ġ ����
		return FALSE;

	return TRUE;
}

BOOL CDlgMenu03::GetRun() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	if(pView->m_bSwRun) // �ʱ������ ���� On/Off
		return TRUE;

	return FALSE;
}

BOOL CDlgMenu03::GetReady()
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	if(pView->m_bReadyDone)
		return TRUE;

	return FALSE;
}

BOOL CDlgMenu03::GetReset()
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	if(pView->m_bSwReset)
		return TRUE;

	return FALSE;
}

BOOL CDlgMenu03::GetMkTq()
{
	return (pDoc->WorkingInfo.Motion.bMkTq);
}

BOOL CDlgMenu03::GetAoiTq()
{
	return (pDoc->WorkingInfo.Motion.bAoiTq);
}

BOOL CDlgMenu03::GetEngraveTq()
{
	return (pDoc->WorkingInfo.Motion.bEngraveTq);
}

void CDlgMenu03::SetMkTq(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnPunch, bOn?1:0);	// ��ŷ�� �ټ� ON (PC�� ON/OFF��Ŵ)
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bMkTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE_ON"), sData, sPath);
}

void CDlgMenu03::SetAoiTq(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnAoi, bOn?1:0);	// �˻�� �ټ� ON (PC�� ON/OFF��Ŵ)
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bAoiTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE_ON"), sData, sPath);	
}

void CDlgMenu03::SetEngraveTq(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.TensionOnEngrave, bOn?1:0);	// �˻�� �ټ� ON (PC�� ON/OFF��Ŵ)
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bEngraveTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE_ON"), sData, sPath);	
}

void CDlgMenu03::SetMkOnePnl(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, bOn?1:0);	// ���ǳ� �̼ۻ��� ON (PC�� ON, OFF)
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bMkOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bMkOnePnl?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), sData, sPath);
}

void CDlgMenu03::SetEngraveCleanner(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedEngrave, bOn ? 1 : 0); // [���κ� ������]
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Cleanner Vel"), sData, sPath);
}

void CDlgMenu03::SetAoiDnCleanner(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedAoiDn, bOn ? 1 : 0); // [AOI(��) ������]
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("AoiDn Cleanner Vel"), sData, sPath);
}

void CDlgMenu03::SetAoiOnePnl(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, bOn?1:0);	// ���ǳ� �̼ۻ��� ON (PC�� ON, OFF)
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bAoiOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bAoiOnePnl?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), sData, sPath);
}

BOOL CDlgMenu03::DoReset()
{
	pView->DispThreadTick();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if(pDoc->Status.bManual)
	{
		BOOL bInit=TRUE;

		if(m_bTIM_CHK_DONE_READY)
		{
			m_bTIM_CHK_DONE_READY = FALSE;
			pView->m_bReadyDone = FALSE; pDoc->SetStatus(_T("General"), _T("bReadyDone"), pView->m_bReadyDone);
			pView->MpeWrite(_T("MB440100"), 0);	// PLC �����غ� �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
		}
		pView->ClrDispMsg();

		if (pView->m_pEngrave)
		{
			pDoc->BtnStatus.EngAuto.Init = TRUE;
			pDoc->BtnStatus.EngAuto.IsInit = FALSE;
			pView->m_pEngrave->SwEngAutoInit(TRUE);
		}
		
		if(IDNO == pView->MsgBox(_T("�ʱ�ȭ�� �Ͻðڽ��ϱ�?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			bInit = FALSE;
		else
		{
			pDoc->m_bDoneChgLot = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneChgLot"), pDoc->m_bDoneChgLot);
			pView->m_nNewLot = 0;

			if (pView->m_pDlgMenu01)
				pView->m_pDlgMenu01->ClrInfo();
		}

		if(!bInit)
		{
			if(IDNO == pView->MsgBox(_T("�̾�⸦ �Ͻðڽ��ϱ�?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				pView->m_bCont = FALSE; pDoc->SetStatus(_T("General"), _T("bCont"), pView->m_bCont);
				return FALSE;
			}
			pView->m_bCont = TRUE; pDoc->SetStatus(_T("General"), _T("bCont"), pView->m_bCont);
		}
		pView->m_nDebugStep = 1; pView->DispThreadTick();
		pView->InitAuto(bInit);

		pView->m_nDebugStep = 2; pView->DispThreadTick();
		pView->SetPathAtBuf();

		pView->m_nDebugStep = 3; pView->DispThreadTick();
		pView->SetAoiDummyShot(0, pView->GetAoiUpDummyShot());

		pView->m_nDebugStep = 4; pView->DispThreadTick();
		if(bDualTest)
			pView->SetAoiDummyShot(1, pView->GetAoiDnDummyShot());

		pView->m_nDebugStep = 5; pView->DispThreadTick();
		pView->m_bAoiFdWrite[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWrite[0]"), pView->m_bAoiFdWrite[0]);
		pView->m_bAoiFdWrite[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWrite[1]"), pView->m_bAoiFdWrite[1]);
		pView->m_bAoiFdWriteF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);
		pView->m_bAoiFdWriteF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[1]"), pView->m_bAoiFdWriteF[1]);
		pView->m_bCycleStop = FALSE; pDoc->SetStatus(_T("General"), _T("bCycleStop"), pView->m_bCycleStop);
		pView->m_bContDiffLot = FALSE; pDoc->SetStatus(_T("General"), _T("bContDiffLot"), pView->m_bContDiffLot);

		pView->m_bInit = bInit;

		CFileFind cFile;
		BOOL bExistSup, bExistBup, bExistSdn, bExistBdn;
		BOOL bExistSVsup, bExistBVsup, bExistSVsdn, bExistBVsdn;

		bExistSup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareUp + _T("*.pcr"));
		bExistBup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufUp + _T("*.pcr"));
		bExistSVsup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsShareUp + _T("*.pcr"));
		bExistBVsup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsDummyBufUp + _T("*.pcr"));
		if(bDualTest)
		{
			bExistSdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareDn + _T("*.pcr"));
			bExistBdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufDn + _T("*.pcr"));
			bExistSVsdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsShareDn + _T("*.pcr"));
			bExistBVsdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsDummyBufDn + _T("*.pcr"));
			if(bExistSup || bExistSdn ||
				bExistBup || bExistBdn ||
				bExistSVsup || bExistSVsdn ||
				bExistBVsup || bExistBVsdn)
			{
				pDoc->DelPcrAll();
			}
			else
			{
				pView->m_bIsBuf[0] = FALSE;
				pView->m_bIsBuf[1] = FALSE;
			}
		}
		else
		{
			if(bExistSup || bExistBup || bExistSVsup || bExistBVsup)
			{
				pDoc->DelPcrAll();
			}
			else
			{
				pView->m_bIsBuf[0] = FALSE;
				pView->m_bIsBuf[1] = FALSE;
			}
		}

		pView->m_nDebugStep = 12; pView->DispThreadTick();
		pView->TowerLamp(RGB_RED, TRUE, FALSE);
		pView->m_nDebugStep = 13; pView->DispThreadTick();
		pView->m_nDebugStep = 14; pView->DispThreadTick();
		pView->DispMain(_T("�� ��"), RGB_RED);	
		pView->m_nDebugStep = 15; pView->DispThreadTick();
		SwAoiReset(TRUE);
		pView->OpenReelmap();

		pView->m_nDebugStep = 16; pView->DispThreadTick();
		if(bInit)
		{
			// �ʱ�ȭ
			pDoc->m_ListBuf[0].Clear();
			pDoc->m_ListBuf[1].Clear();
		}
		else
		{
			// �̾��
			pView->SetListBuf();
		}
		pView->m_nDebugStep = 17; pView->DispThreadTick();
		
		return TRUE;
	}

	return FALSE;		
}

void CDlgMenu03::DoReady()
{
	if(m_bTIM_CHK_DONE_READY)
	{
		m_bTIM_CHK_DONE_READY = FALSE;
		KillTimer(TIM_CHK_DONE_READY);
	}
	ChkReadyDone();
	pDoc->DelPcrAll();
}

void CDlgMenu03::ChkBufHomeDone()
{
	if(!m_bTIM_CHK_DONE_BUF_HOME)
	{
		m_bTIM_CHK_DONE_BUF_HOME = TRUE;
		SetTimer(TIM_CHK_DONE_BUF_HOME, 100, NULL);
	}
}

void CDlgMenu03::ChkBufInitDone()
{
	if(!m_bTIM_CHK_DONE_BUF_INIT)
	{
		m_bTIM_CHK_DONE_BUF_INIT = TRUE;
		SetTimer(TIM_CHK_DONE_BUF_INIT, 100, NULL);
	}
}

void CDlgMenu03::ChkReadyDone()
{
	if(!m_bTIM_CHK_DONE_READY)
	{
		m_bTIM_CHK_DONE_READY = TRUE;
		SetTimer(TIM_CHK_DONE_READY, 100, NULL);
	}
}

// [Main]
void CDlgMenu03::SwRun() 
{
	if(pDoc->Status.bAuto)
	{
		pView->m_bSwRun = TRUE;
		pView->m_nStop = 0;
		pView->m_bSwStop = FALSE;
		pView->m_bSwReady = FALSE;
		pView->m_bSwReset = FALSE;
		pView->m_bCycleStop = FALSE; pDoc->SetStatus(_T("General"), _T("bCycleStop"), pView->m_bCycleStop);

		// ���ǳ� �̼� On
		SetMkOnePnl(TRUE);
		SetAoiOnePnl(TRUE);
		SetEngraveOnePnl(TRUE);

		pDoc->m_sAlmMsg = _T("");
		pDoc->m_sIsAlmMsg = _T("");
		pDoc->m_sPrevAlmMsg = _T("");
	}

	pView->ClrDispMsg();
}

BOOL CDlgMenu03::IsStop() 
{
	return pView->m_bSwStop;
}

void CDlgMenu03::SwStop() 
{
	pView->m_bSwRun = FALSE;
	pView->m_bSwStop = TRUE;
	pView->m_bSwReady = FALSE;
	pView->m_bSwReset = FALSE;

	pView->DispMain(_T("�� ��"), RGB_RED);
	pView->TowerLamp(RGB_RED, TRUE, FALSE);
	pView->MpeWrite(_T("MB440162"), 1); // ��ŷ�� ���� ����ġ ���� ON(PC�� On/Off��Ŵ)
	pView->ClrDispMsg();
}

void CDlgMenu03::SwReady()
{
 	if(!pView->m_bSwRun)
	{
		pView->m_bSwReady = TRUE;
 		DoReady();
	}
}

void CDlgMenu03::SwReset()
{
	pView->ClrDispMsg();

	if(!DoReset())
		return;

	pView->m_bSwRun = FALSE;
	pView->m_bSwStop = FALSE;
	pView->m_bSwReady = FALSE;
	pView->m_bSwReset = TRUE;
}

void CDlgMenu03::SwEngraveTq(BOOL bOn)
{
	SetEngraveTq(bOn);
}

// [Marking]

BOOL CDlgMenu03::IsMkTblBlw()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<2);		//[29] ��ŷ�� ���̺� ��ο� ����ġ ����
	return bOn;
}

BOOL CDlgMenu03::IsMkFdVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<5);	//[29] MB003715,	Y4255	,	��ŷ�� �ǵ� ���� ����ġ ����
	return bOn;
}

BOOL CDlgMenu03::IsMkTqVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<6);		//[29] MB003716,	Y4256	,	��ŷ�� ��ũ ���� ����ġ ����
	return bOn;
}

BOOL CDlgMenu03::IsMkTblVac() 
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<7);		//[29] MB003717,	Y4257	,	��ŷ�� ���̺� ���� ����ġ ����
	return bOn;
}

BOOL CDlgMenu03::IsBufRolSol() 
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<11);		//[29] MB00371B,	Y425B	,	��ŷ�� ���� ���/�ϰ� ����ġ ����
	return bOn;
}

// [AOI]
void CDlgMenu03::SwAoiRelation() 
{
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl != pDoc->WorkingInfo.LastJob.bAoiOnePnl) 
	{
		SetMkOnePnl(FALSE);
		SetAoiOnePnl(FALSE);
		SetEngraveOnePnl(FALSE);
	}
}

BOOL CDlgMenu03::IsAoiFdVac()
{
	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIb[9] & (0x01<<4);
	return bOn;
}

BOOL CDlgMenu03::IsAoiLdRun()
{
	BOOL bRtn = TRUE;

	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 11)) ? TRUE : FALSE;		// �˻�� �� �ڵ� ���� <-> X432B I/F
 	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01<<11)) ? TRUE : FALSE;		// �˻�� �� �ڵ� ���� <-> X442B I/F

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if(bDualTest)
	{
		if((pView->m_AoiLdRun & 0x03) == 0x03)
		{
			if(!bOn0 || !bOn1)
				bRtn = FALSE;
		}

		if(bOn0)
			pView->m_AoiLdRun |= (0x01<<0);
		else 
			pView->m_AoiLdRun &= ~(0x01<<0);

		if(bOn1)
			pView->m_AoiLdRun |= (0x01<<1);
		else 
			pView->m_AoiLdRun &= ~(0x01<<1);


		if(bOn0 && bOn1)
			return TRUE;
		else if(bOn0 && !bOn1)
			return FALSE;
		else if(!bOn0 && !bOn1)
			return FALSE;
	}
	else
	{
		if((pView->m_AoiLdRun & 0x03) == 0x03)
		{
			if(!bOn0)
				bRtn = FALSE;
		}

		if(bOn0)
			pView->m_AoiLdRun |= (0x01<<0);
		else 
			pView->m_AoiLdRun &= ~(0x01<<0);


		if(bOn0)
			return TRUE;

		return FALSE;
	}
	return bRtn;
}

void CDlgMenu03::SwAoiReset(BOOL bOn)
{
	if(bOn)
	{
		pView->MpeWrite(_T("MB00382A"), 1); // �˻�� �� Reset <-> Y436A I/F
		pView->MpeWrite(_T("MB00392A"), 1); // �˻�� �� Reset <-> Y446A I/F
		SetTimer(TIM_AOI_RESET_OFF, 500, NULL);
	}
	else
	{
		pView->MpeWrite(_T("MB00382A"), 0); // �˻�� �� Reset <-> Y436A I/F
		pView->MpeWrite(_T("MB00392A"), 0); // �˻�� �� Reset <-> Y446A I/F
	}
}

void CDlgMenu03::OnChk16() 
{
	BOOL bOn = pDoc->WorkingInfo.LastJob.bMkOnePnl;				// ��ŷ�� ���ǳ� �̼�
	SetMkOnePnl(!bOn);

	SetAoiOnePnl(!bOn);
	SetEngraveOnePnl(!bOn); 
}

void CDlgMenu03::OnChk24() 
{
	BOOL bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl;			// �˻�� ���ǳ� �̼�
	SetAoiOnePnl(!bOn);

	SetMkOnePnl(!bOn);
	SetEngraveOnePnl(!bOn);
}

BOOL CDlgMenu03::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		case 'S':
		case 's':
			if(GetKeyState(VK_CONTROL) < 0) // Ctrl Ű�� ������ ����
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE; 
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);	
			}
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMenu03::DoManual()
{
	SetMkOnePnl(FALSE);
	SetAoiOnePnl(FALSE);
	SetEngraveOnePnl(FALSE);

	SwStop();

	pView->ClrDispMsg();
}

void CDlgMenu03::DoAuto()
{
	SetMkOnePnl(TRUE);
	SetAoiOnePnl(TRUE);
	SetEngraveOnePnl(TRUE);
}

void CDlgMenu03::OnChk62() 
{
	BOOL bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl;			// �˻�� ���ǳ� �̼�
	SetAoiOnePnl(!bOn);

	BOOL bRelation = GetRelation();
	if (bRelation)
	{
		SetMkOnePnl(!bOn);
		SetEngraveOnePnl(!bOn);
	}
}

void CDlgMenu03::SetCore150mmRecoiler(BOOL bOn)
{
	pView->MpeWrite(_T("MB44017E"), bOn?1:0);
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bCore150Recoiler = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bCore150Recoiler?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Recoiler Core 150mm"), sData, sPath);
}

void CDlgMenu03::SetCore150mmUncoiler(BOOL bOn)
{
	pView->MpeWrite(_T("MB44017F"), bOn?1:0);
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bCore150Uncoiler = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bCore150Uncoiler?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Uncoiler Core 150mm"), sData, sPath);
}

void CDlgMenu03::SetDualTest(BOOL bOn)
{
	if(bOn)
	{
		if(!myStcTitle[61].IsWindowVisible())
			myStcTitle[61].ShowWindow(SW_SHOW);
		if(!myBtn[55].IsWindowVisible())
			myBtn[55].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[62].IsWindowVisible())
			myStcTitle[62].ShowWindow(SW_SHOW);
		if(!myBtn[56].IsWindowVisible())
			myBtn[56].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[63].IsWindowVisible())
			myStcTitle[63].ShowWindow(SW_SHOW);
		if(!myBtn[57].IsWindowVisible())
			myBtn[57].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[64].IsWindowVisible())
			myStcTitle[64].ShowWindow(SW_SHOW);
		if(!myBtn[58].IsWindowVisible())
			myBtn[58].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[65].IsWindowVisible())
			myStcTitle[65].ShowWindow(SW_SHOW);
		if(!myBtn[59].IsWindowVisible())
			myBtn[59].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[66].IsWindowVisible())
			myStcTitle[66].ShowWindow(SW_SHOW);
		if(!myBtn[60].IsWindowVisible())
			myBtn[60].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[67].IsWindowVisible())
			myStcTitle[67].ShowWindow(SW_SHOW);
		if(!myBtn[61].IsWindowVisible())
			myBtn[61].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[68].IsWindowVisible())
			myStcTitle[68].ShowWindow(SW_SHOW);
		if(!myBtn[62].IsWindowVisible())
			myBtn[62].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[69].IsWindowVisible())
			myStcTitle[69].ShowWindow(SW_SHOW);
		if(!myBtn[63].IsWindowVisible())
			myBtn[63].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[70].IsWindowVisible())
			myStcTitle[70].ShowWindow(SW_SHOW);
		if(!myBtn[64].IsWindowVisible())
			myBtn[64].ShowWindow(SW_SHOW);
		
		if(!myStcTitle[71].IsWindowVisible())
			myStcTitle[71].ShowWindow(SW_SHOW);
		if(!myBtn[65].IsWindowVisible())
			myBtn[65].ShowWindow(SW_SHOW);
	}
	else
	{
		if(myStcTitle[61].IsWindowVisible())
			myStcTitle[61].ShowWindow(SW_HIDE);
		if(myBtn[55].IsWindowVisible())
			myBtn[55].ShowWindow(SW_HIDE);
		
		if(myStcTitle[62].IsWindowVisible())
			myStcTitle[62].ShowWindow(SW_HIDE);
		if(myBtn[56].IsWindowVisible())
			myBtn[56].ShowWindow(SW_HIDE);
		
		if(myStcTitle[63].IsWindowVisible())
			myStcTitle[63].ShowWindow(SW_HIDE);
		if(myBtn[57].IsWindowVisible())
			myBtn[57].ShowWindow(SW_HIDE);
		
		if(myStcTitle[64].IsWindowVisible())
			myStcTitle[64].ShowWindow(SW_HIDE);
		if(myBtn[58].IsWindowVisible())
			myBtn[58].ShowWindow(SW_HIDE);
		
		if(myStcTitle[65].IsWindowVisible())
			myStcTitle[65].ShowWindow(SW_HIDE);
		if(myBtn[59].IsWindowVisible())
			myBtn[59].ShowWindow(SW_HIDE);
		
		if(myStcTitle[66].IsWindowVisible())
			myStcTitle[66].ShowWindow(SW_HIDE);
		if(myBtn[60].IsWindowVisible())
			myBtn[60].ShowWindow(SW_HIDE);
		
		if(myStcTitle[67].IsWindowVisible())
			myStcTitle[67].ShowWindow(SW_HIDE);
		if(myBtn[61].IsWindowVisible())
			myBtn[61].ShowWindow(SW_HIDE);
		
		if(myStcTitle[68].IsWindowVisible())
			myStcTitle[68].ShowWindow(SW_HIDE);
		if(myBtn[62].IsWindowVisible())
			myBtn[62].ShowWindow(SW_HIDE);
		
		if(myStcTitle[69].IsWindowVisible())
			myStcTitle[69].ShowWindow(SW_HIDE);
		if(myBtn[63].IsWindowVisible())
			myBtn[63].ShowWindow(SW_HIDE);
		
		if(myStcTitle[70].IsWindowVisible())
			myStcTitle[70].ShowWindow(SW_HIDE);
		if(myBtn[64].IsWindowVisible())
			myBtn[64].ShowWindow(SW_HIDE);
		
		if(myStcTitle[71].IsWindowVisible())
			myStcTitle[71].ShowWindow(SW_HIDE);
		if(myBtn[65].IsWindowVisible())
			myBtn[65].ShowWindow(SW_HIDE);
	}
}

BOOL CDlgMenu03::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB_DLG_FRM);
	return TRUE;
}

void CDlgMenu03::SetEngraveOnePnl(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, bOn ? 1 : 0);	// ���ǳ� �̼ۻ��� ON (PC�� ON, OFF)
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bEngraveOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bEngraveOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave One Pannel Move On"), sData, sPath);

}

void CDlgMenu03::OnBnClickedChk79()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bOn = pDoc->WorkingInfo.LastJob.bEngraveOnePnl;				// ���κ� ���ǳ� �̼�

	SetMkOnePnl(!bOn);
	SetAoiOnePnl(!bOn);
	SetEngraveOnePnl(!bOn);
}

void CDlgMenu03::MoveEngrave(double dOffset)
{
	double fLen, fVel, fAcc, fTarget;

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = dOffset;// fLen + dOffset;

	long lData = (long)(fTarget * 1000.0);
	pView->MpeWrite(pView->Plc.DlgFrameHigh.FeedOffsetEngrave, lData);	// ���κ� Feeding �ѷ� Offset(*1000, +:�� ����, -�� ����, PC�� ���� PLC���� ����)
	MoveEngrave();
}

void CDlgMenu03::MoveEngrave(int nDir)
{
	if (pDoc->WorkingInfo.LastJob.bEngraveOnePnl)
	{
		pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwEngrave, 1);	// ���κ� �ǵ� CW ON (PLC�� �ǵ��Ϸ� �� OFF)
	}
}

void CDlgMenu03::ChkDoneEngrave()
{
	if (pDoc->WorkingInfo.LastJob.bEngraveOnePnl)
	{
		if (!m_bTIM_CHK_DONE_ENGRAVE)
		{
			m_bTIM_CHK_DONE_ENGRAVE = TRUE;
			SetTimer(TIM_CHK_DONE_ENGRAVE, 100, NULL);
		}
	}
}

BOOL CDlgMenu03::IsEngraveFdVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 5);		// ���κ� �ǵ� ���� ����ġ ����
	return bOn;
}

BOOL CDlgMenu03::IsEngraveTqVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 6);		// ���κ� ��ũ ���� ����ġ ����
	return bOn;
}

BOOL CDlgMenu03::IsEngraveTblVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 7);		// ���κ� ���̺� ���� ����ġ ����
	return bOn;
}

BOOL CDlgMenu03::IsEngraveTblBlw()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 2);		// ���κ� ���̺� ��ο� ����ġ ����
	return bOn;
}


void CDlgMenu03::UpdateSignal()
{
	if(pView)
		pView->GetPlcParam();

	Disp();
}


void CDlgMenu03::ChkEngBufHomeDone()
{
	if (!m_bTIM_CHK_DONE_ENG_BUF_HOME)
	{
		m_bTIM_CHK_DONE_ENG_BUF_HOME = TRUE;
		SetTimer(TIM_CHK_DONE_ENG_BUF_HOME, 100, NULL);
	}
}

void CDlgMenu03::ChkEngBufInitDone()
{
	if (!m_bTIM_CHK_DONE_ENG_BUF_INIT)
	{
		m_bTIM_CHK_DONE_ENG_BUF_INIT = TRUE;
		SetTimer(TIM_CHK_DONE_ENG_BUF_INIT, 100, NULL);
	}
}

void CDlgMenu03::SetLed(int nIdx, BOOL bOn)
{
	if (bOn && myLabel[nIdx].GetImageBk() != LBL_IMG_DN)
		myLabel[nIdx].SetImageBk(LBL_IMG_DN);
	else if (!bOn && myLabel[nIdx].GetImageBk() != LBL_IMG_UP)
		myLabel[nIdx].SetImageBk(LBL_IMG_UP);
}

