Installation de l'environnement de construction (windows 10)
-------------------

* installation mingwn-get puis, via mingwinget, installation de mignwin32-make et gdb, 
* installation qt4 lib: http://download.qt.io/archive/qt/4.8/4.8.6/,
* installation qtcreator (version 2.5 pour des raisons de gbd python compatible O_o): http://download.qt.io/archive/qtcreator/2.5/qt-creator-win-opensource-2.5.2.exe

Configuration de l'environnement de construction
-------------------

* propriétés du projet avec mingw32-make dans C:\MinGW\bin\ pour le compilateur
* propriétés du projet avec gdb dans C:\MinGW\bin\ pour le debugger

Construction
-------------------

* open cameleon.pro avec QT Creator.
* build via qtcreator

Empaquetage
-------------------

* copy deliverable/instance/* in a delivery zone,
* copy qt*.dll in the bin directory of the delivery zone,
* zip the delivery zone as cameleon-x.y.z.zip

Installation sur un poste client
-------------------

* unpack cameleon-win10-vx.y.zk.zip
* launch cameleon.bat (double click or through CLI)
