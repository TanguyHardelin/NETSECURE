#include <iostream>
#include <QMdiArea>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QProcess>
#include <QAction>
#include <QFile>
#include <cstdlib>

#include "fenetre_principale.h"
#include "fenetre_authentification.h"

using namespace std;

Fenetre_Principal::Fenetre_Principal(){


    //On rajoute la recherche:
    _fen_recherche=new Fenetre_recherche;
    setCentralWidget(_fen_recherche);
    _fen_recherche->show();


    QObject::connect(_fen_recherche,SIGNAL(recherche_finie(QList<QMap<QString,QString> > )),this,SLOT(faire_recherche(QList<QMap<QString,QString> > )));




    //L'icon:
#ifdef linux
    setWindowIcon(QIcon("Images/icon_global.jpg"));
#endif
#ifdef _WIN32
    setWindowIcon(QIcon("Images/icon_global.jpg"));
#endif

    //On rajoute les menus:
    QMenu *menu_fichier= this->menuBar()->addMenu("&Fichier");

    QAction *action_nouvelle_recherche=menu_fichier->addAction("Nouvelle recherche");
    QAction *action_ajouter_mdp=menu_fichier->addAction("Nouveau mot de passe");

    QMenu *menu_securitee= this->menuBar()->addMenu(QString::fromUtf8("Sécuritée"));
    QAction *action_gen_mdp = menu_securitee->addAction(QString::fromUtf8("Genérer mot de passe sûre"));
    QAction *action_gen_clef = menu_securitee->addAction(QString::fromUtf8("Genérer une nouvelle clef de cryptage"));
    QAction *action_exporter_clef = menu_securitee->addAction(QString::fromUtf8("Exporter vos clefs de cryptage"));
    QAction *action_importer_clef = menu_securitee->addAction(QString::fromUtf8("Importer des clefs de cryptage"));

    QObject::connect(action_nouvelle_recherche,SIGNAL(triggered(bool)),this,SLOT(construir_fen_recherche()));
    QObject::connect(action_ajouter_mdp,SIGNAL(triggered(bool)),this,SLOT(construir_fen_nouveau_mdp()));
    QObject::connect(action_gen_mdp,SIGNAL(triggered(bool)),this,SLOT(construir_fen_creer_mdp()));
    QObject::connect(action_gen_clef,SIGNAL(triggered(bool)),this,SLOT(construir_fen_creer_key()));
    QObject::connect(action_exporter_clef,SIGNAL(triggered(bool)),this,SLOT(construir_fen_exporter_key()));
    QObject::connect(action_importer_clef,SIGNAL(triggered(bool)),this,SLOT(construir_fen_importer_key()));
}

void Fenetre_Principal::construir_fen_recherche(){

    //On rajoute la recherche:
    _fen_recherche=new Fenetre_recherche;
    _fen_recherche->show();



    QObject::connect(_fen_recherche,SIGNAL(recherche_finie(QList<QMap<QString,QString> > )),this,SLOT(faire_recherche(QList<QMap<QString,QString> > )));
    QObject::connect(_fen_recherche,SIGNAL(recherche_finie(QList<QMap<QString,QString> > )),_fen_recherche,SLOT(close()));

}
void Fenetre_Principal::construir_fen_creer_key(){
   _fen_generation=new Fenetre_generation_clef_mdp("Clef");
   _fen_generation->show();

}
void Fenetre_Principal::construir_fen_creer_mdp(){
   _fen_generation=new Fenetre_generation_clef_mdp("Mdp");
   setCentralWidget(_fen_generation);
   _fen_generation->show();



}
void Fenetre_Principal::construir_fen_exporter_key(){
    QString clef_a_exporter=QFileDialog::getOpenFileName(this,tr("Choisissez la clef"),QDir::currentPath()+"\\Clefs" , tr("Clefs (*.key)"));
    QString direction=QFileDialog::getExistingDirectory(this,tr("Choisissez la direction"),QDir::homePath());

    #ifdef linux
        QString command="cp "+clef_a_exporter+" "+direction;
        system(command.toStdString().c_str());
    #endif
    #ifdef _WIN32
        QString command="COPY \""+clef_a_exporter.replace('/','\\')+"\" \""+direction.replace('/','\\')+"\"";
        system(command.toStdString().c_str());
    #endif
}
void Fenetre_Principal::construir_fen_importer_key(){
    QString clef_a_importer=QFileDialog::getOpenFileName(this,tr("Choisissez la clef"),QDir::homePath() , tr("Clefs (*.key)"));

    #ifdef linux
        QString command="cp "+clef_a_importer+" "+QDir::currentPath()+"/Clefs";
        system(command.toStdString().c_str());
    #endif
    #ifdef _WIN32
        QString command="COPY \""+clef_a_importer.replace('/','\\')+"\" \""+QDir::currentPath().replace("/","\\")+"\\Clefs\"";
        system(command.toStdString().c_str());
    #endif

}
void Fenetre_Principal::construir_fen_nouveau_mdp(){
   _fen_creer_mdp=new CreerNouveauMdp;
   _fen_creer_mdp->show();

   // QObject::connect( _fen_creer_mdp,SIGNAL(finish()),this,SLOT(clear_windows()));
}

void Fenetre_Principal::faire_recherche(QList<QMap<QString,QString> > infos){
     for(int i=0;i<infos.size();i++){
          qDebug()<<"Log: "<<infos[i]["nom"]<<" -- "<<infos[i]["login"]<<" -- "<<infos[i]["mdp"]<<" -- "<<infos[i]["clef"]<<" -- "<<infos[i]["address"];
          _resultat_recherche=new Resultat_recherche(infos[i]["nom"],infos[i]["login"],infos[i]["mdp"],infos[i]["ip"],infos[i]["address"],infos[i]["autresInformations"],infos[i]["clef"]);
          setCentralWidget(_resultat_recherche);
         _resultat_recherche->show();

         QObject::connect(_resultat_recherche,SIGNAL(changer_informations(QString,QString,QString,QString,QString,QString)),this,SLOT(actualiser_mdp(QString,QString,QString,QString,QString,QString)));


     }
 }

void Fenetre_Principal::actualiser_mdp(QString nom, QString login, QString pwd, QString ip, QString address, QString autresInformations){
    qDebug()<<"Actualisation du mot de passe nom: "<<nom<<" login "<<login<<" pwd "<<pwd<<" ip "<<ip<<" address "<<address<<" autresInformations "<<autresInformations;
   _fen_creer_mdp=new CreerNouveauMdp("update",nom,login,ip,pwd,address,autresInformations);
   _fen_creer_mdp->show();
    QObject::connect( _fen_creer_mdp,SIGNAL(finish()),this,SLOT(clear_windows()));
}
