#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <iostream>
# include <string>
# include <cstring>
# include <list>
# include <unistd.h>
# include <fcntl.h>
# include "Client.hpp"

#define PORT "6667"

class Server
{

	public:

		Server(std::string pword, std::string arg_port);
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );

		void			set_listener_sock(); // set le socket listener
		void			poll_loop();
		void			polling();
		void			handle_pfds();
		void			handle_new_connection();
		void			handle_command(std::list<pollfd>::iterator it);
		void			add_socket_to_list(std::list<pollfd> *pfds, int filed, short ev, short rev);
		std::string		reply(std::string reply_code, std::string target,std::string msg);
		int				complete_command();
		int				check_nicknames(std::string nick);
		
		/* accessors */
		std::string		get_password();
		std::list<Client> get_listClient();


	private:

		std::string			password;
		std::string			port;
		std::string			address;
		std::list<pollfd>	pfds;
		struct pollfd		*arr_pfds;
		char				buf[1000];
		std::string			save;
		std::string			cmd;
		int					listener;
		std::list<Client>	clients;

};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */