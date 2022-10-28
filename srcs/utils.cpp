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
        if (parsed.size() > 2)
            join_or_create_channel(*(first + 1), *(first + 2), itclient);
        else if (parsed.size() > 0)
            join_or_create_channel(*(first + 1), "", itclient);
    }
    if (*first == "PING") {
        pong_reply(*(first + 1), *itclient);
    }
    if (*first == "PRIVMSG") {
        send_prvmsg(*(first + 1), first + 2, parsed.end(), *itclient);
    }
    if (*first == "NICK") {
        if (parsed.size() == 1 || (first + 1)->empty())
        {
            std::string msg = generate_reply("431", "", "No nickname given");
            send_to_client(*itclient, msg);
		    return ;
        }
        set_or_change_nick(*(first + 1), itclient);
    }
}
