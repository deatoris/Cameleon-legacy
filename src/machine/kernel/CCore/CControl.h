/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CCONTROL_H
#define CCONTROL_H
#include <QtGui>
#include <string>
#include <vector>
#include<COperator.h>
using namespace std;
class CControl : public QWidget
{
    Q_OBJECT
signals:
    void errorMsg(int id, string msg);

public:
    typedef COperatorStructure CControlStructure;
    typedef COperator::Key Key;
    typedef COperator::PlugKey PlugKey;
    CControl(QWidget * parent = 0);
    virtual CControl * clone()=0;
    void setKey(Key type);
    Key getKey();
    void setName(string name);
    string getName();
    void setInformation(string name);
    string getInformation();
    void setPath(vector<string> path);
    vector<string> getPath();
    vector<string> & path();
    CControlStructure& structurePlug();
    void createPlug();

    void error(string msg);
    void connectPlugIn(int myindexplugin, COperator::Id opid, int plugout);
    void deconnectPlugIn(int myindexplugout);

    void connectPlugOut(int myindexplugout, COperator::Id opid, int plugin);
    void deconnectPlugOut(int myindexplugout);

    virtual void apply();

    virtual void updatePlugInControl(int ,CData*  );

    void updatePlugInControlComingFromMachine(int plugkey,CData *data, CPlug::State state);
    void sendPlugOutControl(int indexplugout,CData* data,CPlug::State state);
    bool isPlugOutConnected(int indexplugout);

    int getId();
    virtual string toString();
    virtual void fromString(string );
    QMenu* getMenu();

protected:

    int _id;
    static int _idglobal;
    Key _key;
    string _name;
    string _information;
    vector<string> _path;
    CControlStructure _structure;
    virtual void contextMenuEvent ( QContextMenuEvent *  );
    virtual void dragEnterEvent ( QDragEnterEvent *  );
    virtual void dragLeaveEvent ( QDragLeaveEvent *  );
    virtual void dragMoveEvent ( QDragMoveEvent *  );
    virtual void dropEvent ( QDropEvent *  );
    virtual void enterEvent ( QEvent * event );
    virtual void focusInEvent ( QFocusEvent *  );
    virtual bool focusNextPrevChild ( bool next );
    virtual void focusOutEvent ( QFocusEvent * event );
    virtual void hideEvent ( QHideEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
    virtual void leaveEvent ( QEvent * e );
    virtual void mouseDoubleClickEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void moveEvent ( QMoveEvent * event );
    virtual void paintEvent ( QPaintEvent * event );
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void showEvent ( QShowEvent * event );
    virtual void wheelEvent ( QWheelEvent * event );
    QMenu* menu;
private:
    vector<pair<bool,int> > _v_plug_in;
    vector<pair<bool,pair<int,int> > > _v_plug_in_processor;
    vector<pair<bool,pair<int,int> > > _v_plug_out;
    vector<bool> _v_plug_in_firstime;
};


#endif // CCONTROL_H
