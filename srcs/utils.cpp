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
        if (parsed.size() <= 1)
            send_to_client(*itclient, ":" + itclient->get_nick() + " JOIN :Not enough parameters\r\n");
        std::vector<std::string> channels = get_join_args(*(first + 1));
        for (int i = 0; i < channels.size(); i++) {
            join_or_create_channel(channels[i], itclient);
        }
    }
    if (*first == "PART") {
        if (parsed.size() <= 1)
            send_to_client(*itclient, ":" + itclient->get_nick() + " PART :Not enough parameters\r\n");
        std::vector<std::string> channels = get_join_args(*(first + 1));
        for (int i = 0; i < channels.size(); i++) {
            part(channels[i], first + 2, parsed.end(), *itclient);
        }
    }
    if (*first == "PING") {
        pong_reply(*(first + 1), *itclient);
    }
    if (*first == "PRIVMSG") {
        std::vector<std::string> targets = get_join_args(*(first + 1));
        for (int i = 0; i < targets.size(); i++) {
            send_prvmsg(targets[i], first + 2, parsed.end(), *itclient);
        }
    }
    if (*first == "NOTICE") {
        std::vector<std::string> targets = get_join_args(*(first + 1));
        for (int i = 0; i < targets.size(); i++) {
            send_notice(targets[i], first + 2, parsed.end(), *itclient);
        }
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
    if (*first == "MODE")
    {
        if (parsed.size() == 3)
            mode_cmd(*(first + 1), *(first + 2), itclient);
    }
    if (*first == "INVITE")
    {
        if (parsed.size() == 3)
            invite(*(first + 2), *(first + 1), itclient);
    }
    if (*first == "TOPIC")
    {
        if (parsed.size() == 3)
            topic(*(first + 1), *(first + 2), itclient);
        if (parsed.size() >= 2)
            topic(*(first + 1), itclient);
    }
}

void    Server::print_server_pop(void)
{
	    /* Affichage de la liste de clients */
	std::list<Client>::iterator it = clients.begin();
	std::list<Client>::iterator itend = clients.end();
	std::cout << "Clients list: ";
	while (it != itend)
	{
		std::cout << it->get_nick() + " - ";
		it++;
	}
	std::cout << std::endl;
       /* Affichage des channel et de leur population */
    std::vector<Channel>::iterator  itChan = _channels.begin();
    std::vector<Channel>::iterator  itChanend = _channels.end();
    std::vector<Client>::iterator     itCli;
    std::vector<Client>::iterator     itCliend;

    while (itChan != itChanend)
    {
        itCli = itChan->get_clients().begin();
        itCliend = itChan->get_clients().end();
        std::cout << itChan->get_name() + ": ";
        while (itCli != itCliend)
        {
            std::cout << " - " + itCli->get_nick();
            itCli++;
        }
        std::cout << std::endl;
        itChan++;
    }
    std::cout << std::endl;
}