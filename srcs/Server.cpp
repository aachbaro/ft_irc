#include "../inc/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(std::string pword, std::string given_port) : password(pword), port(given_port)
{
	this->address = "127.0.0.1";
}

Server::Server( const Server & src )
{
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Server::poll_loop()
{
	while (1)
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

		/* Scan de la liste de fd  en recherche d'activite*/
		std::cout << "polling fds..." << std::endl;
		polling();
		handle_pfds();
	}
}

void	Server::polling()
{
	/* traduction de la liste chainee de pfds en array pour la fonction poll */
	this->arr_pfds = (pollfd *)malloc(sizeof(this->arr_pfds) * sizeof(this->pfds.size()));
	std::copy(this->pfds.begin(), this->pfds.end(), this->arr_pfds);
	std::cout << "pfds : " << pfds.size() << std::endl;
	poll(this->arr_pfds, this->pfds.size(), -1);
	std::copy(this->arr_pfds, this->arr_pfds + this->pfds.size(), this->pfds.begin());
}

void	Server::handle_pfds()
{
	/* Renvoie vers handle_connection/command en fonction du socket ou poll detecte une activite */
	std::list<pollfd>::iterator it  = this->pfds.begin();
	std::list<Client>::iterator itclient = clients.begin();
	std::list<pollfd>::iterator itend = this->pfds.end();

	for (it ; it != itend; it++)
	{
		if (it->revents & POLLIN)
		{
			if (it->fd == this->listener)
				this->handle_new_connection();
			else
				this->handle_command(itclient);
		}
		if (it != pfds.begin())
			itclient++;
	}
}

void	Server::handle_new_connection()
{
	struct sockaddr_storage	remote_addr;
	socklen_t addr_size = sizeof(remote_addr);
	int new_fd = accept(this->listener, (struct sockaddr *)&remote_addr, &addr_size);
	Client		new_client(new_fd);

	new_client.connection();
	if (!this->check_nicknames(new_client.get_nick()))
	{
		if (!this->password.compare(new_client.get_pass()))
		{
			add_socket_to_list(&this->pfds, new_fd, POLLIN, 0);
			this->clients.push_back(new_client);
			send_welcome_msg(new_client);
			std::cout << "pollserver: new connection :" + new_client.get_nick() << std::endl;
		}
		else
			std::cout << "Wrong password" << std::endl;
	}
	else
	{
		std::string reply = this->reply("433", new_client.get_nick(), "Nickname already in use");
		std::cout << reply << std::endl;
		send(new_client.get_fd(), reply.c_str(), reply.length() + 1, SOCK_STREAM);
	}
}

void	Server::handle_command(std::list<Client>::iterator itclient)
{
	itclient->get_fullcmd();
	std::string cmd = itclient->get_cmd();
	std::cout << "from user: " << itclient->get_nick() << "\n------CMD PACKET------\n" + cmd + "\n----------------------" << std::endl;
	parse_cmd(cmd, itclient);
	redirect_cmd(itclient);
	itclient->get_cmd().clear();
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

	if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
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


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string		Server::get_password() { return ( this->password ); }
std::list<Client>	Server::get_listClient() { return (this->clients); }

/* ************************************************************************** */