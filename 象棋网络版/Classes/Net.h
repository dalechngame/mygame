
#ifndef __NET_H__
#define __NET_H__

#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
// in linux, android, ios
#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket close
#define DWORD void *
#define WINAPI

#include <stdlib.h>
#include <pthread.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

class Net
{
public:
	static bool Accept(short port = 9999);
	static bool Connect(const char* ip, short port = 9999);

	// 收发数据的接口
	static int Send(const char* buffer, int len);
	static bool RecvStart();
	static char* RecvData(int& len);

	static bool isConnected();
	static bool isRecvComplete();

	static SOCKET _server;
	static SOCKET _connet;

	static bool _isConnected;
	static bool _isRecvComplete;

	static char* _recvData;

	static DWORD WINAPI AcceptThreadFunc(void* arg);
	static DWORD WINAPI RecvThreadFunc(void* arg);

};

#endif

