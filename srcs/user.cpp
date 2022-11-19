#include "../inc/Server.hpp"

void    Server::reg_user(std::list<Client>::iterator client, std::string cmd)
{
    if (client->get_pass() == "") { return ; }
    //if (client->get_nick() == "") { return ; }
    if (client->isRegistered()) {
        send_to_client(*client, ":" + address + " 462 " + client->get_nick() + " :You may not reregister\r\n");
    }
	client->set_username(cmd.substr(cmd.find(" ") + 1, (cmd.find(" ", cmd.find(" ") + 1)) - cmd.find(" ") - 1));
	client->set_realname(cmd.substr(cmd.find(":") + 1));
    if (client->get_user().empty()) {
        send_to_client(*client, generate_reply("461", client->get_nick(), "Not enough parameters\r\n"));
    }
    else {
        if (client->get_nick() == "") {return ;}
        send_welcome_msg(*client);
	    std::cout << "pollserver: new connection :" + client->get_nick() << std::endl;
        client->set_registered(true);
    }
}