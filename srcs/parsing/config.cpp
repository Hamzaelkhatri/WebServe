#include "config.hpp"

int main(int ac, char **ag)
{
    if (ac == 2)
    {
        try
        {
            config _con(ag[1]);
        }
        catch (std::exception &e)
        {
            // std::cerr << "jjjdjdjd" << '\n';
        }
    }
    else
        std::cout << "error : config file not found !" << std::endl;
    return (0);
}