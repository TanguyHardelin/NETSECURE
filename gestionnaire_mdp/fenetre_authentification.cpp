#include <QString>
#include <string>
#include <fstream>
#include <QFileDialog>
#include <QDebug>

#include "fenetre_authentification.h"
#include "gestion_cryptage.h"
#include "ui_fenetre_authentification.h"

using namespace std;

Fenetre_authentification::Fenetre_authentification(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Fenetre_authentification)
{
    qDebug()<<"authentification";
    ui->setupUi(this);
    setLayout(ui->global);


    //On change le titre et l'image de la fenetre:
    setWindowTitle("Selectionner votre clef de cryptage");
#ifdef linux
    setWindowIcon(QIcon("Images/icon_clef_cryptage.png"));
#endif
#ifdef _WIN32
    //setWindowIcon(QIcon("Images\icon_clef_cryptage.png"));
#endif

    string nom_clef=Gestion_cryptage::getNomClef();
#ifdef linux
    ui->label_clef_courante->setText(QString(nom_clef.c_str()).split("Clefs/")[1]);
#endif
#ifdef _WIN32
   // ui->label_clef_courante->setText(QString(nom_clef.c_str()).split("Clefs\\")[1]);
#endif

    qDebug()<<"pres changement image";

    authentification();
    QObject::connect(ui->bouton_changer_clef,SIGNAL(clicked(bool)),this,SLOT(changer_clef()));
    QObject::connect(ui->bouton_valider,SIGNAL(clicked(bool)),this,SLOT(authentification()));

}


void Fenetre_authentification::changer_clef(){
#ifdef linux
    QString nom_fichier=QFileDialog::getOpenFileName(this,tr("Open Image"),QDir::currentPath()+"/Clefs" , tr("Image Files (*.key)"));
    ui->label_clef_courante->setText(nom_fichier.split(QDir::currentPath()+"/Clefs/")[1]);
#endif
#ifdef _WIN32
    QString nom_fichier=QFileDialog::getOpenFileName(this,tr("Choisissez la clef"),QDir::currentPath()+"\\Clefs" , tr("Image Files (*.key)"));
    ui->label_clef_courante->setText(nom_fichier.split(QDir::currentPath()+"/Clefs/")[1]);
#endif
}


void Fenetre_authentification::authentification(){
    //On change la clef courante:
    ofstream fichier_nom_clef("nom_clef.txt",ios::out);

    if(!fichier_nom_clef){
    }
#ifdef linux
    fichier_nom_clef<<"Clefs/"+ui->label_clef_courante->text().toStdString();
#endif
#ifdef _WIN32
    fichier_nom_clef<<"Clefs\\"+ui->label_clef_courante->text().toStdString();
#endif


    fichier_nom_clef.close();



    emit authentification_complete();
    this->close();

}

Fenetre_authentification::~Fenetre_authentification()
{
    delete ui;
}
