Pré-requis
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

Installation de l'environnement de construction principal (gcc/g++/make/qmake ...):
``
apt-get install qt4-default
``

Installation
-------------------

* apt-get build-essential
* apt-get install qt4-default
* tar -xvf cameleon-deb-vx.y.zk.tar.gz
* cd cameleon-deb-vx.y.zk
* ./cameleon.sh
