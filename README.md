# ft_irc

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