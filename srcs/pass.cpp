#include "../inc/Server.hpp"

void    Server::reg_password(std::list<Client>::iterator client, std::string pass)
{
    std::string reply;
    if (client->isRegistered()) {
        reply = ":" + address + " 462 " + client->get_nick() + " :You may not reregister";
        send_to_client(*client, reply);
    }
    else if (pass != password) {
        reply = ":" + address + " 464 " + client->get_nick() + " :Password incorrect";
        send_to_client(*client, reply);
        for (std::list<pollfd>::iterator it_fds = pfds.begin(); it_fds != pfds.end(); ++it_fds) {
        if (it_fds->fd == client->get_fd()) { pfds.erase(it_fds); break ; }
        }
        clients.erase(client);
    }
    else {
        client->set_pass(pass);
    }
}