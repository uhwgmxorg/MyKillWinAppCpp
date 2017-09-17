
// MyKillWinAppCppDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMyKillWinAppCppDlg dialog
class CMyKillWinAppCppDlg : public CDialogEx
{
// Construction
public:
    CMyKillWinAppCppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MYKILLWINAPPCPP_DIALOG };
#endif

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

    BOOL CloseExWindows(CString);
    void FillTheComboBox(CComboBox *cb);
    void RemoveDuplicates(CStringList &lst);
    bool GetProductAndVersion(CStringA & strProductVersion);

public:
    afx_msg void OnBnClickedButtonCloseExternWindow();
    afx_msg void OnBnClickedButtonReload();
    afx_msg void OnCbnSelchangeComboAllWindows();
    afx_msg void OnNMClickSyslinkGithub(NMHDR *pNMHDR, LRESULT *pResult);
    BOOL PreTranslateMessage(MSG * pMsg);

protected:
    CEdit        m_EditWindowTitleToClose;
    CString	     m_WindowTitleToClose;
    CComboBox    m_CBWindowList;
    CButton      m_ButtonCloseExternWindow;
    CButton      m_ButtonReload;
    CToolTipCtrl m_ButtonCloseExternWindowToolTip;
    CToolTipCtrl m_ButtonReloadToolTip;
    CToolTipCtrl m_TextBoxWindowTitleToCloseToolTip;
    CToolTipCtrl m_CBWindowListToolTip;
//    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//    virtual void PreSubclassWindow();
};
