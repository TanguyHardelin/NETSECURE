#include <QSqlQuery>
#include <QDebug>

#include "row_of_list_acces.h"
#include "ui_row_of_list_acces.h"
#include "dialog_log.h"

row_of_list_acces::row_of_list_acces(QString nom_porte,QString id_porte,QString id_utilisateur,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::row_of_list_acces)
{
    ui->setupUi(this);
    this->id_utilisateur=id_utilisateur;
    this->id_porte=id_porte;

    //On set le nom:
    ui->nom_porte->setText(nom_porte);

    //On cherche à savoir si l'utilisateur a actuellement le droit à la porte:
    QSqlQuery query;
    query.exec("SELECT * FROM access WHERE id_porte='"+id_porte+"' and id_utilisateur='"+id_utilisateur+"'");

    int compteur=0;
    while(query.next()){
        compteur++;
    }
    switch(compteur){
        case 0:
            ui->bouton_refuser->setEnabled(false);
            break;
        case 1:
            ui->bouton_autoriser->setEnabled(false);
            break;
        default:
            //IL y a un probleme on ne doit avoir que 1 ou 0
            //On evite d'empirer le probleme et on cache les 2:
            ui->bouton_refuser->setEnabled(false);
            ui->bouton_autoriser->setEnabled(false);
    }

    //Les connects:
    QObject::connect(ui->bouton_autoriser,SIGNAL(clicked()),this,SLOT(autoriser_acces()));
    QObject::connect(ui->bouton_refuser,SIGNAL(clicked()),this,SLOT(supprimer_acces()));
    QObject::connect(ui->bouton_log,SIGNAL(clicked()),this,SLOT(show_log()));

}
void row_of_list_acces::autoriser_acces(){
    ui->bouton_refuser->setEnabled(true);
    ui->bouton_autoriser->setEnabled(false);

    QSqlQuery query;
    query.exec("INSERT INTO access(id_utilisateur,id_porte) VALUES ('"+id_utilisateur+"','"+id_porte+"')");
    while(query.next());
}
void row_of_list_acces::supprimer_acces(){
    ui->bouton_refuser->setEnabled(false);
    ui->bouton_autoriser->setEnabled(true);

    QSqlQuery query;
    query.exec("DELETE FROM access WHERE id_utilisateur='"+id_utilisateur+"' AND id_porte='"+id_porte+"'");
    while(query.next());
}
void row_of_list_acces::show_log(){
   qDebug()<<"Log pour: "+id_porte;
   fenetre_log=new Dialog_log(id_porte);
   fenetre_log->show();

}

row_of_list_acces::~row_of_list_acces()
{
    delete ui;
}
