#include "server.hpp"

int main()
{
    // char *str = new char("Hi Hamza");
    // str = "Hi Hamza";
    server _server(8080, "Content-type: text/html\n\n <html><title>Hello</title><body>\n Goodbye Cruel World\n </body></html>");
    _server._listen();

    return (0);
}