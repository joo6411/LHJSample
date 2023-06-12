#include "IOCPClient.h"

int main()
{
    IOCPClient* c = new IOCPClient();

    c->Init();
    c->Run();

    delete c;
    return 0;
}