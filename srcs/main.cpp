#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

#define PORT "6667"

int main(int ac, char **av)
{
    Server  irc_serv(av[2], av[1]);

    std::list<Client>   lst;
    Client              client1(4);
    client1.set_nick("adam");
    Client              client2(3);
    client2.set_nick("pascuans");
    lst.push_back(client1);
    lst.push_back(client2);
    std::list<Client>::iterator it = lst.begin();
    while (it != lst.end())
    {
        std::cout << it->get_nick() << std::endl;
        it++;
    }
 //   irc_serv.set_listener_sock();
 //   irc_serv.poll_loop();
}