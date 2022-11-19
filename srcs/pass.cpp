#include "../inc/Server.hpp"

void    Server::reg_password(std::list<Client>::iterator client, std::string pass)
{
    std::string reply;
    if (client->isRegistered()) {
        reply = ":" + address + " 462 " + client->get_nick() + " :You may not reregister\r\n";
        send_to_client(*client, reply);
    }
    else if (pass != password) {
        reply = ":" + address + " 464 " + client->get_nick() + " :Password incorrect\r\n";
        send_to_client(*client, reply);
        for (std::list<pollfd>::iterator it_fds = pfds.begin(); it_fds != pfds.end(); ++it_fds) {
            if (it_fds->fd == client->get_fd()) {
                _pfdErase.push_back(it_fds);
                break ;
            }
        }
        _toErase.push_back(client);
    }
    else {
        client->set_pass(pass);
    }
}