#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"

Channel::Channel(): _name(""), _clients(), _topic(""), _inviteOnly(false) {}
Channel::Channel(std::string name): _name(name), _clients(), _topic(""), _inviteOnly(false) {}
Channel::Channel(std::string name, Client client): _name(name), _topic(""), _chanOperator(client), _inviteOnly(false) {
    /*
        Constructor with client
        - Add client in Channel
    */
    add_client(client);
}
Channel::~Channel() {
    this->_clients.clear();
}

Channel & Channel::operator=(Channel const & rhs) {
    this->_name = rhs._name;;
    this->_clients = rhs._clients;
    _topic = rhs._topic;
    _chanOperator = rhs._chanOperator;

    return *this;
}

std::string Channel::get_name() {
    return this->_name;
}

std::vector<Client> Channel::get_clients() {
    return _clients; }

int Channel::add_client(Client client) {
    /*
        Add client to the channel
        - If client already in channel, return 0
        - If client added, return 1;
    */
    std::vector<Client>::iterator it = _clients.begin();
    std::vector<Client>::iterator ite = _clients.end();

    for (; it != ite; ++it) {
        if (it->get_nick() == client.get_nick())
            return 0;
    }

    this->_clients.push_back(client);
    return 1;
}

void    Channel::del_client_by_nick(std::string nick_name)
{
    std::vector<Client>::iterator   it = _clients.begin();
    std::vector<Client>::iterator   itend = _clients.end();

    while (it != itend)
    {
        if (it->get_nick() == nick_name) {
            if (_chanOperator.get_nick() == nick_name)  { _chanOperator.set_nick(""); }
            std::cout << it->get_nick() << std::endl;
            _clients.erase(it);
            return ;
        }
        it++;
    }
}

void    Channel::change_clients_nick(std::string old_nick, std::string new_nick)
{
    std::vector<Client>::iterator   it = _clients.begin();
    std::vector<Client>::iterator   itend = _clients.end();

    if (_chanOperator.get_nick() == old_nick)  { _chanOperator.set_nick(new_nick); }
    while (it != itend)
    {
        if (it->get_nick() == old_nick) { it->set_nick(new_nick); }
        it++;
    }
}

void Channel::send(std::string msg, Client client, bool send_to_same) {
    std::vector<Client>::iterator it = _clients.begin();
    std::vector<Client>::iterator ite = _clients.end();
    for (; it != ite; ++it) {
        if (!send_to_same && it->get_nick() == client.get_nick())
            continue;
        Server::send_to_client(*it, msg);
    }
}

void  Channel::names(Client client) {
    std::vector<Client>::iterator it = _clients.begin();
    std::vector<Client>::iterator ite = _clients.end();
    std::string clients = "";
    for (; it != ite; ++it) {
        if (it != _clients.begin())
            clients += " ";
        clients += it->get_nick();
    }
    std::string msg = ":127.0.0.1 353 " + client.get_nick() + " = " + this->get_name() + " :" + clients + "\r\n";
    Server::send_to_client(client, msg);
    msg.clear();
    msg = ":127.0.0.1 366 " + client.get_nick() + " " + this->get_name() + " :End of NAMES list\r\n";
    Server::send_to_client(client, msg);
}

bool    Channel::isInInvited(std::string nick) {
    std::vector<std::string>::iterator it = _invited.begin();
    std::vector<std::string>::iterator itend = _invited.end();

    while (it != itend) {
        if (*it == nick) { return (true); }
        it++;
    }
    return (false);
}

std::string Channel::get_topic() { return _topic; }
Client  Channel::get_chanOp() { return (_chanOperator); }
bool    Channel::getMode() { return (_inviteOnly); }
bool    Channel::getProtecTopic() { return (_protectedTopic); }
void    Channel::addInvited(std::string invited) { _invited.push_back(invited); }
void    Channel::setMode(bool mode) { _inviteOnly = mode; }
void    Channel::setProtecTopic(bool mode) { _protectedTopic = mode; }
void    Channel::setTopic(const std::string new_topic) { _topic = new_topic; }

void Channel::leave_channel(Client client, std::string reason, std::string address) {
    std::vector<Client>::iterator instance = find_client(client.get_nick());  
    if (instance == _clients.end()) {
        Server::send_to_client(client, ":" + address + " 442 " +  client.get_nick() + " " + _name + " :You're not on that channel\r\n");
        return ;
    }

    _clients.erase(instance);
    std::string end_msg = " PART " + _name + "\r\n";
    if (reason.length() > 0) {
        end_msg = " PART " + _name + " " + reason + "\r\n";
    }
    for (int i = 0; i < _clients.size(); i++) {
        Server::send_to_client(_clients[i], ":" + client.get_nick() + end_msg);
    }
    Server::send_to_client(client, ":" + client.get_nick() + end_msg);
}

std::vector<Client>::iterator Channel::find_client(std::string nick) {
    std::vector<Client>::iterator it = _clients.begin();
	std::vector<Client>::iterator ite = _clients.end();

	for (; it != ite; ++it) {
		if (it->get_nick() == nick) {
			return it;
        }
	}

	return ite;
}

bool    Channel::actualize(void)
{
    if (_chanOperator.get_nick() == "" && !_clients.empty())
    {
        std::cout << _clients.begin()->get_nick() << std::endl;
        _chanOperator = *_clients.begin();
        return (true);
    }
    else if (_clients.empty()) {return (false); }
    return (true);
}
