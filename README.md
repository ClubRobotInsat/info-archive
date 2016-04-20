# info
------------
Installation de tout ce qu'il faut
------------

Alors, qu'est-ce qu'il faut faire pendant les vacances sur le simu ?
Une fois que les matrices sont en place côté graphique, debug la branche simu-matrix. 
Une fois debug on merge simu-matrix dans simu-cartes.

En attendant, dans simu-cartes on peut rajouter les interfaces (dans le dossier robot/module) et les propriétés du robot.
En effet, il faut que chaque carte ping (au minimum). De plus, ça serait pas mal que l'on puisse visionner l'état des variables,
notamment des contacteurs via une interface graphique ou les pompes.
De plus il faudrait rajouter au robot tous les modules nécessaires vis à vis des interactions avec le monde.
Exemple : le robot doit pouvoir ouvrir ses bras implique que l'on possède une variable correspondant à l'état des bras 
dans simulateur/robot/Robot2016
De plus, cette variable doit être accessible via l'IA. C'est à dire, que l'IA envoie un message à une carte virtuelle, 
qui, via le module adéquat viens lire ou écrire la variable dans Robot2016. 
De plus il y a différentes interfaces dans simulateur/robot/cartes/IQUELQUECHOSE (ex Ideplacement) qui correspondent aux interfaces à implémenter.

Bonne chance les gars !
Je ne peut que vous recommandez de regarder le code de l'implémentation des différents modules de l'année dernière !
