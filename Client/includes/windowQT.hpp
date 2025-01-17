//chaque fenetre de l'appli doit hériter de ça
#ifndef WINDOW_QT
#define WINDOW_QT

#include "mainWindow.hpp"
#include "client.hpp"
#include <QtWidgets>

class MainWindow;

class WindowQT: public QWidget{
    Q_OBJECT
private:
    int intervalle;
protected:
    int id;
    MainWindow *parent;
    Client* client;
    QSignalMapper* signalMapper;
    QTimer* timer;
    QTimer* checkServerTimer;
public:
    WindowQT(int id, MainWindow *parent, Client* cli);
    virtual void initWindow();
    int getId();
    void startTimer();
    void stopTimer();
    void setTimerIntervalle(int t);
    virtual ~WindowQT();
private slots:
    void checkServer();
};

#endif
