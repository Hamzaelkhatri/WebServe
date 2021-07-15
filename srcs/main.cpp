#include "server.hpp"

int main()
{
    // char *str = new char("Hi Hamza");
    // str = "Hi Hamza";
    server _server(8080, "Hello");
    _server._listen();
}