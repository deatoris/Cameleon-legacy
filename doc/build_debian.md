1.  Installation de l'environnement de construction
VM debian - debian-8.8.0-amd64-xfce-CD-1.iso depuis https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/

Via la commande:
``
sudo apt edit-sources
``

Ajout au source.list[1]:
``
deb http://httpredir.debian.org/debian jessie main contrib
deb-src http://httpredir.debian.org/debian jessie main contrib
deb http://httpredir.debian.org/debian jessie-updates main contrib
deb-src http://httpredir.debian.org/debian jessie-updates main contrib
deb http://security.debian.org/ jessie/updates main contrib
deb-src http://security.debian.org/ jessie/updates main contrib 
``

``
apt-get update
apt-get upgrade
apt-get install -f
``
Environnement de dev principal (gcc/g++/make/qmake ...):
``
apt-get build-essential
apt-get install git
apt-get install qt4-default
``

2.  Construction de l'executable
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

tar -czvf cameleon-deb-vx.y.zk.tar.gz deliverable/instance/

3.  Install
* apt-get build-essential
* apt-get install qt4-default
* tar -xvf cameleon-deb-vx.y.zk.tar.gz
* cd cameleon-deb-vx.y.zk
* ./cameleon.sh

ref:
* [1] https://unix.stackexchange.com/questions/359252/problem-installing-build-essential-in-debian/359327