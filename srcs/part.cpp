#include "../inc/Server.hpp"

void Server::part(std::string channel, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, Client client) {
    std::string msg = "";
    if (it != ite) {
        if ((*it)[0] != ':') {
            send_to_client(client, ":" + address + " 461 " + client.get_nick() + " PART :Syntax Error\r\n");
            return ;
        }
        msg = (*it).erase(0, 1);
        ++it;
        for (; it != ite; ++it) {
            msg += " " + *it;
        }
    }

    std::list<Channel>::iterator it_channel = this->_channels.begin();
    std::list<Channel>::iterator ite_channel = this->_channels.end();
    for (; it_channel != ite_channel; ++it_channel) {
        if (it_channel->get_name() == channel) {
            it_channel->leave_channel(client, msg, address);
            return ;
        }
    }
    send_to_client(client, ":" + address + " 403 " + client.get_nick() + " " + channel + " :No such channel\r\n");
}
