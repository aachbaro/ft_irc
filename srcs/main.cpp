#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

#define PORT "6667"

int main(int ac, char **av)
{
    Server  irc_serv(av[2], av[1]);

/*
    std::list<std::string>  lst;
    lst.push_back("un");
    lst.push_back("deux");
    lst.push_back("trois");
    std::list<std::string>::iterator it = lst.begin();
    std::list<std::string>::iterator itend = lst.end();
    std::list<std::string>::iterator tmp;
    bool    erased(0);
    while (it != itend)
    {
        std::cout << *it << std::endl;
        if (*it == "deux") {
            tmp = it;
            it++;
            erased = 1;
            lst.erase(tmp);
        }
        if (!erased)
            it++;
        erased = 0;
    }
    it = lst.begin();
    while (it != itend)
    {
        std::cout << *it << std::endl;
        it++;
    }
*/
    irc_serv.set_listener_sock();
    irc_serv.poll_loop();
}