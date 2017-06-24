Install build environment
-------------------

VM debian - debian-8.8.0-amd64-xfce-CD-1.iso from https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/

Build environment (gcc/g++/make/qmake):
``
apt-get install build-essential
apt-get install git
apt-get install qt4-default libqtwebkit4
``

For a dev environment:
``
apt-get install qtcreator
``

On debian, if you can't get those packages, you can set up new packages repositories (see bellow).

Build the binary
-------------------
Build command lines:
``
git clone https://ocds@framagit.org/ocds/Cameleon.git
cd Cameleon/src
qmake
make
``

Debian packages repositories
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
