#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <Ws2tcpip.h>

#include <thread>
#include <vector>

#define MAX_SOCKBUF 1024	//��Ŷ ũ��
#define MAX_WORKERTHREAD 4  //������ Ǯ�� ���� ������ ��

enum class IOOperation
{
	RECV,
	SEND
};

//WSAOVERLAPPED����ü�� Ȯ�� ���Ѽ� �ʿ��� ������ �� �־���.
struct stOverlappedEx
{
	WSAOVERLAPPED m_wsaOverlapped;		//Overlapped I/O����ü
	SOCKET		m_socketClient;			//Ŭ���̾�Ʈ ����
	WSABUF		m_wsaBuf;				//Overlapped I/O�۾� ����
	IOOperation m_eOperation;			//�۾� ���� ����
};

//Ŭ���̾�Ʈ ������ ������� ����ü
struct stClientInfo
{
	INT32 mIndex = 0;
	SOCKET			m_socketClient;			//Cliet�� ����Ǵ� ����
	stOverlappedEx	m_stRecvOverlappedEx;	//RECV Overlapped I/O�۾��� ���� ����
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O�۾��� ���� ����

	char			mRecvBuf[MAX_SOCKBUF]; //������ ����
	char			mSendBuf[MAX_SOCKBUF]; //������ ����

	stClientInfo()
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}
};


class IOCPServer
{
public:
	IOCPServer();
	~IOCPServer();

	bool InitSocket();

	//------������ �Լ�-------//
	//������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� 
	//������ ����ϴ� �Լ�
	bool BindandListen(int nBindPort);
	//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
	bool StartServer(const UINT32 maxClientCount);
	//�����Ǿ��ִ� �����带 �ı��Ѵ�.
	void DestroyThread();

	// ��Ʈ��ũ �̺�Ʈ�� ó���� �Լ���
	virtual void OnConnect(const UINT32 clientIndex_) {}
	virtual void OnClose(const UINT32 clientIndex_) {}
	virtual void OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_) {}

private:
	void CreateClient(const UINT32 maxClientCount);

	//WaitingThread Queue���� ����� ��������� ����
	bool CreateWokerThread();

	//accept��û�� ó���ϴ� ������ ����
	bool CreateAccepterThread();

	//������� �ʴ� Ŭ���̾�Ʈ ���� ����ü�� ��ȯ�Ѵ�.
	stClientInfo* GetEmptyClientInfo();

	//CompletionPort��ü�� ���ϰ� CompletionKey�� �����Ű�� ������ �Ѵ�.
	bool BindIOCPServer(stClientInfo* pClientInfo);

	//WSARecv Overlapped I/O �۾��� ��Ų��.
	bool BindRecv(stClientInfo* pClientInfo);

	//WSASend Overlapped I/O�۾��� ��Ų��.
	bool SendMsg(stClientInfo* pClientInfo, char* pMsg, int nLen);

	//Overlapped I/O�۾��� ���� �Ϸ� �뺸�� �޾� 
	//�׿� �ش��ϴ� ó���� �ϴ� �Լ�
	void WokerThread();

	//������� ������ �޴� ������
	void AccepterThread();

	//������ ������ ���� ��Ų��.
	void CloseSocket(stClientInfo* pClientInfo, bool bIsForce = false);

	//Ŭ���̾�Ʈ ���� ���� ����ü
	std::vector<stClientInfo> mClientInfos;

	//Ŭ���̾�Ʈ�� ������ �ޱ����� ���� ����
	SOCKET		mListenSocket = INVALID_SOCKET;

	//���� �Ǿ��ִ� Ŭ���̾�Ʈ ��
	int			mClientCnt = 0;

	//IO Worker ������
	std::vector<std::thread> mIOWorkerThreads;

	//Accept ������
	std::thread	mAccepterThread;

	//CompletionPort��ü �ڵ�
	HANDLE		mIOCPHandle = INVALID_HANDLE_VALUE;

	//�۾� ������ ���� �÷���
	bool		mIsWorkerRun = true;

	//���� ������ ���� �÷���
	bool		mIsAccepterRun = true;
	//���� ����
	char		mSocketBuf[1024] = { 0, };
};