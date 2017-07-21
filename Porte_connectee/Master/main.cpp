#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>
#include <QDebug>
#include "acceuil.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Connection à la BDD:
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("192.168.3.213");
        db.setDatabaseName("lanestel");
        db.setUserName("root");
        db.setPassword("bba9e4d");
        bool ok = db.open();
    Acceuil w;
    if(ok){
        w.show();
    }
    else{
        qDebug()<<"Impossible de se connecter à la BDD le driver MYQL est peut etre absent...";
        cout<<"Problème"<<endl;
    }





    return a.exec();
}
