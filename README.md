# ft_irc


Tu peux tester avec le client irssi, juste a lancer irssi dans le terminal puis tu te connect au serveur avec 
/connect 127.0.0.1 6667 <password> <nickname>

Le serveur est setup et accepte de ouvelle connection, ajoute le nouveau client a son repertoire puis recois les commandes de chacun de ses clients,
Les commandes sont formater avec le mot cle au debut suivit des parametre (ex: "NICK <nickname>" definit le surnom). Le serveur repond ensuite
avec le format construit dans la fonction reply de la classe serveur. 
  Le client envoie ensuite les commandes MODE et PING, auquel il faut creer et associer la fonction correspondante.
Le but va etre ensuite de gerer les differentes commandes envoyer par le client pour effectuer les actions suivante (cf sujet):
- Creer et rejoindre un channel (JOIN)
- envoyer des message prive (PRVMSG)
- Set un utilsateur en operateur (MODE)
- Des commandes specifique qux operqteurs (KICK, DIE etc..)
Une fois les commandes implementees on pourra clore le projet en implementant la gestion d'erreur, et theoriquement on a fini


list des commandes presente sur le github d'un mec qui a fini, y en a pas mal qui sont pas obligatoire mais on verra lesauelles on fait au fur et a mesure:
away
die
info
invite
join
kick
kill
list
lusers
mode
motd
names
nick
notice
oper
part
pass
pingpong
privmsg
quit
rehash
restart
service
servlist
squery
stats
time
topic
user
userhost
version
who
whois
whowas

  
  TACHES
  - ajouter les reponses du serveur au client en cas de join, tel que:
  ![image](https://user-images.githubusercontent.com/71929000/196733919-264eaf03-7617-4ba3-bcc9-6531a56ba8dc.png)
