#include "../inc/Server.hpp"

void Server::quit(Client client, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, bool kill) {
    std::string reason;
    if ((*it)[0] == ':')
        reason = (*it).erase(0, 1);
    else
        reason = *it;
    if (kill == true) {
        reason += "Killed (" + client.get_nick() + "(";
    }
    ++it;
    for (; it != ite; ++it) {
        reason += " " + *it;
    }
    if (kill == true) {
        reason += "))";
    }
    std::list<Channel>::iterator  itChan = _channels.begin();
    std::list<Channel>::iterator  itChanend = _channels.end();
    while (itChan != itChanend) {
        std::list<Client> chan_clients = itChan->get_clients();
        itChan->send(":" + client.get_nick() + " QUIT :Quit: " + reason + "\r\n", client, false);
        itChan++;
    }
    std::list<Client>::iterator it_client = find_client_by_nick(client.get_nick());
    _toErase.push_back(it_client);
    for (std::list<pollfd>::iterator it_fds = pfds.begin(); it_fds != pfds.end(); ++it_fds) {
        if (it_fds->fd == client.get_fd()) {
            _pfdErase.push_back(it_fds);
            return ;
        }
    }
}
