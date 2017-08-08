#include <fstream>
#include <string>

#include "installation.h"
#include "ui_installation.h"

using namespace std;


/*
    Paramètres:
        aucun

     Retour:
        aucun

     Fonction:
        Constructeur de la class Installation.
*/
Installation::Installation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Installation)
{
    ui->setupUi(this);


    //Ajustement de la taille de l'image:
    const QPixmap *image_logo=ui->image_logo->pixmap();
    int width=this->width();
    int height=this->height();

    ui->image_logo->setPixmap(image_logo->scaled(QSize(width/2,height),  Qt::KeepAspectRatio));

    //Remplissage des champs en cas d'erreur:
    ifstream fichier_config("config.xml",std::ifstream::in);
    if(fichier_config){
        //Lecture du fichier de conf:
        string config="";
        char c=1;
        while(EOF!=(c=fichier_config.get()))
            config+=c;
        fichier_config.close();

        //Conexion à la BDD:
        ui->edit_hote->setText(get_value_of_tag("hote",config).c_str());
        ui->edit_name_BDD->setText(get_value_of_tag("name_database",config).c_str());
        ui->edit_login->setText(get_value_of_tag("login",config).c_str());
        ui->edit_mdp->setText(get_value_of_tag("pwd",config).c_str());
    }
    QObject::connect(ui->bouton_valider,SIGNAL(clicked()),this,SLOT(on_bouton_valider_clicked()));
}
/*
    Paramètres:
        aucun

     Retour:
        aucun

     Fonction:
        Destructeur de la class Installation.
*/
Installation::~Installation()
{
    delete ui;
}
/*
    Paramètres:
       QString error: texte d'erreur à afficher.

     Retour:
        aucun

     Fonction:
        Affiche un message d'erreur dans le fenetre d'installation.
*/
void Installation::send_error_msg(QString error){
    ui->label_error->setText(error);
}
/*
    Paramètres:
        aucun

     Retour:
        aucun

     Fonction:
        Fonction appelée lorsqu'on clique sur le bouton "valider". Cette fonction prend les différentes informations remplies par l'utilisateur et les enregistre sous forme
        d'arbre XML dans le fichier config.xml .
*/
void Installation::on_bouton_valider_clicked()
{
   //écriture dans le fichier de configuration:
   ofstream fichier_config("config.xml",std::ofstream::out);
   fichier_config<<("<sql>"+ui->comboBox_sql->currentText()+"</sql>").toStdString();
   fichier_config<<("<hote>"+ui->edit_hote->text()+"</hote>").toStdString();
   fichier_config<<("<name_database>"+ui->edit_name_BDD->text()+"</name_database>").toStdString();
   fichier_config<<("<login>"+ui->edit_login->text()+"</login>").toStdString();
   fichier_config<<("<pwd>"+ui->edit_mdp->text()+"</pwd>").toStdString();

   fichier_config.close();

    this->close();

}

/*
    Paramètres:
        QString tag: tag XML dans lequel on veut recupérer les données.
        QString text: text XML à parser.

     Retour:
        Contenu présent entre les balises <tag></tag>
*/
string get_value_of_tag(string tag,string text){
    QString text_to_split(text.c_str());
    QString tag_of_xml(tag.c_str());
    return text_to_split.split("</"+tag_of_xml+">")[0].split("<"+tag_of_xml+">")[1].toStdString();
}
