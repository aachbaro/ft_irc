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
    if (*first == "USER") {
        if (parsed.size() < 4)
            send_to_client(*itclient, ":" + itclient->get_nick() + " USER :Not enough parameters\r\n");
        else {
            send_to_client(*itclient, ":" + address + " 462 " + itclient->get_nick() + " :You may not reregister\r\n");
        }
    }
    if (*first == "JOIN") {
        if (parsed.size() <= 1) {
            send_to_client(*itclient, ":" + itclient->get_nick() + " JOIN :Not enough parameters\r\n");
            return ;
        }
        std::vector<std::string> channels = get_join_args(*(first + 1));
        for (int i = 0; i < channels.size(); i++) {
            join_or_create_channel(channels[i], itclient);
        }
    }
    if (*first == "PART") {
        if (parsed.size() <= 1) {
            send_to_client(*itclient, ":" + itclient->get_nick() + " PART :Not enough parameters\r\n");
            return ;
        }
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
            mode_cmd(*(first + 1), (first + 2)->erase(0, 1), itclient);
        if (parsed.size() == 2) {}

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
    if (*first == "OPER") {
        if (parsed.size() < 3) {
            send_to_client(*itclient, ":" + itclient->get_nick() + " OPER :Not enough parameters\r\n");
            return ;
        }
        else
            oper(itclient, *(first + 1), *(first + 2));
    }
    if (*first == "QUIT") {
        quit(*itclient, first + 1, parsed.end(), false);
    }
    if (*first == "kill") {
        if (parsed.size() < 3) {
            send_to_client(*itclient, ":" + itclient->get_nick() + " KILL :Not enough parameters\r\n");
            return ;
        }
        kill(*itclient, *(first + 1), first + 2, parsed.end());
    }
    if (*first == "die")
    {
        if (itclient->get_operator()) { _die = true; }
        else { send_to_client(*itclient, ":" + address + " 481 " + itclient->get_nick() + " :Permission Denied\r\n"); }
    }
}

void    Server::print_server_pop(void)
{
	    /* Affichage de la liste de clients */
    std::cout << "---------------- SERVER INFO ------------------" << std::endl;
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
    std::list<Channel>::iterator  itChan = _channels.begin();
    std::list<Channel>::iterator  itChanend = _channels.end();
    std::list<Client>::iterator     itCli;
    std::list<Client>::iterator     itCliend;

    while (itChan != itChanend)
    {
        std::list<Client> chan_clients = itChan->get_clients();
        itCli = chan_clients.begin();
        itCliend = chan_clients.end();
        std::cout << "------- " + itChan->get_name() + ": " + "topic :" + itChan->get_topic() << std::endl;
        std::cout << "channel operator: " + itChan->get_chanOp().get_nick() << std::endl;
        while (itCli != itCliend)
        {
            std::cout << " - " + itCli->get_nick();
            itCli++;
        }
        std::cout << std::endl;
        itChan++;
    }
    std::cout << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}