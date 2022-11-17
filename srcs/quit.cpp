#include "../inc/Server.hpp"

void Server::quit(Client client, std::vector<std::string> comment, bool kill, bool theresReason) {
    std::vector<std::string>::iterator  it = comment.begin();
    std::vector<std::string>::iterator  ite = comment.end();

    std::string reason = "";
    if (theresReason)
    {
        if (kill) { while (*it != client.get_nick()) {it++; } }
        else { it++; }
        it++;
        reason = (*it).erase(0, 1);

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
    }
        //std::string reason = "temporary reason";
    std::vector<Channel>::iterator  itChan = _channels.begin();
    std::vector<Channel>::iterator  itChanend = _channels.end();
    while (itChan != itChanend) {
        std::list<Client> chan_clients = itChan->get_clients();
        std::list<Client>::iterator it_client = itChan->find_client(client.get_nick());
        itChan->send(":" + client.get_nick() + " QUIT :Quit: " + reason + "\r\n", client, false);
        //_channels[i].del_client_by_nick(client.get_nick());
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
