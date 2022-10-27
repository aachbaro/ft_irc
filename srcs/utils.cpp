#include "../inc/Server.hpp"

std::vector<std::string> Server::parse_cmd(std::string command, std::list<Client>::iterator itclient) {
    /*
        Parse command.
        - Put the first word (JOIN, MODE...) in cmd private attribute.
        - Put all the args after in args private attribute.
    */
    std::vector<std::string> parsed;
    size_t pos = 0;
    while((pos = command.find(" ")) != std::string::npos) {
        std::string word = command.substr(0, pos);
        parsed.push_back(word);
        command.erase(0, pos + 1);
    }
    // this->args.push_back(command);
    if (command.length() > 0)
        parsed.push_back(command);
    return parsed;
}

void Server::redirect_cmd(std::vector<std::string> parsed, std::list<Client>::iterator itclient) {
    /* Check what is in cmd private attribute and call the right method. */
    if (parsed.empty())
        return ;
    std::vector<std::string>::iterator first = parsed.begin();
    if (*first == "JOIN") {
        join_or_create_channel(*(first + 1), *(first + 2), itclient);
    }
    if (*first == "PING") {
        pong_reply(*(first + 1), *itclient);
    }
}

void Server::clear_args() {
    this->args.clear();
}
