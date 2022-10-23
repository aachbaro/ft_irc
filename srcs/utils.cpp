#include "../inc/Server.hpp"

void Server::parse_cmd(std::string command, std::list<Client>::iterator itclient) {
    /*
        Parse command.
        - Put the first word (JOIN, MODE...) in cmd private attribute.
        - Put all the args after in args private attribute.
    */
    this->clear_args();
    size_t pos = 0;
    int i = 0;
    while((pos = command.find(" ")) != std::string::npos) {
        std::string word = command.substr(0, pos);
        if (i == 0)
            this->cmd = word;
        else
            this->args.push_back(word);
        command.erase(0, pos + 1);
        i++;
    }
    if (i == 0 && !command.empty())
        this->cmd = command;
    else if (i > 0 && !command.empty())
        this->args.push_back(command);
}

void Server::redirect_cmd(std::list<Client>::iterator itclient) {
    /* Check what is in cmd private attribute and call the right method. */
    if (this->cmd == "JOIN") {
        join_or_create_channel(*(args.begin()), itclient);
    }
    if (this->cmd == "PING") {
        pong_reply(*(args.begin()), *itclient);
    }
}

void Server::clear_args() {
    this->args.clear();
}
