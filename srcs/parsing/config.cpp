#include "config.hpp"

int main(int ac, char **ag)
{
    if (ac == 2)
        config _con(ag[1]);
    else
        std::cout << "error : config file not found !" << std::endl;
    return (0);
}