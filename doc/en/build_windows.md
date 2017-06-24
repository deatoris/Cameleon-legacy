Install build environnement
-------------------

* install mingwn-get then through mingwinget, install mignwin32-make and gdb, 
* install qt4 lib from http://download.qt.io/archive/qt/4.8/4.8.6/,
* install qtcreator from http://download.qt.io/archive/qtcreator/2.5/qt-creator-win-opensource-2.5.2.exe

Configure QT environment
-------------------
* open cameleon.pro with qtcreator.

In qtcreator:
* Set project properity with mingw32-make in C:\MinGW\bin\ for the compiler
* Set project property with gdb in C:\MinGW\bin\ for the debugger

Build
-------------------
Build through qtcreator :
* Build > Run qmake
* Build > Build All
