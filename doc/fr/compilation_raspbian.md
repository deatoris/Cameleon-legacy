<code>
apt-get update
apt-get install qt-sdk
qmake cameleon-cli.pro
make
</code>

si rapbian lite, installer le server x + activation du forward TCP:

<code>

sudo nano /etc/ssh/sshd_config
TCPForwarding yes

sudo apt-get install x11-xserver-utils
sudo startx &
cat /var/log/Xorg.1.log
./lcm -cm="project/example/example.cm" 
</code>