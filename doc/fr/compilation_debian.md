Installation de l'environnement de construction
-------------------
Installation de l'environnement de construction principal (gcc/g++/make/qmake):
``
apt-get install qt4-default
apt-get install git
``

Pour un environnement de développement ajouter qtcreator via la commande:
``
apt-get install qtcreator
``

Sous debian, si vous n'avez pas accès à ces paquets, vous pouvez configurer les dépôts précisés plus bas.

Construction de l'executable
-------------------
Récupérez les sources:
``
git clone https://framagit.org/ocds/Cameleon.git
``

Suite de commandes de construction pour construire ecm:
``
créer un répertoire build sous Cameleon/
cd Cameleon/build
qmake ../src/ecm.pro
make
cp ecm ../deliverable
``

Lancer ecm dans deliverable

Suite de commandes de construction pour construire lcm:
``
créer un répertoire build-lcm sous Cameleon/
cd Cameleon/build-lcm
qmake ../src/lcm.pro
make
cp lcm ../deliverable
``

Lancer lcm en ligne de commande depuis deliverable


Dépots des packets pour debian
-------------------
Lancer la commande:
``
sudo apt edit-sources
``

Modifier le fichier afin d'ajouter ces dépôts au source.list:
``
deb http://httpredir.debian.org/debian jessie main contrib
deb-src http://httpredir.debian.org/debian jessie main contrib
deb http://httpredir.debian.org/debian jessie-updates main contrib
deb-src http://httpredir.debian.org/debian jessie-updates main contrib
deb http://security.debian.org/ jessie/updates main contrib
deb-src http://security.debian.org/ jessiel/updates main contrib 
``

Maj des sources:
``
apt-get update
apt-get upgrade
apt-get install -f
``
