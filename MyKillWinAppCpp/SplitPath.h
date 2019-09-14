#pragma once
class CSplitPath
{
public:
    // Construction
    CSplitPath(LPCTSTR lpszPath = NULL);

    // Operations
    BOOL    Split(LPCTSTR lpszPath);
    CString GetPath(void) { return path_buffer; }
    CString GetDrive(void) { return drive; }
    CString GetDirectory(void) { return dir; }
    CString GetFileName(void) { return fname; }
    CString GetExtension(void) { return ext; }

    // Attributes
protected:
    TCHAR path_buffer[_MAX_PATH];
    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];
};

