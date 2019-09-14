
// MyKillWinAppCppDlg.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"

#include "MyKillWinAppCpp.h"
#include "MyKillWinAppCppDlg.h"
#include "afxdialogex.h"
#include <vector>

#include "SplitPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PROGRAM_MANAGER L"Program Manager"

//------------------------------/
//-     Global functions       -/
//------------------------------/
// this is necessary because 
// EnumWindowsProc has to be 
// declared outside of the class
#pragma region Global functions
struct EnumDataCloseWindow
{
    HWND hWnd;
    CString windowTitleToClose;
};
BOOL CALLBACK EnumWindowsProcForClosing(_In_ HWND   hwnd, _In_ LPARAM lParam)
{
    LPTSTR szWinTitle = new TCHAR[1024];
    DWORD size = 1024;
    EnumDataCloseWindow *windowTitleToClose;

    windowTitleToClose = (EnumDataCloseWindow*)lParam;

    ::GetWindowText(hwnd, szWinTitle, 1024);
    TRACE(L"%s\n", szWinTitle);

    CString strHlp = szWinTitle;
    if (strHlp.Find(windowTitleToClose->windowTitleToClose) >= 0)
    {
        windowTitleToClose->hWnd = hwnd;
        free(szWinTitle);
        return false;
    }

    free(szWinTitle);
    return true;
}

struct EnumDataWindowList
{
    CStringList listOfWindows;
};
BOOL CALLBACK EnumWindowsProcForFillComboBox(_In_ HWND   hwnd, _In_ LPARAM lParam)
{
    LPTSTR szWinTitle = new TCHAR[1024];
    EnumDataWindowList *listOfWindows;

    listOfWindows = (EnumDataWindowList*)lParam;

    ::GetWindowText(hwnd, szWinTitle, 1024);
    TRACE(L"%s\n", szWinTitle);

    CString strHlp = szWinTitle;
    if (!strHlp.IsEmpty() && IsWindowVisible(hwnd))
        (listOfWindows->listOfWindows).AddTail(strHlp);

    free(szWinTitle);
    return true;
}
#pragma endregion

// CMyKillWinAppCppDlg dialog
CMyKillWinAppCppDlg::CMyKillWinAppCppDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_MYKILLWINAPPCPP_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyKillWinAppCppDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMyKillWinAppCppDlg)
    DDX_Text(pDX, IDC_EDIT_WINDOW_TITLE_TO_CLOSE, m_WindowTitleToClose);
    DDX_Control(pDX, IDC_COMBO_ALL_WINDOWS, m_CBWindowList);
    DDX_Control(pDX, IDC_BUTTON_CLOSE_EXTERN_WINDOW, m_ButtonCloseExternWindow);
    DDX_Control(pDX, IDC_BUTTON_RELOAD, m_ButtonReload);
    DDX_Control(pDX, IDC_EDIT_WINDOW_TITLE_TO_CLOSE, m_EditWindowTitleToClose);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyKillWinAppCppDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_CLOSE_EXTERN_WINDOW, &CMyKillWinAppCppDlg::OnBnClickedButtonCloseExternWindow)
    ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CMyKillWinAppCppDlg::OnBnClickedButtonReload)
    ON_CBN_SELCHANGE(IDC_COMBO_ALL_WINDOWS, &CMyKillWinAppCppDlg::OnCbnSelchangeComboAllWindows)
    ON_NOTIFY(NM_CLICK, IDC_SYSLINK_GITHUB, &CMyKillWinAppCppDlg::OnNMClickSyslinkGithub)
END_MESSAGE_MAP()


// CMyKillWinAppCppDlg message handlers

BOOL CMyKillWinAppCppDlg::OnInitDialog()
{
    CString strHelp;
    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // fill the ComboBox
    FillTheComboBox(&m_CBWindowList);

    // init the ToolTips
    strHelp.LoadString(IDS_STRING_TT_TBCTW);
    m_ButtonCloseExternWindowToolTip.Create(this);
    m_ButtonCloseExternWindowToolTip.AddTool(&m_ButtonCloseExternWindow, strHelp);
    m_ButtonCloseExternWindowToolTip.SetMaxTipWidth(SHRT_MAX);               // this is required for line breaks in the ToolTip
    m_ButtonCloseExternWindowToolTip.SetDelayTime(TTDT_AUTOPOP,30 * 1000);   // this sets the time how long the ToolTip is displayed
    m_ButtonCloseExternWindowToolTip.Activate(TRUE);

    strHelp.LoadString(IDS_STRING_TT_RTWL);
    m_ButtonReloadToolTip.Create(this);
    m_ButtonReloadToolTip.AddTool(&m_ButtonReload, strHelp);
    m_ButtonReloadToolTip.Activate(TRUE);

    strHelp.LoadString(IDS_STRING_TT_TITWT);
    m_TextBoxWindowTitleToCloseToolTip.Create(this);
    m_TextBoxWindowTitleToCloseToolTip.AddTool(&m_EditWindowTitleToClose, strHelp);
    m_TextBoxWindowTitleToCloseToolTip.Activate(TRUE);

    strHelp.LoadString(IDS_STRING_TT_LOAWC);
    m_CBWindowListToolTip.Create(this);
    m_CBWindowListToolTip.AddTool(&m_CBWindowList, strHelp);
    m_CBWindowListToolTip.Activate(TRUE);

    // Get .exe name
    TCHAR szExeFileName[MAX_PATH];
    GetModuleFileName(NULL, szExeFileName, MAX_PATH);
    CSplitPath sp(szExeFileName);
    m_AppName = sp.GetFileName();

	// get the windows title
    LPTSTR szWinText = new TCHAR[1024];
    GetWindowTextW(szWinText,1024);
    CString windowText = szWinText;
	// get the Version from the resource
	CStringA versionA;
    GetProductAndVersion(versionA);
    CString version = CA2W(versionA);

	// set the new windows title
#ifdef DEBUG
    windowText = windowText + L"   Debug Version " + version;
#else
    windowText = windowText + L"   Release Version " + version;
#endif // DEBUG

    SetWindowText(windowText);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CMyKillWinAppCppDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyKillWinAppCppDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

//------------------------------/
//-     My Button Events       -/
//------------------------------/
#pragma region My Button Events

void CMyKillWinAppCppDlg::OnBnClickedButtonCloseExternWindow()
{
    CString strHelp;
    CString strMessage;

    UpdateData(true);

    // Error checking
    if (!ComboBoxContains(&m_CBWindowList, m_WindowTitleToClose))
    {
        strMessage.LoadString(IDS_STRING_NOT_WNA);
        strHelp.Format(strMessage,m_WindowTitleToClose);
        MessageBox(strHelp, m_AppName, MB_OK | MB_ICONASTERISK);
        return;
    }
    if (m_WindowTitleToClose.Find(PROGRAM_MANAGER) != -1)
    {
        strMessage.LoadString(IDS_STRING_WARNING_PMANAGER);
        if(MessageBox(strMessage, m_AppName, MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING) == IDNO)
            return;
    }

    while(CloseExWindows(m_WindowTitleToClose));
    FillTheComboBox(&m_CBWindowList);
}

void CMyKillWinAppCppDlg::OnBnClickedButtonReload()
{
    FillTheComboBox(&m_CBWindowList);
}

#pragma endregion

//------------------------------/
//-     My Other Events        -/
//------------------------------/
#pragma region My Other Events

void CMyKillWinAppCppDlg::OnCbnSelchangeComboAllWindows()
{
    CString str;
    m_CBWindowList.GetLBText(m_CBWindowList.GetCurSel(), str);
    m_WindowTitleToClose = str;
    UpdateData(FALSE);
}

BOOL CMyKillWinAppCppDlg::PreTranslateMessage(MSG* pMsg)
{
    m_ButtonCloseExternWindowToolTip.RelayEvent(pMsg);
    m_ButtonReloadToolTip.RelayEvent(pMsg);
    m_TextBoxWindowTitleToCloseToolTip.RelayEvent(pMsg);
    m_CBWindowListToolTip.RelayEvent(pMsg);

    return CDialog::PreTranslateMessage(pMsg);
}

void CMyKillWinAppCppDlg::OnNMClickSyslinkGithub(NMHDR *pNMHDR, LRESULT *pResult)
{

    PNMLINK pNMLink = (PNMLINK)pNMHDR;

    ShellExecuteW(NULL, L"open", L"https://github.com/uhwgmxorg/MyKillWinAppCpp", NULL, NULL, SW_SHOWNORMAL);

    *pResult = 0;
}

#pragma endregion

//------------------------------/
//-    My Other Functions      -/
//------------------------------/
#pragma region My Other Functions

BOOL CMyKillWinAppCppDlg::CloseExWindows(CString windowToClose)
{
    TRACE(L"Enter CloseExWindows()\n");
    TRACE(L"The value of windowToClose is %s\n", windowToClose);

    EnumDataCloseWindow windowTitleToClose;
    windowTitleToClose.windowTitleToClose = m_WindowTitleToClose;

    BOOL rc = ::EnumWindows(EnumWindowsProcForClosing, (LPARAM)&windowTitleToClose);

    if (windowTitleToClose.hWnd)
    {
        DWORD dwProcessId = 0;
        DWORD dwThreadId = GetWindowThreadProcessId(windowTitleToClose.hWnd, &dwProcessId);

        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);

        if (hProcess)
        {
            LPTSTR szExeName = new TCHAR[1024];
            DWORD size = 1024;

            QueryFullProcessImageName(hProcess, 0, szExeName, &size);
            TRACE(L"Find Process %s\n", szExeName);
            BOOL result = TerminateProcess(hProcess, 1);
            TRACE(L"Result is %i\n", result);

            CloseHandle(hProcess);

            return true;
        }
    }

    return false;
}

