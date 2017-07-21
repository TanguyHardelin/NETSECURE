#include <cstdlib>
#include <ctime>
#include <string>
#include <QProcess>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

#include "creerauthaurisation.h"
#include "ui_creerauthaurisation.h"

using namespace std;

CreerAuthaurisation::CreerAuthaurisation(QStringList const& all_name,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreerAuthaurisation),_list_name(all_name)
{
    ui->setupUi(this);

    //On change le titre:
    setWindowTitle("Ajouter un salarié");




    //On met bien le layout:
    setLayout(ui->global);

    //Les connects:
    QObject::connect(ui->bouton_annuler,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(ui->bouton_valider,SIGNAL(clicked(bool)),this,SLOT(generation_clef()));
}
void CreerAuthaurisation::generation_clef(){
    srand(time(NULL));

    int tailleClef=500+rand()%500;
    string clef="";
    for(int i=0;i<tailleClef;i++){
        bool ok=false;
        unsigned char c;
        do{
            c=63+rand()%63;
            switch (c) {
                case 91:
                    break;
                case 92:
                    break;
                case 93:
                    break;
                case 96:
                    break;
                case 94:
                    break;
                case 95:
                    break;
                case 123:
                    break;
                case 124:
                    break;
                case 125:
                    break;
                case 126:
                    break;
                default:
                    ok=true;
                    break;
            }






        }while(!ok);
        clef+=c;
    }
    _clef=QString(clef.c_str());
    //On enregistre dans la BDD:
    bool status_update=update_BDD();

    if(status_update)
        emit clef_finie(QString(clef.c_str()));
    else
        qDebug()<<"Aucune modif";


}
bool CreerAuthaurisation::update_BDD(){
    if(_list_name.contains(ui->edit_nom_prenom->text())){
        //Afficher: "ce nom existe déjà"
        qDebug()<<"Ce nom est déjà pris !";
        ui->label_error->setText("Ce nom existe déjà dans la base de donnée. Allez dans \"Rechercher utilisateur\" pour editer ses permitions");
        return false;
    }
    else{
        QSqlQuery query;
        query.exec("INSERT INTO utilisateurs(nom,clef) VALUES('"+ui->edit_nom_prenom->text()+"','"+_clef+"')");
        while(query.next()){
        }
    }
    return true;
}

CreerAuthaurisation::~CreerAuthaurisation()
{
    delete ui;
}
