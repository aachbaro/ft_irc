#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <arpa/inet.h>
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
# include <ctime>
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
		void			handle_command(std::list<Client>::iterator itclient, std::list<pollfd>::iterator it);
		void			add_socket_to_list(std::list<pollfd> *pfds, int filed, short ev, short rev);
		std::string		reply(std::string reply_code, std::string target,std::string msg);
		int				complete_command();
		int				check_nicknames(std::string nick);
		Client			findClient(int fd);

		/* accessors */
		std::string		get_password();
		std::list<Client> get_listClient();
		static void send_to_client(Client client, std::string msg);
		void names(Client client);
		std::string 				generate_reply(std::string code, std::string target, std::string msg);
		void send_prvmsg(std::string target, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, Client client);
		void names(std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, Client client);
		std::list<Client>::iterator find_client_by_nick(std::string nick);
		void oper(std::list<Client>::iterator client, std::string name, std::string password);
		void quit(Client client, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, bool kill);
		void kill(Client client, std::string nickname, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite);
		void kick(Client client, std::string channel_name, std::string client_name, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite);
		void names(Client client, std::string name);
		void list(Client client, std::string channel_name);

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
		std::list<Channel> _channels;
		time_t				start;
		bool				_die;
		std::list<std::list<Client>::iterator>	_toErase;
		std::list<std::list<pollfd>::iterator>	_pfdErase;

		std::vector<std::string>	parse_cmd(std::string command);
		void 						redirect_cmd(std::vector<std::string> parsed, std::list<Client>::iterator itclient, std::string cmd);
		void						clear_args();
		void 						send_welcome_msg(Client client);

		void						join_or_create_channel(std::string name, std::list<Client>::iterator itclient);
		void						pong_reply(std::string to, Client client);
		void		set_or_change_nick(const std::string new_nickname, std::list<Client>::iterator itclient);
		void		invite(std::string channel, std::string client, std::list<Client>::iterator itclient);
		void		mode_cmd(std::string target, std::string param,std::list<Client>::iterator itclient);
		void		mode_cmd(std::string target, std::list<Client>::iterator itclient);
		void topic(Client client, std::string channel_name, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite);
		void send_topic(Client client, std::string channel_name);
		std::vector<std::string> get_join_args(std::string parsed);
		void send_notice(std::string target, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, Client client);
		void part(std::string channel, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite, Client client);
        void    change_nick_in_channels(const std::string old_nick, const std::string new_nick);
		void	print_server_pop(void);
		void	reg_password(std::list<Client>::iterator client, std::string pass);
		void	reg_user(std::list<Client>::iterator client, std::string cmd);
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */