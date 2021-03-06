#pragma once
#include <iostream>
#include <Windows.h>

class CRFIDReader
{
public:
	enum class Status
	{
		RFID_INIT,
		RFID_CONNECT
		
	};

	enum class ErrorType
	{
		ERROR_OK,
		ERROR_SOCKET_INIT,
		ERROR_SOCKET_CREATE,
		ERROR_SOCKET_CONNECT,
		ERROR_SOCKET_CLOSED,
		ERROR_SOCKET_SEND,
		ERROR_SOCKET_RECV,
		ERROR_BUFFER_NULL,
		ERROR_XML_PARSE,
		ERROR_XML_LABEL
		//ERROR_
	};


	CRFIDReader();
	~CRFIDReader();


	/*
	读取条形码接口，其他暂时没有实现
	*/
	std::wstring readBarcode();



private:
	bool init();

	/*
	连接rfid通讯
	*/
	ErrorType initRFID(unsigned int rfidIp, int port);


	// not implement
	ErrorType isConnect(SOCKET fd);

	ErrorType hostGreetings(SOCKET fd);

	ErrorType hostGoodbye(SOCKET fd);

	ErrorType startRead(SOCKET fd);

	ErrorType stopRead(SOCKET fd);

	ErrorType heartBeat(SOCKET fd);


	/*
	此操作会造成和rfid断开连接
	*/
	ErrorType setIPConfig(SOCKET fd, char *Ip, char* Netmask, char *Gateway, bool enableDHCP);

	/*
	
	*/
	ErrorType resetReader(SOCKET fd, char *resetType);

	ErrorType parseReplyPackage(char *buffer, int length, int &id, int &resultCode);

	ErrorType __communicate(SOCKET fd, char *buffer, int length);


	char m_szDefaultIp[16];			// 192.168.0.254
	char m_szDefaultNetmask[16];	// 255.255.255.0
	unsigned short m_nDefaultPort;	// 10001
	char m_szDefaultGatewap[16];	// 192.168.0.1
	bool m_bDefaultDHCP;			////关

	SOCKET m_nSocket;

	int m_nSessionID;
	char m_szVersion[16];
	char m_szReadType[16];
	char m_szReadMode[16];

};

