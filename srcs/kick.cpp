#include "../inc/Server.hpp"

void Server::kick(Client client, std::string channel_name, std::string client_name, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite) {
    Channel channel;
    std::list<Channel>::iterator it_channel = _channels.begin();
    std::list<Channel>::iterator ite_channel = _channels.end();
    for (; it_channel != ite_channel; ++it_channel) {
        if (it_channel->get_name() == channel_name) {
            channel = *it_channel;
            break ;
        }
    }
    if (it_channel == ite_channel) {
        send_to_client(client, ":" + address + " 401 " + client.get_nick() + " " + channel_name + " :No such nick/channel\r\n");
        return ;
    }
    if (it_channel->get_chanOp().get_nick() != client.get_nick()) {
        send_to_client(client, ":" + address + " 482 " + client.get_nick() + " " + channel_name + " :You're not channel operator\r\n");
        return ;
    }

    std::list<Client>::iterator it_client =  find_client_by_nick(client_name);
    if (it_client == clients.end()) {
        send_to_client(client, ":" + address + " 401 " + client.get_nick() + " " + channel_name + " :No such nick/channel\r\n");
        return ;
    }
    part(channel_name, it, ite, *it_client);
}