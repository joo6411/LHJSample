#include "IOCPServer.h"

int main()
{
    IOCPServer* s = new IOCPServer();

    s->Init();
    s->Run();

    delete s;
    return 0;
}