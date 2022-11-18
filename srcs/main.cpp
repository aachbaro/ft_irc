#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

#define PORT "6667"

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cout << "Wrong number of arguments" << std::endl;
        return -1;
    }
    Server  irc_serv(av[2], av[1]);

    irc_serv.set_listener_sock();
    irc_serv.poll_loop();
}