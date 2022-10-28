#include "../inc/Server.hpp"

void Server::send_prvmsg(std::string target, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, Client client) {
    std::string msg = (*it).erase(0, 1);
    ++it;
    for (; it != ite; ++it) {
        msg += " " + *it;
    }
    std::string to_send = ":" + client.get_nick() + " PRIVMSG "  + target + " " + msg + "\r\n";
    std::cout << "TO SEND: " << to_send << std::endl;
    Channel chan;
    if (target[0] == '#') {
        std::vector<Channel>::iterator it_channel = _channels.begin();
        std::vector<Channel>::iterator ite_channel = _channels.end();
        for (; it_channel != ite_channel; ++it_channel) {
            if (it_channel->get_name() == target) {
                chan = *it_channel;
                break;
            }
        }
        return chan.send(to_send, client, false);
    }

    Client to = find_client_by_nick(target);
    send_to_client(to, to_send);
}
