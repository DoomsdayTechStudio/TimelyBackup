//using for timely backup files

#include <time.h>
#include <iostream> 
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>
#include <vector>
#include <io.h>

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

int main()
{
	void getFiles(const string& dataPath, vector<string>& files);
	void RemoveDir(string& dirPath);
	void CopyDir(const string& oriPath, const string& destPath);
    time_t timep;
	string rt,wt,t_now,bdir_o,bdir_f,mkdir_o,mkdir_f,rmdir_o,rmdir_f;
	int t_now_int,rt_int,result_tm;
	ifstream read_t("./tlog.log");
	read_t>>rt;
	if(rt==""){
    	wt = to_string(time(&timep));
   		read_t.close();
   		ofstream write_t("./tlog.log");
    	write_t<<wt;
    	write_t.close();
	}
	while(1)
	{
		t_now=to_string(time(&timep));
		istringstream t_now_ss(t_now);
		istringstream rt_ss(rt);
		t_now_ss>>t_now_int;
		rt_ss>>rt_int;
		result_tm=t_now_int-rt_int;
		cout<<result_tm<<endl;
		if(result_tm>=10800){//Ĭ��3Сʱһ�� 
			rt = to_string(time(&timep));
			wt = to_string(time(&timep));
			ofstream write_t("./tlog.log");
			write_t<<wt;
			write_t.close();
			bdir_o = "./origin-" + wt;
			bdir_f = "./forge-" + wt;
			mkdir_o = "mkdir origin-" + wt;
			mkdir_f = " mkdir forge-" + wt;
			rmdir_o = "./origin-" + to_string(t_now_int - 259200);
			rmdir_f = "./forge-" + to_string(t_now_int - 259200);//Ĭ��ɾ��72h֮ǰ�ı��� 
			const char* mkdir_o_char = mkdir_o.data();
			const char* mkdir_f_char = mkdir_f.data();
			system(mkdir_o_char);
			system(mkdir_f_char);
			CopyDir("ԭ��·��", bdir_o);
			CopyDir("forge�˵�·��",bdir_f);//�ǵø����������������·�� ����ʽΪ��./*****��ͬĿ¼�� ../���ϼ��� 
		}
	}
    return 0;
}
