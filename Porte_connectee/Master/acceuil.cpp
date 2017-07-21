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

using namespace std;

#include "acceuil.h"
#include "row_of_list_acces.h"
#include "ui_acceuil.h"

Acceuil::Acceuil(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Acceuil)
{
    ui->setupUi(this);


    //On change l'icon et le titre:
    setWindowIcon(QIcon("Images/icon_acceuil.png"));
    setWindowTitle("Gestion des accès");

    //Layout:
    setLayout(ui->global);
    actualise_adapteur();
    ui->fenetre_resultat->hide();

    //Les connects:
    QObject::connect(ui->bouton_ajouter,SIGNAL(clicked(bool)),this,SLOT(creerNouvelleAutorisation()));
    QObject::connect(ui->bouton_rechercher,SIGNAL(clicked(bool)),this,SLOT(faire_recherche()));
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


void Acceuil::faire_recherche(){
    //On recherche d'abord l'id de l'utilisateur:
    QSqlQuery query;
    query.exec("SELECT id_utilisateur FROM utilisateurs WHERE nom='"+ui->edit_nom->text()+"'");

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
    ui->label_indication->setText("Gestion des accès pour "+ui->edit_nom->text());


    //On rajoute ces elements au scrollView:
    QWidget *contenu = new QWidget;
    QVBoxLayout *mLayout = new QVBoxLayout();
    contenu->setLayout(mLayout);

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

void Acceuil::creerNouvelleAutorisation(){
    fen_nouvelle_authorisation=new CreerAuthaurisation(*_list_name_utilisateur);
    fen_nouvelle_authorisation->show();
    QObject::connect(fen_nouvelle_authorisation,SIGNAL(clef_finie(QString)),this,SLOT(montrerFenetreQrCode(QString)));
}
void Acceuil::montrerFenetreQrCode(QString clef){
    actualise_adapteur();
    fen_nouvelle_authorisation->close();
    fen_qrcode=new Generation_QrCode("",clef);

    fen_qrcode->show();
}

Acceuil::~Acceuil()
{
    delete ui;
}
