#include "stdafx.h"
#include "Communication.h"
#include "tinyxml.h"
#include <string.h>

static char *man_do_msg = "<?xml version=\"1.0\" encoding=\"gbk\"?> \n<reply>1</reply>";
static char *man_not_do_msg = "<?xml version=\"1.0\" encoding=\"gbk\"?> \n<reply>0</reply>";

CDataCtrl::CDataCtrl()
{
	Dport = 47005;
}

CDataCtrl::~CDataCtrl()
{

}

void CDataCtrl::DataControl(COPC* dOpc)
{
	//初始化Winscok
	if (!AfxSocketInit())
	{
		AfxMessageBox(TEXT("初始化失败"));
		return;
	}

	//socket----创建2个socket--------------------------------------------
	CSocket aSocket, serverSocket;
	//最好不要使用aSocket.Create创建，因为容易会出现10048错误
	if (!aSocket.Socket())
	{
		AfxMessageBox(TEXT("SOCKET创建失败"));
		return;
	}

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	//设置Socket的选项, 解决10048错误必须的步骤
	aSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);

	//绑定
	if (!aSocket.Bind(Dport))
	{
		AfxMessageBox(TEXT("SOCKET 绑定失败"));
		return;
	}

	//监听
	if (!aSocket.Listen(1))
	{
		AfxMessageBox(TEXT("SOCKET 监听失败"));
		return;
	}
	//接收外部连接
	aSocket.Accept(serverSocket);
	AfxMessageBox(TEXT("连接成功"));
	while(1)
	{
		int rt;
		float data = 0;
		char szRecvMsg[256] = { 0 };
		char szOutMsg[256] = { 0 };
		serverSocket.Receive(szRecvMsg, 256); //接收客户端内容:阻塞
		rt = ParseXmlData(szRecvMsg, &data);
		if (rt == 0)
		{
			float result;
			result = ReadData(dOpc);
			CString str;
			str.Format(_T("%f"), result);
			AfxMessageBox(str);
			if (result == 0) 
			{
				serverSocket.Send(man_not_do_msg, sizeof(man_not_do_msg));//发送内容给客户端
			}
			else 
			{
				serverSocket.Send(man_do_msg, sizeof(man_do_msg));//发送内容给客户端
			}
		}
		else if (rt == 1)
		{
			WriteData(dOpc, data);
		}
		else
		{
			AfxMessageBox(TEXT("XML解析失败"));
		}
	}
	serverSocket.Close();//关闭
	aSocket.Close();
	return;
}


int CDataCtrl::ParseXmlData(char* data, float* num)
{ 
	TiXmlDocument clientMsgXml;
	clientMsgXml.Parse(data);
	if (clientMsgXml.Error())
	{
		AfxMessageBox(TEXT("XML格式错误"));
		return -1;
	}
	TiXmlElement *rootElement = clientMsgXml.RootElement();
	//TiXmlElement *rootElement = doc.RootElement();
	if (rootElement != nullptr)
	{
		if (strncmp(rootElement->Value(), "read", strlen("read")) == 0) 
		{
			AfxMessageBox(TEXT("读数据请求"));
			return 0;
		} 
		else if ((strncmp(rootElement->Value(), "write", strlen("write")) == 0))
		{
			AfxMessageBox(TEXT("写数据请求"));
			const char* value = rootElement->GetText();
			*num = atoi(value);

			return 1;
		}
		else
		{
			AfxMessageBox(TEXT("非法请求"));
			return -1;
		}
	}
	else 
	{
		AfxMessageBox(TEXT("空数据"));
	}
	return 0;
}

bool CDataCtrl::WriteData(COPC* dOpc, float data) 
{
	dOpc->WriteData(1, 0, &COleVariant(data));

	return true;
}

float CDataCtrl::ReadData(COPC* dOpc)
{
	dOpc->ReadData(1, 0);
	float data = dOpc->ItemData[0];

	return data;
}