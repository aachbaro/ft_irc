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

void	Server::poll_loop()
{
	while (1)
	{
		std::list<Client>::iterator it = clients.begin();
		std::list<Client>::iterator itend = clients.end();

		while (it != itend)
		{
			std::cout << it->get_nick();
			it++;
		}
		std::cout << "polling fds..." << std::endl;
		polling();
		handle_pfds();
	}
}

void	Server::polling()
{
	this->arr_pfds = (pollfd *)malloc(sizeof(this->arr_pfds) * sizeof(this->pfds.size()));
	std::copy(this->pfds.begin(), this->pfds.end(), this->arr_pfds);
	poll(this->arr_pfds, this->pfds.size(), -1);
	std::copy(this->arr_pfds, this->arr_pfds + this->pfds.size(), this->pfds.begin());
}

void	Server::handle_pfds()
{
	std::list<pollfd>::iterator it;
	std::list<pollfd>::iterator itend;

	itend = this->pfds.end();
	for (it = this->pfds.begin(); it != itend; it++)
	{
		if (it->revents & POLLIN)
		{
			if (it->fd == this->listener)
				this->handle_new_connection();
			else
				this->handle_command(it);
		}
	}
}

void	Server::handle_new_connection()
{
	struct sockaddr_storage	remote_addr;
	socklen_t addr_size = sizeof(remote_addr);
	int new_fd = accept(this->listener, (struct sockaddr *)&remote_addr, &addr_size);
	Client		*new_client = new Client(new_fd);

	new_client->connection();

	if (!this->check_nicknames(new_client->get_nick()))
	{
		if (!this->password.compare(new_client->get_pass()))
		{
			add_socket_to_list(&this->pfds, new_fd, POLLIN, 0);
			this->clients.push_back(*new_client);
			std::string reply = this->reply("001", new_client->get_nick(), "Welcome to the Internet Relay Chat Network " + this->address);
			send(new_client->get_fd(), reply.c_str(), reply.length() + 1, SOCK_STREAM);
			std::cout << "pollserver: new connection :" + new_client->get_nick() << std::endl;
		}
		else
			delete new_client;
	}
	else
	{
		std::string reply = this->reply("433", new_client->get_nick(), "Nickname already in use.");
		send(new_client->get_fd(), reply.c_str(), reply.length() + 1, SOCK_STREAM);
		delete new_client;
	}
	memset(this->buf, 0, 1000);
}

void	Server::handle_command(std::list<pollfd>::iterator it)
{
	int	nbytes = recv(it->fd, buf, 1000, 0);
	std::string	cpy(buf);
	std::cout << "from user: " << it->fd << "\n------CMD PACKET------\n" + cpy  + "\n----------------------" << std::endl;
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
		std::cout << i->get_nick() << std::endl;
		if (!i->get_nick().compare(nick))
			return (1);
		i++;
	}
	return (0);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string		Server::get_password() { return ( this->password ); }
std::list<Client>	Server::get_listClient() { return (this->clients); }

/* ************************************************************************** */