#include "../inc/Server.hpp"

void Server::quit(Client client, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite) {
    std::string reason = (*it).erase(0, 1);
    ++it;
    for (; it != ite; ++it) {
        reason += " " + *it;
    }

    for (int i = 0; i < _channels.size(); i++) {
        std::vector<Client> chan_clients = _channels[i].get_clients();
        std::vector<Client>::iterator it_client = _channels[i].find_client(client.get_nick());
        if (it_client == chan_clients.end())
            continue ;
        _channels[i].send(":" + client.get_nick() + " QUIT :Quit: " + reason + "\r\n", client, false);
        chan_clients.erase(it_client);
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
