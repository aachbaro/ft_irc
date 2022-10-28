#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"

Channel::Channel(): _name(""), _clients(), _topic("") {}
Channel::Channel(std::string name): _name(name), _clients(), _topic("") {}
Channel::Channel(std::string name, Client client, std::string topic): _name(name), _topic(topic) {
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

    return *this;
}

std::string Channel::get_name() {
    return this->_name;
}

std::vector<Client> Channel::get_clients() {
    return this->_clients;
}

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

std::string Channel::get_topic() {
    return _topic;
}
