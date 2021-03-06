// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Query_Module.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent),
	mAutoSaveFlag(false)
{
	memset(mUserName, 0, sizeof(mUserName));
	memset(mPasswd, 0, sizeof(mPasswd));
}

CLoginDlg::~CLoginDlg()
{
}

bool CLoginDlg::GetLoginUserName(wchar_t * str, int & length)
{
	if (wcslen(mUserName) > 0)
	{
		length = wcslen(mUserName);
		memcpy(str, mUserName, sizeof(wchar_t) * length);
		return true;
	}
	return false;
}

bool CLoginDlg::GetLoginPasswd(wchar_t * str, int & length)
{
	if (wcslen(mPasswd) > 0)
	{
		length = wcslen(mPasswd);
		memcpy(str, mPasswd, sizeof(wchar_t) * length);
		return true;
	}
	return false;
}

bool CLoginDlg::GetAutoSaveFlag()
{
	return mAutoSaveFlag;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	memset(mUserName, 0, sizeof(mUserName));
	GetDlgItemText(IDC_EDIT_USERNAME, mUserName, MAX_CHAR_LENGTH);

	
	memset(mPasswd, 0, sizeof(mPasswd));
	GetDlgItemText(IDC_EDIT_PASSWD, mPasswd, MAX_CHAR_LENGTH);

	int ret = ((CButton*)GetDlgItem(IDC_CHECK_SAVE_USERNAME_PASSWD))->GetCheck();
	
	mAutoSaveFlag = (ret == 1);

	CDialogEx::OnOK();
}
