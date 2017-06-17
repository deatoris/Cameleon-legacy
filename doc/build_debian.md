Installation de l'environnement de construction
-------------------

VM debian - debian-8.8.0-amd64-xfce-CD-1.iso depuis https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/

Installation de l'environnement de construction principal (gcc/g++/make/qmake ...):
``
apt-get install build-essential
apt-get install git
apt-get install qt4-default libqtwebkit4
``

Pour un environnement de développement ajouter qtcreator via la commande:
``
apt-get install qtcreator
``

Sous debian, si vous n'avez pas accès à ces paquets, vous pouvez configurer les dépôts précisés plus bas.

Construction de l'executable
-------------------
Suite de commandes de construction:
``
mkdir cameleon-src
cd cameleon-src/
git clone https://ocds@framagit.org/ocds/Cameleon.git
``

Dans le répertoire des sources:
``
qmake
make
``

Dépots des packets pour debian
-------------------
Lancer la commande:
``
sudo apt edit-sources
``

Pour ajout au source.list[1]:
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