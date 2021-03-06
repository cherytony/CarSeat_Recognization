#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

/*

解析参数文件config.txt
此文件中的参数不会动态改变

*/

class CParamManager
{
public:
	CParamManager();
	~CParamManager();

	static CParamManager* GetInstance();

	unsigned int GetLocalIP();

	unsigned int GetServerIP();
	unsigned int GetServerPort();
	
	const std::vector<std::wstring> *GetFtpParameter()const;
	const std::vector<std::wstring> *GetMethodType()const;
	const std::vector<std::wstring> *GetSeatType()const;
	const std::vector<std::wstring> *GetLineNo()const;


	std::wstring GetLocalName() { return m_strLocalName; }
	std::wstring GetLoginUserName() { return m_strUsrName; }
	std::wstring GetLoginPasswd() { return m_strPasswd; }

	bool SetLoginUserName(std::wstring tmpUserName);
	bool SetLoginPasswd(std::wstring tmpPasswd);


private:

	static CParamManager *m_pInstance;

	void Init();

	void serialization();
	

	unsigned int __auxLocalIP();

	//自动保存标志
	bool mAutoSaveFlag;

	// serverip， 服务器Ip
	size_t m_nServerIp;

	// serverport 服务器通信端口号
	int m_nServerPort;

	// 本机和服务器通信的Ip
	size_t m_nLocalIp;

	// 本机名
	std::wstring m_strLocalName;

	std::wstring m_strUsrName;
	std::wstring m_strPasswd;

	//ftp用户名密码以及图像的存储路径
	std::vector<std::wstring> *m_pFtp;

	//产线编号
	std::vector<std::wstring> *m_pLineVec;

	//methodType
	std::vector<std::wstring> *m_pMethodType;

	//seatType
	std::vector<std::wstring> *m_pSeatType;
};

