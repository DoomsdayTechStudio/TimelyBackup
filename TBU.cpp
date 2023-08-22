//using for timely backup files

#include <iostream> 
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>
#include <windows.h>
#include <vector>
#include <io.h>
#include <shellapi.h>
#include <strsafe.h>
#include "res.h"

#define WM_SHOWTASK (WM_USER+114)

using namespace std;

void getFiles(const string& dataPath, vector<string>& files)
{
	long long hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	int i = 0;
	if ((hFile = _findfirst(p.assign(dataPath).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR)) 
				continue;
			else
				files.push_back(p.assign(dataPath).append("/").append(fileinfo.name));
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void RemoveDir(string& dirPath)  //ɾ���ļ��У�����string�ַ������͵����·��
{
	vector<string> dirFileList;
	getFiles(dirPath, dirFileList);  //��ȡ�����ļ�
	while (!dirFileList.empty())
	{
		string fileName = dirFileList.back(); //�Ӻ���ǰɾ��
		DeleteFile(fileName.c_str()); //һ����ɾ�����ļ�
		dirFileList.pop_back();
	}
	RemoveDirectory(dirPath.c_str()); //ɾ���ļ��У����ļ��в������������ɾ��
}
void CopyDir(const string& oriPath, const string& destPath) //�����ļ��е��µ����ļ���ͬ���������·��
{
	vector<string> dirFileList;
	getFiles(oriPath, dirFileList);
	while (!dirFileList.empty())
	{
		string fileName = dirFileList.back(); //�Ӻ���ǰ����
		size_t Position = fileName.find_last_of('/');
		string prefixName = fileName.substr(Position, fileName.size()); //��ȡ���ļ����ļ���
		CopyFile(fileName.c_str(), (destPath+ prefixName).c_str(),false);
		dirFileList.pop_back();
	}
}

void BallonMsg(int MsgType,HWND hwnd,string INFOTITLETEXT,string INFOTEXT)//����֪ͨ 
{
	HINSTANCE hins = NULL;
	NOTIFYICONDATAA nid = {};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uFlags = NIF_MESSAGE | NIF_GUID | NIF_INFO;
	nid.uCallbackMessage=WM_USER;
	const char* INFOTITLETEXT_char = INFOTITLETEXT.data();
	const char* INFOTEXT_char = INFOTEXT.data();
	StringCchCopyA(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle),INFOTITLETEXT_char);
	StringCchCopyA(nid.szInfo, ARRAYSIZE(nid.szInfo),INFOTEXT_char);
    nid.uTimeout=10000;
	switch(MsgType)
	{
		case 1:{
			nid.dwInfoFlags=NIIF_INFO;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
		case 2:{
			nid.dwInfoFlags=NIIF_WARNING;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
		case 3:{
			nid.dwInfoFlags=NIIF_ERROR;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
		case 4:{
			nid.dwInfoFlags=NIIF_USER;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
		default:{
			nid.dwInfoFlags=NIIF_NONE | NIIF_NOSOUND;
			Shell_NotifyIcon(NIM_MODIFY, &nid);
			break;
		}
	} 
 } 
 
 void TrayWindowIcon(HINSTANCE hInstance,HWND hWnd,string TIPTEXT)//����ͼ�� 
 {
 	HINSTANCE hins = hInstance; 
	NOTIFYICONDATAA nid = {};
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_GUID | NIF_INFO;
	nid.hIcon =LoadIcon(hins, MAKEINTRESOURCE(IDI_NOTIFICATIONICON)); 
	const char* TIPTEXT_char = TIPTEXT.data();
	StringCchCopyA(nid.szTip, ARRAYSIZE(nid.szTip),TIPTEXT_char);
	nid.uCallbackMessage=WM_USER;
	Shell_NotifyIcon(NIM_ADD, &nid);
 }
 
 void TrayIconTipText(HWND hWnd,string TIPTEXT)//��tiptext 
 {
 	NOTIFYICONDATAA nid = {};
 	nid.hWnd = hWnd;
	nid.uFlags = NIF_TIP | NIF_GUID | NIF_INFO;
	const char* TIPTEXT_char = TIPTEXT.data();
	StringCchCopyA(nid.szTip, ARRAYSIZE(nid.szTip),TIPTEXT_char);
	Shell_NotifyIcon(NIM_MODIFY, &nid);
  } 

int main()
{
	HWND console = GetConsoleWindow();
	HINSTANCE hInst= GetModuleHandle (0);
	void getFiles(const string& dataPath, vector<string>& files);
	void RemoveDir(string& dirPath);
	void CopyDir(const string& oriPath, const string& destPath);
	void BallonMsg(int MsgType,HWND hwnd,string INFOTITLETEXT,string INFOTEXT); 
	void TrayWindowIcon(HINSTANCE hInstance,HWND hWnd,string TIPTEXT);
	void TrayIconTipText(HWND hWnd,string TIPTEXT);
	
    time_t timep;
	string rt,wt,t_now,bdir_o,bdir_f,mkdir_o,mkdir_f,rmdir_o,rmdir_f,ballon,ballon_init,tip_content;
	int t_now_int,rt_int,result_tm;
	
	SetWindowPos(console,HWND_BOTTOM,0,0,0,0,SWP_HIDEWINDOW | SWP_NOOWNERZORDER);
	TrayWindowIcon(hInst,console,"�´α���ʱ�䣺δ����");
	ifstream read_t("./tlog.log");
	read_t>>rt;
	if(rt==""){
    	wt = to_string(time(&timep));
   		read_t.close();
   		ofstream write_t("./tlog.log");
    	write_t<<wt;
    	write_t.close();
	}
    ballon_init = "�ϴα���ʱ�䣺" + rt; 
	BallonMsg(0,console,"�����ɹ�",ballon_init);
	while(1)
	{
		t_now=to_string(time(&timep));
		istringstream t_now_ss(t_now);
		istringstream rt_ss(rt);
		t_now_ss>>t_now_int;
		rt_ss>>rt_int;
		result_tm=t_now_int-rt_int;
		tip_content = "�´α���ʱ�䣺" + to_string(10800-result_tm) + "s"; 
		TrayIconTipText(console,tip_content);
		if(result_tm>=10800){//Ĭ��3Сʱһ�� 
			ballon = "ʱ�䣺" + to_string(time(&timep));
			BallonMsg(0,console,"�ѱ���",ballon);
			rt = to_string(time(&timep));
			wt = to_string(time(&timep));
			ofstream write_t("./tlog.log");
			write_t<<wt;
			write_t.close();
			bdir_o = "./origin-" + wt;
			bdir_f = "./forge-" + wt;
			mkdir_o = "mkdir origin-" + wt;
			mkdir_f = "mkdir forge-" + wt;
			rmdir_o = "./origin-" + to_string(t_now_int - 259200);
			rmdir_f = "./forge-" + to_string(t_now_int - 259200);//Ĭ��ɾ��72h֮ǰ�ı��� 
			const char* mkdir_o_char = mkdir_o.data();
			const char* mkdir_f_char = mkdir_f.data();
			system(mkdir_o_char);
			system(mkdir_f_char);
			RemoveDir(rmdir_o);
			RemoveDir(rmdir_f);
			CopyDir("./test", bdir_o);
			CopyDir("./test",bdir_f);//�ǵø����������������·�� ����ʽΪ��./*****��ͬĿ¼�� ../���ϼ��� 
		}
		Sleep(300);
	}
    return 0;
}