void CMyKillWinAppCppDlg::FillTheComboBox(CComboBox *cb)
{
    EnumDataWindowList listOfWindows;

    BOOL rc = ::EnumWindows(EnumWindowsProcForFillComboBox, (LPARAM)&listOfWindows);
    RemoveDuplicates(listOfWindows.listOfWindows);

    cb->ResetContent();
    for (POSITION pos = listOfWindows.listOfWindows.GetHeadPosition(); pos != NULL;)
        cb->AddString(listOfWindows.listOfWindows.GetNext(pos));

    cb->SetCurSel(0);
    m_CBWindowList.GetLBText(m_CBWindowList.GetCurSel(), m_WindowTitleToClose);  // set the current selected item in to the CEdit
    UpdateData(FALSE);
}

BOOL CMyKillWinAppCppDlg::ComboBoxContains(CComboBox *cb, CString windowToClose)
{
    if (cb->FindString(0, windowToClose) >= 0)
        return true;
    else
        return false;
}

void CMyKillWinAppCppDlg::RemoveDuplicates(CStringList &lst)
{
    for (POSITION pos = lst.GetHeadPosition(); pos; )
    {
        const CString &strValue = lst.GetNext(pos);

        // check next hits in the list
        for (POSITION pos2 = pos; pos2; )
        {
            // Save current pointer
            POSITION posToDelete = pos2;
            const CString &strValue2 = lst.GetNext(pos2);
            if (strValue == strValue2)
            {
                // remove duplicate
                lst.RemoveAt(posToDelete);

                // There is a chance that we just deleted the follower from the outer loop
                if (posToDelete == pos)
                    pos = pos2;
            }
        }
    }
}

bool CMyKillWinAppCppDlg::GetProductAndVersion(CStringA & strProductVersion)
{
    // get the filename of the executable containing the version resource
    TCHAR szFilename[MAX_PATH + 1] = { 0 };
    if (GetModuleFileName(NULL, szFilename, MAX_PATH) == 0)
    {
        TRACE("GetModuleFileName failed with error %d\n", GetLastError());
        return false;
    }

    // allocate a block of memory for the version info
    DWORD dummy;
    DWORD dwSize = GetFileVersionInfoSize(szFilename, &dummy);
    if (dwSize == 0)
    {
        TRACE("GetFileVersionInfoSize failed with error %d\n", GetLastError());
        return false;
    }
    std::vector<BYTE> data(dwSize);

    // load the version info
    if (!GetFileVersionInfo(szFilename, NULL, dwSize, &data[0]))
    {
        TRACE("GetFileVersionInfo failed with error %d\n", GetLastError());
        return false;
    }

    // get the name and version strings
    LPVOID pvProductName = NULL;
    unsigned int iProductNameLen = 0;
    LPVOID pvProductVersion = NULL;
    unsigned int iProductVersionLen = 0;

    // replace "040904e4" with the language ID of your resources
    std::string string2(data.begin(), data.end());

    UINT                uiVerLen = 0;
    VS_FIXEDFILEINFO*   pFixedInfo = 0;     // pointer to fixed file info structure
                                            // get the fixed file info (language-independent) 
    if (VerQueryValue(&data[0], TEXT("\\"), (void**)&pFixedInfo, (UINT*)&uiVerLen) == 0)
    {
        return false;
    }

    strProductVersion.Format("%u.%u.%u.%u",
        HIWORD(pFixedInfo->dwProductVersionMS),
        LOWORD(pFixedInfo->dwProductVersionMS),
        HIWORD(pFixedInfo->dwProductVersionLS),
        LOWORD(pFixedInfo->dwProductVersionLS));

    return true;
}

#pragma endregion
