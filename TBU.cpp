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
void RemoveDir(string& dirPath)  //删除文件夹，输入string字符串类型的相对路径
{
	vector<string> dirFileList;
	getFiles(dirPath, dirFileList);  //读取所有文件
	while (!dirFileList.empty())
	{
		string fileName = dirFileList.back(); //从后往前删除
		DeleteFile(fileName.c_str()); //一个个删除子文件
		dirFileList.pop_back();
	}
	RemoveDirectory(dirPath.c_str()); //删除文件夹，空文件夹才能用这个函数删除
}
void CopyDir(const string& oriPath, const string& destPath) //复制文件夹底下的子文件，同样输入相对路径
{
	vector<string> dirFileList;
	getFiles(oriPath, dirFileList);
	while (!dirFileList.empty())
	{
		string fileName = dirFileList.back(); //从后往前复制
		size_t Position = fileName.find_last_of('/');
		string prefixName = fileName.substr(Position, fileName.size()); //读取子文件的文件名
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
		if(result_tm>=10800){//默认3小时一次 
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
			rmdir_f = "./forge-" + to_string(t_now_int - 259200);//默认删除72h之前的备份 
			const char* mkdir_o_char = mkdir_o.data();
			const char* mkdir_f_char = mkdir_f.data();
			system(mkdir_o_char);
			system(mkdir_f_char);
			CopyDir("原版路径", bdir_o);
			CopyDir("forge端的路径",bdir_f);//记得改名，而且这是相对路径 ，形式为：./*****（同目录） ../（上级） 
		}
	}
    return 0;
}
