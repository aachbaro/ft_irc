#include "../inc/Server.hpp"

void Server::kill(Client client, std::string nickname, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite) {
    if (client.get_operator() == false) {
        send_to_client(client, ":" + address + " 481 " + client.get_nick() + " :Permission Denied\r\n");
        return ;
    }
    std::string comment = (*it).erase(0, 1);
    ++it;
    for (; it != ite; ++it) {
        comment += " " + *it;
    }
    std::string kill_msg = ":" + client.get_nick() + " KILL " + nickname + " " +  comment + "\r\n";
    std::list<Client>::iterator it_client = find_client_by_nick(nickname);
    if (it_client == clients.end())
        return ;

    send_to_client(*it_client, kill_msg);
    std::vector<std::string> reason_to_quit;
    reason_to_quit.push_back(":");
    reason_to_quit.push_back(comment);
    quit(*find_client_by_nick(nickname), reason_to_quit.begin(), reason_to_quit.end(), true);
}
