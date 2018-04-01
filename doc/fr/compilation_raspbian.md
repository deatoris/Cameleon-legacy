Installation de l'environnement de construction
-------------------
<code>
apt-get update
apt-get install qt-sdk
</code>

Pour un environnement de développement ajouter qtcreator via la commande:
``
apt-get install qtcreator
``

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


Forward display: utiliser l'éditeur graphique depuis un serveur distant.
-------------------
si rapbian lite, installer le server x + activation du forward TCP:
<code>
sudo nano /etc/ssh/sshd_config
TCPForwarding yes
sudo apt-get install x11-xserver-utils
sudo startx &
cat /var/log/Xorg.1.log
./ecm project/example/example.cm 
</code>

at reboot:
<code>
  158  sudo startx &
  165  export DISPLAY=:0.0
  166  sudo xhost + &
</code>