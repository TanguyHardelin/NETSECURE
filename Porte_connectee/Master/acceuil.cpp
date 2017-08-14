#include <vector>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPrinter>
#include <QPrintDialog>
#include <QCompleter>
#include <QProcess>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

using namespace std;

#include "acceuil.h"
#include "row_of_list_acces.h"
#include "ui_acceuil.h"

Acceuil::Acceuil(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Acceuil)
{
    ui->setupUi(this);
    nom_utilisateur="";

    fen_nouvelle_authorisation=new CreerAuthaurisation;

    //On change l'icon et le titre:
    setWindowIcon(QIcon("Images/icon_acceuil.png"));
    setWindowTitle("Gestion des accès");

    //Layout:
    actualise_adapteur();
    ui->fenetre_resultat->hide();
    ui->infos_complementaires->hide();


    //Les connects:
    QObject::connect(ui->bouton_rechercher,SIGNAL(clicked(bool)),this,SLOT(faire_recherche()));
    QObject::connect(ui->bouton_valider_changements,SIGNAL(clicked()),this,SLOT(update_BDD()));
    QObject::connect(ui->bouton_supprimer_utilisateur,SIGNAL(clicked()),this,SLOT(supprimer_utilisateur()));

}
void Acceuil::ajouter_fen_auth(CreerAuthaurisation *fen_auth){
    fen_nouvelle_authorisation=fen_auth;
    QObject::connect(fen_nouvelle_authorisation,SIGNAL(finish(QString)),this,SLOT(show()));
    QObject::connect(fen_nouvelle_authorisation,SIGNAL(finish(QString)),this,SLOT(faire_recherche(QString)));
    QObject::connect(fen_nouvelle_authorisation,SIGNAL(finish(QString)),fen_nouvelle_authorisation,SLOT(close()));
}

void Acceuil::actualise_adapteur(){
    get_all_name_utilisateur();

    //On set l'adapteur:
    QCompleter *completer_name=new QCompleter(*_list_name_utilisateur,this);

    completer_name->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

    completer_name->setCaseSensitivity(Qt::CaseSensitive);
    completer_name->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer_name->setCompletionMode(QCompleter::PopupCompletion);
    completer_name->setWrapAround(false);

    ui->edit_nom->setCompleter(completer_name);
}
void Acceuil::supprimer_utilisateur(){
    //On recherche d'abord l'id de l'utilisateur:
    QSqlQuery query;
    query.exec("SELECT id_utilisateur FROM utilisateurs WHERE nom='"+nom_utilisateur+"'");

    int compteur=0;
    QString id_utilisateur;
    while(query.next()){
        id_utilisateur=query.value(0).toString();
        compteur++;
    }
    if(compteur==0 || compteur>1){
        qDebug()<<"Erreur aucun salarié à ce nom ou bien cases multiples, compteur d'occurence:"<<compteur;
        return;
    }

    query.exec("DELETE FROM utilisateurs WHERE nom='"+nom_utilisateur+"'");
    query.exec("DELETE FROM access WHERE id_utilisateur="+id_utilisateur);

    //On retourne à la fenetre de recherche:
    ui->fenetre_resultat->hide();
    ui->infos_complementaires->hide();

    get_all_name_utilisateur();

}


