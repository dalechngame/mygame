#include "Net.h"

SOCKET Net::_server = INVALID_SOCKET;
SOCKET Net::_connet = INVALID_SOCKET;
bool Net::_isConnected = false;
bool Net::_isRecvComplete = false;
char* Net::_recvData = NULL;

bool Net::Accept(short port)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return false;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	int ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	if (ret != 0)
	{
		closesocket(sock);
		return false;
	}

	listen(sock, 10);

	_server = sock;
	_isConnected = false;

#ifdef WIN32
	HANDLE hThread = CreateThread(NULL, 0, AcceptThreadFunc, NULL, 0, NULL);
	CloseHandle(hThread);
#else
	pthread_t tid;
	pthread_create(&tid, NULL, AcceptThreadFunc, NULL);
#endif

	return true;
}

bool Net::Connect(const char* ip, short port)
{
	_connet = socket(AF_INET, SOCK_STREAM, 0);
	if (_connet == INVALID_SOCKET)
		return false;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	int ret = connect(_connet, (struct sockaddr*)&addr, sizeof(addr));
	if (ret != 0)
	{
		closesocket(_connet);
		return false;
	}

	return true;
}

DWORD Net::AcceptThreadFunc(void*)
{
	_connet = accept(_server, NULL, NULL);
	_isConnected = 1;

	return NULL;
}


DWORD Net::RecvThreadFunc(void*)
{
	static char buf[16];
	recv(_connet, buf, 1, 0);
	// 选中
	if (buf[0] == 1)
	{
		recv(_connet, &buf[1], 1, 0);
	}
	// 走棋
	else if (buf[0] == 2)
	{
		for (int i = 1; i <= 3; ++i)
			recv(_connet, &buf[i], 1, 0);
	}

	// recv结束了
	_recvData = buf;
	_isRecvComplete = true;

	return NULL;
}

bool Net::RecvStart()
{
	// recv阻塞式方法
	_isRecvComplete = false;

#ifdef WIN32
	HANDLE hThread = CreateThread(NULL, 0, RecvThreadFunc, NULL, 0, NULL);
	CloseHandle(hThread);
#else
	pthread_t tid;
	pthread_create(&tid, NULL, RecvThreadFunc, NULL);
#endif
	return true;
}

bool Net::isConnected()
{
	return _isConnected;
}

bool Net::isRecvComplete()
{
	return _isRecvComplete;
}

char* Net::RecvData(int& len)
{
	len = 0; // len目前没有用
	_isRecvComplete = false; // 报文被取走，接收状态回到初始状态
	return _recvData;
}

int Net::Send(const char* buffer, int len)
{
	return send(_connet, buffer, len, 0);
}