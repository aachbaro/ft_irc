#include "../inc/Server.hpp"

void Server::list(Client client, std::string channel_name) {
    std::list<Channel>::iterator it_channel = this->_channels.begin();
    std::list<Channel>::iterator ite_channel = this->_channels.end();
    for (; it_channel != ite_channel; ++it_channel) {
        if (it_channel->get_name() == channel_name) {
            std::list<Client> chan_clients = it_channel->get_clients();
            std::string nb_of_clients = std::to_string(chan_clients.size());
            send_to_client(client, ":" + address + " 322 " + client.get_nick() + " " + channel_name + " " + nb_of_clients + " :" + it_channel->get_topic() + "\r\n");
            return ;
        }
    }
}