void Acceuil::faire_recherche(QString nom){
    if(nom!=""){
        ui->edit_nom->setText(nom);
        get_all_name_utilisateur();
    }
    //On recherche d'abord l'id de l'utilisateur:
    QSqlQuery query;
    query.exec("SELECT id_utilisateur FROM utilisateurs WHERE nom='"+ui->edit_nom->text()+"'");
    nom_utilisateur=ui->edit_nom->text();

    int compteur=0;
    QString id_utilisateur;
    while(query.next()){
        id_utilisateur=query.value(0).toString();
        compteur++;
    }
    if(compteur==0 || compteur>1){
        qDebug()<<"Erreur aucun salarié à ce nom ou bien cases multiples, compteur d'occurence:"<<compteur;
        return;
    }


    ui->fenetre_resultat->show();
    ui->fenetre_resultat->setTabText(0, ui->edit_nom->text());
    ui->fenetre_resultat->setTabText(1, "Paramètres");
    ui->infos_complementaires->show();


    //On rajoute ces elements au scrollView:
    QWidget *contenu = new QWidget;
    QVBoxLayout *mLayout = new QVBoxLayout();
    contenu->setLayout(mLayout);

    query.exec("SELECT  heure_debut,heure_fin,date_debut,date_fin,numero FROM utilisateurs WHERE nom='"+ui->edit_nom->text()+"'");
    query.next();
    ui->_label_heure_debut->setText(query.value(0).toString());
    ui->_label_heure_fin->setText(query.value(1).toString());


    //On set les dates pickers:
    if(query.value(0).toString()!="NONE" && query.value(1).toString()!="NONE"){
        ui->_heure_debut->setTime(QTime(query.value(0).toString().split(":")[0].toInt(),query.value(0).toString().split(":")[1].toInt()));
        ui->_heure_fin->setTime(QTime(query.value(1).toString().split(":")[0].toInt(),query.value(1).toString().split(":")[1].toInt()));
    }
    ui->_label_date_debut->setText(query.value(2).toString());
    ui->_label_date_fin->setText(query.value(3).toString());
    ui->_edit_numero->setText(query.value(4).toString());
    old_numero=query.value(4).toString();


    //On recherche toute les portes

    query.exec("SELECT * FROM porte");
    while(query.next())
    {
        QHBoxLayout *layout = new QHBoxLayout();
        mLayout->addLayout(layout);

        layout->addWidget(new row_of_list_acces(query.value(1).toString(),query.value(0).toString(),id_utilisateur));
    }
    ui->acces_scroll->setWidget(contenu);

}

void Acceuil::get_all_name_utilisateur(){
    _list_name_utilisateur=new QStringList;
    QSqlQuery query;
    query.exec("SELECT nom FROM utilisateurs");
    while(query.next()){
        _list_name_utilisateur->append(query.value(0).toString());
    }
}
void Acceuil::get_all_name_porte(){
    _list_name_porte=new QStringList;
    QSqlQuery query;
    query.exec("SELECT nom FROM porte");
    while(query.next()){
        _list_name_porte->append(query.value(0).toString());
    }
}
void Acceuil::fermer_fenetre_creation_utilisateur(){
    fen_nouvelle_authorisation->close();
    delete(fen_nouvelle_authorisation);

}

bool Acceuil::update_BDD(){
    bool permition=false;
    bool horraire=false;
    if(ui->widget_plage_horraire->isVisible()){
        horraire=true;
        qDebug()<<"Horraire TRUE";
    }
    if(ui->widget_periode->isVisible()){
        permition=true;
        qDebug()<<"Permition TRUE";
    }
    QSqlQuery query;

    if(permition)
        query.exec("UPDATE utilisateurs SET date_debut='"+ui->_date_debut->text()+"',date_fin='"+ui->_date_fin->text()+"' WHERE nom='"+ui->edit_nom->text()+"'");
    else
        query.exec("UPDATE utilisateurs SET date_debut='NONE',date_fin='NONE' WHERE nom='"+ui->edit_nom->text()+"'");

    if(horraire)
        query.exec("UPDATE utilisateurs SET heure_debut='"+ui->_heure_debut->text()+"',heure_fin='"+ui->_heure_fin->text()+"' WHERE nom='"+ui->edit_nom->text()+"'");
    else
        query.exec("UPDATE utilisateurs SET heure_debut='NONE',heure_fin='NONE' WHERE nom='"+ui->edit_nom->text()+"'");
    if(old_numero!=ui->_edit_numero->text()){
        query.exec("UPDATE utilisateurs SET numero='"+ui->_edit_numero->text()+"' WHERE nom='"+ui->edit_nom->text()+"'");
        qDebug()<<"Numero update: "+ui->_edit_numero->text();

    }

    while(query.next());
    faire_recherche();
    return true;
}

void Acceuil::creerNouvelleAutorisation(){
    /*
    fen_nouvelle_authorisation=new CreerAuthaurisation(*_list_name_utilisateur);
    fen_nouvelle_authorisation->show();
    QObject::connect(fen_nouvelle_authorisation,SIGNAL(finish(QString)),this,SLOT(faire_recherche(QString)));
    QObject::connect(fen_nouvelle_authorisation,SIGNAL(finish(QString)),this,SLOT(fermer_fenetre_creation_utilisateur()));
    */
}
Acceuil::~Acceuil()
{
    delete ui;
}
