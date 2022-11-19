#include "../inc/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(std::string pword, std::string given_port) : password(pword), port(given_port), start(time(0))
{
	this->address = "127.0.0.1";
	_die = false;
}

Server::Server( const Server & src )
{
	this->password = src.password;
	this->port = src.port;
	this->address = src.address;
	this->pfds = src.pfds;
	this->arr_pfds = src.arr_pfds;
	this->cmd = src.cmd;
	this->args = src.args;
	this->listener = src.listener;
	this->clients = src.clients;
	this->_channels = src._channels;
	this->start = src.start;
	this->_die = src._die;
	this->_toErase = src._toErase;
	this->_pfdErase = src._pfdErase;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	if ( this != &rhs )
	{
		this->password = rhs.password;
		this->port = rhs.port;
		this->address = rhs.address;
		this->pfds = rhs.pfds;
		this->arr_pfds = rhs.arr_pfds;
		this->cmd = rhs.cmd;
		this->args = rhs.args;
		this->listener = rhs.listener;
		this->clients = rhs.clients;
		this->_channels = rhs._channels;
		this->start = rhs.start;
		this->_die = rhs._die;
		this->_toErase = rhs._toErase;
		this->_pfdErase = rhs._pfdErase;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Server::poll_loop()
{
	while (!_die)
	{
		print_server_pop();

		/* Scan de la liste de fd  en recherche d'activite*/
		std::cout << "Polling fds..." << std::endl;
		polling();
		handle_pfds();
	}
}

void	Server::polling()
{
	/* traduction de la liste chainee de pfds en array pour la fonction poll */
	this->arr_pfds = (pollfd *)malloc(sizeof(this->arr_pfds) * sizeof(pfds.size()));
	std::copy(this->pfds.begin(), this->pfds.end(), this->arr_pfds);
	poll(this->arr_pfds, this->pfds.size(), -1);
	std::copy(this->arr_pfds, this->arr_pfds + this->pfds.size(), this->pfds.begin());
	free(arr_pfds);
}

void	Server::handle_pfds()
{
	/* Renvoie vers handle_connection/command en fonction du socket ou poll detecte une activite */
	std::list<pollfd>::iterator it  = this->pfds.begin();
	std::list<Client>::iterator itclient = clients.begin();
	std::list<pollfd>::iterator itend = this->pfds.end();

	for (; it != itend; it++)
	{
		if (it->revents & POLLIN)
		{
			if (it->fd == this->listener)
				this->handle_new_connection();
			else
				this->handle_command(itclient, it);
		}
		if (it != pfds.begin())
			itclient++;
	}
	std::list<std::list<Client>::iterator>::iterator	itit = _toErase.begin();
	std::list<std::list<Client>::iterator>::iterator	ititend = _toErase.end();
	std::list<Channel>::iterator						itChan;
	std::list<Channel>::iterator						itChanend;
	while (itit != ititend)
	{
		itChan = _channels.begin();
		itChanend = _channels.end();
		std::cout << (*itit)->get_nick() << std::endl;
		while (itChan != itChanend) { itChan->del_client_by_nick((*itit)->get_nick()); itChan++; }
		clients.erase(*itit);
		itit++;
	}
	std::list<std::list<pollfd>::iterator>::iterator	itfd = _pfdErase.begin();
	std::list<std::list<pollfd>::iterator>::iterator	itfdend = _pfdErase.end();
	while (itfd != itfdend)
	{
		pfds.erase(*itfd);
		itfd++;
	}
	_toErase.clear();
	_pfdErase.clear();
	itChan = _channels.begin();
	itChanend = _channels.end();
    std::list<Channel>::iterator tmp;
    bool    erased(0);
    while (itChan != itChanend)
    {
        if (itChan->get_clients().empty()) {
            tmp = itChan;
            itChan++;
            erased = 1;
            _channels.erase(tmp);
        }
		else { itChan->set_operator(*itChan->get_clients().begin()); }
        if (!erased)
            itChan++;
        erased = 0;
    }

}

void	Server::handle_new_connection()
{
	struct sockaddr_storage	remote_addr;
	socklen_t addr_size = sizeof(remote_addr);
	int new_fd = accept(this->listener, (struct sockaddr *)&remote_addr, &addr_size);
	getsockname(new_fd, (struct sockaddr*)(&remote_addr), &addr_size);
	struct sockaddr_in * tmp = reinterpret_cast<struct sockaddr_in *> (&remote_addr);
	std::string host = inet_ntoa(tmp->sin_addr);
	Client		new_client(new_fd, host);

	add_socket_to_list(&this->pfds, new_fd, POLLIN, 0);
	this->clients.push_back(new_client);
}

void	Server::handle_command(std::list<Client>::iterator itclient, std::list<pollfd>::iterator it)
{
	memset(itclient->get_buf(), 0, 1000);
	if (recv(itclient->get_fd(), itclient->get_buf(), 1000, 0) <= 0) {
		_pfdErase.push_back(it);
		_toErase.push_back(itclient);
		return ;
	}
	do {
		itclient->clear_cmd();
		if (itclient->complete_command()) { return ; }
		std::string cmd = itclient->get_cmd();
		std::cout << "From user: " << itclient->get_nick() << "\n------CMD PACKET------\n" + cmd + "\n----------------------" << std::endl;
		std::vector<std::string> parsed = parse_cmd(cmd);
		redirect_cmd(parsed, itclient, cmd);
	} while (!itclient->get_cmd().empty());
}

std::string	Server::reply(std::string reply_code, std::string target, std::string msg)
{
	return (":" + this->address + " " + reply_code + " " + target + " :" + msg + "\r\n");
}

int		Server::check_nicknames(std::string nick)
{
	std::list<Client>::iterator i = this->clients.begin();
	while (i != this->clients.end())
	{
		if (!i->get_nick().compare(nick))
			return (1);
		i++;
	}
	return (0);
}

Client	Server::findClient(int fd)
{
	std::list<Client>::iterator i = clients.begin();


	while (i != clients.end())
	{
		if (i->get_fd() == fd)
			return (*i);
	}
	return (*clients.begin());
}


void	Server::set_listener_sock(void)
{
	int listener;
	int yes = 1;
	int status;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;



	if ((status = getaddrinfo(NULL, this->port.c_str(), &hints, &servinfo)) != 0)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		exit(EXIT_FAILURE);
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		listener = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
		if (listener < 0)
		{
			std::cerr << "socket error" << std::endl;
			continue ;
		}
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		fcntl(listener, F_SETFL, O_NONBLOCK);
		if ((bind(listener, servinfo->ai_addr, servinfo->ai_addrlen)) < 0)
		{
			std::cerr << "bind error" << std::endl;
			close(listener);
			continue ;
		}
		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL)
	{
		std::cerr << "Server: failed to bind" << std::endl;
		exit(1);
	}

	if (listen(listener, 5) == -1)
	{
		std::cerr << "listen" << std::endl;
		exit(1);
	}

	this->listener = listener;
	add_socket_to_list(&this->pfds, listener, POLLIN, 0);
}

void	Server::add_socket_to_list(std::list<pollfd> *pfds, int filed, short ev, short rev)
{
	struct pollfd tmp;

	tmp.fd = filed;
	tmp.events = ev;
	tmp.revents = rev;

	pfds->push_back(tmp);
}

std::list<Client>::iterator Server::find_client_by_nick(std::string nick) {
	std::list<Client>::iterator it = clients.begin();
	std::list<Client>::iterator ite = clients.end();

	for (; it != ite; ++it) {
		if (it->get_nick() == nick)
			return it;
	}

	return ite;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string		Server::get_password() { return ( this->password ); }
std::list<Client>	Server::get_listClient() { return (this->clients); }

/* ************************************************************************** */