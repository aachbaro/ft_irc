#include "../inc/Server.hpp"

void Server::send_prvmsg(std::string target, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, Client client) {
    std::string msg = "";
    if ((*it)[0] == ':')
        msg = (*it).erase(0, 1);
    else
        msg = *it;
    ++it;
    for (; it != ite; ++it) {
        msg += " " + *it;
    }
    std::string to_send = ":" + client.get_nick() + " PRIVMSG "  + target + " " + msg + "\r\n";
    Channel chan;
    if (target[0] == '#') {
        std::list<Channel>::iterator it_channel = _channels.begin();
        std::list<Channel>::iterator ite_channel = _channels.end();
        for (; it_channel != ite_channel; ++it_channel) {
            if (it_channel->get_name() == target) {
                chan = *it_channel;
                break;
            }
        }
        if (it_channel != ite_channel) {
            return chan.send(to_send, client, false);
        }
    }

    std::list<Client>::iterator to = find_client_by_nick(target);
    if (to == clients.end()) {
        send_to_client(client, ":" + address + " 401 " + client.get_nick() + " " + target + " :No such nick/channel\r\n");
        return ;
    }
    Client final_client = *to;
    send_to_client(final_client, to_send);
}
