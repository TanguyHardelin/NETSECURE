#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>
#include <QDebug>
#include <QProcess>

#include "acceuil.h"
#include "creerauthaurisation.h"
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

    if(argc<2){
        cout<<"Usage:"<<endl;
        cout<<"--rajouter_utilisateur pour rajouter un utilisateur"<<endl;
        cout<<"--management pour manager les access"<<endl;
        return 1;
    }

    string argument(argv[1]);
    CreerAuthaurisation *fen_auth=new CreerAuthaurisation;
    Acceuil w;
    if(argument=="--rajouter_utilisateur"){
        w.ajouter_fen_auth(fen_auth);
        if(ok){
            fen_auth->show();
        }
        else{
            qDebug()<<"Impossible de se connecter à la BDD le driver MYQL est peut etre absent...";
            cout<<"Problème"<<endl;
        }
    }
    else if(argument=="--management"){
        if(ok){
            w.show();
        }
        else{
            qDebug()<<"Impossible de se connecter à la BDD le driver MYQL est peut etre absent...";
            cout<<"Problème"<<endl;
        }
    }

    return a.exec();
}
