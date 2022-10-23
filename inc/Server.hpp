#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <iostream>
# include <algorithm>
# include <string>
# include <cstring>
# include <list>
# include <vector>
# include <unistd.h>
# include <fcntl.h>
# include <vector>
# include "Client.hpp"
# include "Channel.hpp"

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
		void			handle_command(std::list<Client>::iterator itclient);
		void			add_socket_to_list(std::list<pollfd> *pfds, int filed, short ev, short rev);
		std::string		reply(std::string reply_code, std::string target,std::string msg);
		int				complete_command();
		int				check_nicknames(std::string nick);
		Client			findClient(int fd);

		/* accessors */
		std::string		get_password();
		std::list<Client> get_listClient();


	private:

		std::string			password;
		std::string			port;
		std::string			address;
		std::list<pollfd>	pfds;
		struct pollfd		*arr_pfds;
		std::string			cmd;
		std::vector<std::string> args;
		int					listener;
		std::list<Client>	clients;
		std::vector<Channel> _channels;

		void		parse_cmd(std::string command, std::list<Client>::iterator itclient);
		void 		redirect_cmd(std::list<Client>::iterator itclient);
		void		join_or_create_channel(std::string name, std::list<Client>::iterator itclient);
		void		clear_args();
		void 		send_to_client(Client client, std::string msg);
		void 		send_welcome_msg(Client client);
		std::string generate_message(std::string code, std::string target, std::string msg);
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */