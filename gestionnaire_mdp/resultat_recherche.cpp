#include <QClipboard>
#include <QDebug>
#include <fstream>
#include <QCloseEvent>
#include <string>

#include "resultat_recherche.h"
#include "gestion_cryptage.h"
#include "ui_resultat_recherche.h"

using namespace std;
bool is_readable(const string &file) //indique si un fichier est lisible (et donc si il existe)
{
  ifstream fichier(file.c_str());
  return !fichier.fail();
}
/*
    Paramètres:
        QString nom: nom de l'appareil.
        QString login: login de l'utilisateur
        QString pwd: le mot de passe de l'utilisateur
        QString port: le port de connection de l'appareil
        QString autresInformations: informations utiles sur l'appareil
        QString clef: clef utilisé pour crypter le mot de passe
        QWidget *parent: parent éventuel.

     Fonction:
        Constructeur de la class Resultat_recherche. Permet l'affichage du mot de passe ainsi que du login.
*/
Resultat_recherche::Resultat_recherche(QString nom,QString login,QString pwd,QString ip,QString address,QString autresInformations,QString clef,QWidget *parent) :
    _nom(nom),_login(login),_pwd(pwd),_ip(ip),_address(address),_autresInformations(autresInformations),
    QWidget(parent),
    ui(new Ui::Resultat_recherche)
{
    ui->setupUi(this);

    //On test si la clef est bonne:
#ifdef linux
    QString nom_clef=QString(Gestion_cryptage::getNomClef().c_str()).split("Clefs/")[1];
#endif

#ifdef _WIN32
        QString nom_clef=QString(Gestion_cryptage::getNomClef().c_str()).split("Clefs\\")[1];
#endif
    qDebug()<<"nom_clef: "<<nom_clef;
    qDebug()<<"clef: "<<clef;
    if(nom_clef!=clef){

        if(is_readable("Clefs\\"+clef.toStdString())){
            //On change la clef courante:
            ofstream fichier_nom_clef("nom_clef.txt",ios::out);

            if(!fichier_nom_clef){
            }
            #ifdef linux
                fichier_nom_clef<<"Clefs/"+clef.toStdString();
            #endif
            #ifdef _WIN32
                fichier_nom_clef<<"Clefs\\"+clef.toStdString();
            #endif


            fichier_nom_clef.close();

            //On ajoute les


        }
        else{
            ui->modif_informations->hide();
            ui->resultat_erreur->setText("/!\\ Fichier d'authorisation absent pour cette recherche");
        }
    }

    //On decrypte le mdp:
    qDebug()<<"MDP CRYPT: "<<_pwd;
    _pwd=QString(Gestion_cryptage::decrypt(pwd.toStdString()).c_str());     //Convertion un peu barbare pour avoir un QString



    setLayout(ui->layout_global);

    //On remplit avec les bonnes valeurs:
    setWindowTitle("Resultat de la recherche pour "+nom.toUtf8());
    ui->edit_login->setText(_login);
    ui->edit_pwd->setText(_pwd);


    //On rajoute le complement de la recherche:
    _complements=new complement_recherche(login,ip,address,autresInformations);
    ui->layout_global->addWidget(_complements);

    //On connect les boutons de copier:
    QObject::connect(ui->bouton_login,SIGNAL(clicked(bool)),this,SLOT(copier_login()));
    QObject::connect(ui->bouton_pwd,SIGNAL(clicked(bool)),this,SLOT(copier_pwd()));
    QObject::connect(ui->modif_informations,SIGNAL(clicked(bool)),this,SLOT(modifier_informations()));

}
/*
    Paramètres:
        aucun

     Retour:
        aucun

     Fonction:
        Copie le login dans le presse papier.
*/
void Resultat_recherche::copier_login(){
    QClipboard *pressePapiers = QApplication::clipboard();
    pressePapiers->setText(ui->edit_login->text());
}
/*
    Paramètres:
        aucun

     Retour:
        aucun

     Fonction:
        Copie le mot de passe dans le presse papier.
*/
void Resultat_recherche::copier_pwd(){
    QClipboard *pressePapiers = QApplication::clipboard();
    pressePapiers->setText(ui->edit_pwd->text());
}
void Resultat_recherche::modifier_informations(){
    emit changer_informations(_nom,_login,_pwd,_ip,_address,_autresInformations);
}
/*
    Paramètres:
        aucun

     Retour:
        aucun

     Fonction:
        Destructeur de la class Resultat_recherche.
*/
Resultat_recherche::~Resultat_recherche()
{
    delete ui;
}
