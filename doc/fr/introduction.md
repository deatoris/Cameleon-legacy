> Cette documentation est en construction et soumise à modification. 

Un peu d'histoire
-------------------
Nous travaillons depuis 2009 au sein de shinoe (https://www.instagram.com/shinoe.lab/) sur un outil en ligne de commande puissant, Caméléon (http://www.shinoe.org/cameleon).

Il permet de lancer des workflows. Il est réputé peu couteux en ressources par rapport à la concurrence. D'autant qu'il offre, en bonus, un éditeur graphique pour composer rapidement de nouvelles solutions, de nouvelles compositions -il a un formalisme déclaratif en XML, un méta-model formel et une compatibilité avec json. Il accélère l'exécution concrète des workflows (100 processus sur un raspberry pi !) et accélère la recomposition de workflows. Il est turing complet et s'appuie sur une théorie forte de l'analyse de la complexité. Caméléon est une approche complémentaire de la complexité.

Cet outil, implémentation du langage Caméléon, a des utilisateurs pointus et ses cas d'usages sont aussi surprenants qu'efficaces. Par exemple, un thésard du laboratoire Navier de l'école nationale des ponts et chaussées (ENPC, 2012)[ref these], aujourd’hui docteur, Jean-François Bruchon, a conduit l'ensemble de sa thèse grâce à Caméléon. Mathématicien non codeur, Jean-François a pris en main l'outil qui a été l'interface principale avec le système pour calibrer ses traitements et analyses scientifiques. Caméléon était aussi son interface principale de discussion avec shinoe qui développait la librairie de traitement d'image Population, base de travail de la thèse de Jean-François: il analysait des images microscopiques de matériaux, minimum 1To par image. Analyse qualitative et quantitative, expérience et calibration faisaient parties de ses activités quotidiennes. Grâce à Caméléon, la calibration était simple et l'isolement des fonctions par Caméléon a contraint à produire des algorithmes isolés réutilisables. Caméléon, et la théorie sur les workflows qui l'accompagne, ont été un driver de la conception des API de Population -déjà en 2012! En ce sens, nous sommes convaincus qu'il est une aide à l'élaboration d'expériences développeurs et utilisateurs agréables et compréhensibles.

Nous pouvons aussi parler d'expériences en captation de données (la meute), en traitement d'image artistique (grapher), en développement photo (plusieurs itérations du processus cette années: voyages, réunions de familles et évènements) et, bientôt, en conformation vidéo 4K. Nous avons 12 cas d'usages explicables en détails. Et l’on peut en imaginer et élaborer d'autres. Il est intéressant de constater, à l'usage, son caractère évolutif -probablement un héritage du fait que Caméléon est un dérivé du réseau de Pétri. Plus on utilise Caméléon, plus il est complet pour nous et pour les autres si nous partageons nos travaux. 

Caméléon a une conception bas niveau. Ainsi, il s'intègre facilement avec n'importe quel langage et n'importe quel système. Il ne prétend pas remplacer les développeurs mais plutôt les augmenter, les compléter exactement. Caméléon aime jouer avec python, R, Go, node.js entre autres! Il aime découvrir de nouveaux écosystèmes. Via API, CLI ou autre, il essaie toujours de se trouver un chemin. En fait, il aime un peu tout le monde Caméléon, il se fond dans l'univers de ses amis ;)

Recherche de processus
-------------------
Caméléon aide les analystes à rechercher, tester et partager des processus de traitement de données.
Il s'agit d'un langage visuel libre sous license MIT[6].

Une synthaxe graphique
-------------------
Complémentaires aux langages écrits, Caméléon est un langage graphique puisant son inspiration dans les résultats de recherche de Pétri[] et de nombreuses tentatives de réaliser un langage graphique[] utile.
Caméléon possède une interface souple, intuitive et addictive. Sa grammaire visuelle peut-être apprise en quelques minutes.

Omni domaines, Omni échelles
-------------------
Caméléon permet de découvrir, modéliser et exécuter des processus métiers. Caméléon possède une approche multi-échelle. Caméléon est adapté pour orchestrer des fonctions disponibles via API, CLI, C++ natif ou encore Python. Il permet d'explorer n'importe quel métier.

Usages
-------------------
Vous êtes un analyste métier? Un architecte solution? Un data scientist? Un data artist? La souplesse de Caméléon ...

* Usage - [Génération d'un site web](./usage_site.md)
* Usage - [Traitement d'image avec imagemagick](./usage_image.md)
* Usage - [Capatation et traitement de données avec twitter et python](./usage_twitter.md)

Plus précisément
-------------------
Caméléon est un langage graphique de programmation fonctionnelle [1]. Il permet de concevoir, développer et tester des processus: flux de travaux[1], flux de contrôles[2] et flux de données[3].

Caméléon est un dérivé de pétri[4]. Caméléon est un langage turing complet[5].

Autrement dit, c'est un langage visuel qui, théoriquement, permet de programmer toute la logique d'un langage textuel classique (logique booléenne, boucles, encapsulation, récurcivité).

Néanmoins, Caméléon ne se substitue pas à la programmation textuelle, il la complète. Il est toujours nécessaire de coder un opérateur. Et il est possible de le faire dans n'importe quel langage.

Il vous permet de fabriquer des traitements et de les calibrer en temps réel. Il rend lisible le besoin et accélère la calibration. 

Fonctionnalités
-------------------
* Léger, rapide et adaptable,
* Extensible avec vos propres fonctions écrites dans le langage écrit de votre choix,
* Editeur graphique efficace et (paraît-il) addictif,
* Orchestration de fonctions,
* Calibration temps réel,
* Librairie standard d'opérateurs pour les types simples (nombres, chaînes de caractères, map, vecteur, tableau),
* Intégration facile via la ligne de commande.

Téléchargement
-------------------
* http://www.shinoe.org/cameleon/release

Références
-------------------
* [1] 
* [2]
* [3]
* [4]
* [5] 
* [6] https://framagit.org/ocds/Cameleon/blob/master/licence.md
