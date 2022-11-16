#include "../inc/Server.hpp"

void Server::kill(Client client, std::string nickname, std::vector<std::string> reason) {
    if (client.get_operator() == false) {
        // std::string msg = generate_reply("481", client.get_nick(), "Permission Denied- You're not an IRC operator");
        send_to_client(client, ":" + address + " 481 " + client.get_nick() + " :Permission Denied\r\n");
        return ;
    }
    std::vector<std::string>::iterator  it = reason.begin();
    std::vector<std::string>::iterator  ite = reason.end();
    while (*it != nickname) { it++; }
    it++;
    std::string comment; // = (*it).erase(0, 1);
    //++it;
    for (; it != ite; ++it) {
        comment += " " + *it;
    }
    std::string kill_msg = ":" + client.get_nick() + " KILL " + nickname + " " +  comment + "\r\n";
    std::list<Client>::iterator it_client = find_client_by_nick(nickname);
    if (it_client == clients.end())
        return ;

    send_to_client(*it_client, kill_msg);
    quit(*find_client_by_nick(nickname), reason, true, true);
    // KILL la connexion
}
