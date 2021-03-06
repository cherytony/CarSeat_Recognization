#include "../stdafx.h"
#include "utils.h"
#include "Log.h"



namespace utils
{

	char* WcharToChar(wchar_t* wc)
	{
		if (wc == nullptr)
		{
			return nullptr;
		}
		int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
		char *m_char = new char[len + 1];
		memset(m_char, 0, sizeof(char) * (len + 1));

		WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
		m_char[len] = '\0';
		return m_char;
	}
	wchar_t* CharToWchar(char* c)
	{
		if (c == nullptr)
		{
			return nullptr;
		}
		int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
		wchar_t *m_wchar = new wchar_t[len + 1];
		memset(m_wchar, 0, sizeof(wchar_t) * (len + 1));

		MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
		m_wchar[len] = '\0';
		return m_wchar;
	}

	bool WCharToChar(wchar_t * wc, char * bc, int * length)
	{
		if ((wc == nullptr) || (bc == nullptr) || (length == nullptr))
		{
			return false;
		}
		int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
		if (*length < len)
		{
			return false;
		}
		WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), bc, len, NULL, NULL);
		*length = len;
		bc[len] = '\0';
		return true;
	}

	bool CharToWChar(char * bc, wchar_t * wc, int * length)
	{
		if ((bc == nullptr) || (wc == nullptr) || (length == nullptr))
		{
			return false;
		}
		int len = MultiByteToWideChar(CP_ACP, 0, bc, strlen(bc), NULL, 0);
		if (*length < len)
		{
			return false;
		}
		MultiByteToWideChar(CP_ACP, 0, bc, strlen(bc), wc, len);
		wc[len] = '\0';
		*length = len;
		return true;
	}

	std::wstring StrToWStr(const std::string str)
	{
		//wchar_t* CharToWchar(char* c);
		wchar_t *pWChar = CharToWchar(const_cast<char*>(str.c_str()));
		std::wstring tmpStr(pWChar);
		delete[]pWChar;
		pWChar = nullptr;
		return tmpStr;
	}
	std::string WStrToStr(const std::wstring wstr)
	{
		char *pChar = WcharToChar(const_cast<wchar_t*>(wstr.c_str()));

		std::string tmpStr(pChar);
		delete[]pChar;
		pChar = nullptr;

		return tmpStr;
	}

	bool delBlankSpace(char * text, size_t len)
	{
		if ((text == nullptr) || (len == 0))
		{
			return true;
		}
		size_t i = 0;
		size_t j = 0;
		for (; j < len; )
		{
			if (text[j] > 0x20)
			{
				text[i] = text[j];
				++i;
				++j;
				continue;
			}
			else if (text[j] <= 0x20)
			{
				if ((j > 0) && (text[j - 1] <= 0x20))
				{
					++j;
					continue;
				}
				else if (j == 0)
				{
					++j;
					continue;
				}
				else if ((j > 0) && (text[j - 1] > 0x20))
				{
					text[i] = ' ';
					++i;
					++j;
					continue;
				}
			}
		}
		if (j < len)
		{
			memset(text + j, 0, sizeof(char) * (len - j));
		}
		return true;
	}

	bool readFile(char * file, char * &content, size_t & FileSize)
	{
		FILE *fp = nullptr;
		content = nullptr;
		FileSize = 0;
		fopen_s(&fp, file, "rb");
		if (fp == nullptr)
		{
			return false;
		}
		fseek(fp, 0, SEEK_END);

		size_t nSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		content = new char[nSize + 1];
		if (content == nullptr)
		{
			return false;
		}
		memset(content, 0, sizeof(char) * (nSize + 1));
		fread(content, 1, nSize, fp);
		fclose(fp);

		FileSize = nSize;
		return true;
	}



	bool parseVector(const char *content, const char * name, std::vector<std::wstring>* pVector)
	{
		if ((content == nullptr) || (name == nullptr) || (pVector == nullptr))
		{
			return false;
		}
		char *p = const_cast<char*>(strstr(content, name));
		if (p != NULL)
		{
			char *line = strchr(p, '{');
			char *end = strchr(p, '}');
			if ((line != NULL) && (end != NULL))
			{

				if (parseLineSegment(line + 1, end - line - 1, pVector) == true)
				{



				}
			}
		}
		return true;
	}

	bool parseLineSegment(const char * pContent, size_t length, std::vector<std::wstring>* pData)
	{
		if ((pContent == nullptr) || (length < 1) || (pData == nullptr))
		{
			return false;
		}

		char *p = const_cast<char*>(pContent);
		char tmpStr[100];
		char c = '\"';
		while (p < pContent + length)
		{
			char* begin = strchr(p, c);
			if ((begin == NULL) || (begin >= pContent + length))
			{
				break;
			}
			char* end = strchr(begin + 1, c);
			if ((end == NULL) || (end >= pContent + length))
			{
				break;
			}
			p = end + 1;
			if (end - begin - 1 != 0)
			{
				memset(tmpStr, 0, sizeof(tmpStr));
				memcpy(tmpStr, begin + 1, sizeof(char)*(end - begin - 1));
				wchar_t *wchar = utils::CharToWchar(tmpStr);
				if (wchar == nullptr)
				{
					continue;
				}
				std::wstring wStr(wchar);
				pData->push_back(wStr);
				if (wchar != nullptr)
				{
					delete[]wchar;
					wchar = nullptr;
				}
			}
		}
		return true;
	}

	int parseIp(const char * content, const char * name)
	{
		if ((content == nullptr) || (name == nullptr))
		{
			return 0;
		}
		char *p = const_cast<char*>(strstr(content, name));
		if (p == nullptr)
		{
			return 0;
		}
		const char *quote = strchr(p, '=');
		if (quote == nullptr)
		{
			return 0;
		}
		const char *endline = strchr(quote + 1, '\n');
		char str[100];
		memset(str, 0, sizeof(str));
		memcpy(str, quote + 1, endline - quote - 1);
		TRACE1("ServerIp = %s", str);
		unsigned int one = 0;
		unsigned int two = 0;
		unsigned int three = 0;
		unsigned int four = 0;
		sscanf_s(str, "%d.%d.%d.%d", &one, &two, &three, &four);
		unsigned int tmpServerIp = (one << 24) | (two << 16) | (three << 8) | four;

		return tmpServerIp;
	}

	int parseMap(const char * content, const char * name, std::unordered_map<std::wstring, std::wstring>* pMap)
	{
		if ((content == nullptr) || (name == nullptr) || (pMap == nullptr))
		{
			return 0;
		}
		const char *p = strstr(content, name);
		if (p == nullptr)
		{
			return 0;
		}
		const char *endLine = strchr(p, '}');
		if (endLine == nullptr)
		{
			return 0;
		}
		char tmpStr[50] = { 0 };
		while (p < endLine)
		{
			const char *first = strchr(p, '\"');
			if ((first > endLine) || (first == nullptr))
			{
				return pMap->size();
			}
			const char *second = strchr(first + 1, '\"');
			if ((second > endLine) || (second == nullptr))
			{
				return pMap->size();
			}
			memset(tmpStr, 0, sizeof(tmpStr));
			size_t tmpLength = second - first - 1;
			memcpy(tmpStr, first + 1, tmpLength);
			TRACE1("tmpStr = [%s]\n", tmpStr);
			std::string keyChar(tmpStr);
			std::wstring keyWChar = utils::StrToWStr(keyChar);

			p = second + 1;

			first = strchr(p, '\"');
			if ((first > endLine) || (first == nullptr))
			{
				return pMap->size();
			}
			second = strchr(first + 1, '\"');
			if ((second > endLine) || (second == nullptr))
			{
				return pMap->size();
			}
			memset(tmpStr, 0, sizeof(tmpStr));
			tmpLength = second - first - 1;
			memcpy(tmpStr, first + 1, tmpLength);
			TRACE1("tmpStr = [%s]\n", tmpStr);
			std::string valueChar(tmpStr);
			std::wstring valueWChar = utils::StrToWStr(valueChar);

			pMap->insert(std::make_pair(keyWChar, valueWChar));

			p = second + 1;

		}

		return pMap->size();
	}

	bool getValueByName(const char *content, const char * name, char * value)
	{
		if ((name == nullptr) || (value == nullptr) || (content == nullptr))
		{
			return false;
		}

		char *p = strstr(const_cast<char*>(content), name);
		if (p == NULL)
		{
			return false;
		}
		char *lineEnd = strstr(p, "\n");
		if (lineEnd == NULL)
		{
			return false;
		}
		char *begin = strstr(p + 1, "=");
		if ((begin == NULL) || (begin >= lineEnd))
		{
			return false;
		}
		memcpy_s(value, MAX_CHAR_LENGTH, begin + 1, lineEnd - begin - 1);

		return true;
	}
	int SystemTimeCmp(SYSTEMTIME a, SYSTEMTIME b)
	{
		do
		{
			if (a.wYear > b.wYear)
			{
				return 1;
			}
			else if (a.wYear < b.wYear)
			{
				return -1;
			}
			if (a.wMonth > b.wMonth)
			{
				return 1;
			}
			else if (a.wMonth < b.wMonth)
			{
				return -1;
			}
			if (a.wDay > b.wDay)
			{
				return 1;
			}
			else if (a.wDay < b.wDay)
			{
				return -1;
			}
			if (a.wHour > b.wHour)
			{
				return 1;
			}
			else if (a.wHour < b.wHour)
			{
				return -1;
			}
			if (a.wMinute > b.wMinute)
			{
				return 1;
			}
			else if (a.wMinute < b.wMinute)
			{
				return -1;
			}
			if (a.wSecond > b.wSecond)
			{
				return 1;
			}
			else if (a.wSecond < b.wSecond)
			{
				return -1;
			}
			if (a.wMilliseconds > b.wMilliseconds)
			{
				return 1;
			}
			else if (a.wMilliseconds < b.wMilliseconds)
			{
				return -1;
			}

		} while (0);
		return 0;
	}



	bool sendToServer(unsigned int serverIp, int port, unsigned int localIp, const char *sendMsg, \
		size_t sendMsgLen, char *recvMsg, size_t &recvMsgLen)
	{
		//unsigned int localIp = CParamManager::GetIn->GetLocalIP();
		if ((localIp == 0) || (localIp == -1))
		{
			recvMsgLen = 0;
			return false;
		}

		WSADATA wsaData;
		int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (err != 0)
		{
			err = WSAGetLastError();
			WriteError("err = %u", err);
			recvMsgLen = 0;
			return false;
		}
		SOCKET socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socketFD == -1)
		{
			TRACE("create socket failed\n");
			WSACleanup();
			recvMsgLen = 0;
			return false;
		}

		sockaddr_in addr;

		//����ʱ��
		int nNetTimeout = 1000;
		setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.S_un.S_addr = htonl(serverIp);

		if (0 != connect(socketFD, (sockaddr*)&addr, sizeof(sockaddr)))
		{
			err = WSAGetLastError();
			TRACE1("connect failed,err = %u\n", err);
			WriteError("connect failed, err = %u", err);
			closesocket(socketFD);
			WSACleanup();
			recvMsgLen = 0;
			return false;
		}

		size_t tmpLen = send(socketFD, sendMsg, sendMsgLen, 0);// MSG_DONTROUTE);
		if (tmpLen == SOCKET_ERROR)
		{
			WriteError("send Failed, msg = %s, len = %u, Err:", sendMsg, sendMsgLen, WSAGetLastError());
			closesocket(socketFD);
			WSACleanup();
			recvMsgLen = 0;
			return false;
		}
		tmpLen = recv(socketFD, recvMsg, recvMsgLen, MSG_WAITALL);
		if (SOCKET_ERROR == tmpLen)
		{
			WriteError("recv Failed, Err: %u", GetLastError());
			closesocket(socketFD);
			WSACleanup();
			recvMsgLen = 0;
			return false;
		}
		recvMsgLen = tmpLen;
		closesocket(socketFD);
		socketFD = -1;

		WSACleanup();

		return true;
	}

}
