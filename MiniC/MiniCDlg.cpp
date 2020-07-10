﻿
// MiniCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MiniC.h"
#include "MiniCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <fstream>
#include"globals.h"
#include"parse.h"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMiniCDlg 对话框



CMiniCDlg::CMiniCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINIC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAMMA, Gramma);
}

BEGIN_MESSAGE_MAP(CMiniCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CMiniCDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_SAVE, &CMiniCDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SAVEAS, &CMiniCDlg::OnBnClickedSaveas)
	ON_BN_CLICKED(BTN_LEX, &CMiniCDlg::OnBnClickedLex)
	ON_BN_CLICKED(BTN_GRAMMA, &CMiniCDlg::OnBnClickedGramma)
END_MESSAGE_MAP()


// CMiniCDlg 消息处理程序

BOOL CMiniCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMiniCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMiniCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMiniCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMiniCDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CString fpath, str;
	TCHAR sizefilter[] = _T("C file(*.C)|*.C|txt file(*.txt)|*.txt|All file(*.*)|*.*||");
	CFileDialog opendlg(TRUE, _T("C"), NULL, 0, sizefilter, this);
	if (opendlg.DoModal() == IDOK) {
		fpath = opendlg.GetPathName();
		path = fpath;
		CString tempstr = L"";
		CStdioFile file;
		file.Open(fpath, CFile::modeRead);
		while (file.ReadString(tempstr)) {
			tempstr += "\r\n";
			edit += tempstr;
		}
		file.Close();
		UpdateData(false);
	}
	else {
		MessageBox(_T("open file error"));
		return;
	}*/
}


void CMiniCDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (!path.IsEmpty()) {
		ofstream file(path);
		UpdateData(true);
		program.GetWindowTextW(edit);
		for (int i = 0; i < edit.GetLength(); i++)
			if (edit[i] != '\r')
				file.put(edit[i]);
		file.close();
	}
	else {
		SaveAs();
		UpdateData(false);
	}*/
}


void CMiniCDlg::OnBnClickedSaveas()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMiniCDlg::OnBnClickedLex()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CMiniCDlg::OnBnClickedGramma()
{
	// TODO: 在此添加控件通知处理程序代码
	TreeNode* tree = parse();
}
