#include <QDebug>
#include <QProcess>
#include <string>
#include <cstdlib>
#include <QFileDialog>
#include <QSqlQuery>

#include "creernouveaumdp.h"
#include "gestion_cryptage.h"
#include "ui_creernouveaumdp.h"

using namespace std;

CreerNouveauMdp::CreerNouveauMdp(QString action,QString nom,QString login,QString ip,QString mdp,QString address, QString autres_informations,QWidget *parent) :
    QWidget(parent),_action(action),
    ui(new Ui::CreerNouveauMdp)
{
    ui->setupUi(this);

    _clef="";
    //On set les layouts:
    setLayout(ui->global);
    ui->tabWidget->widget(0)->setLayout(ui->global_general);
    ui->tabWidget->widget(1)->setLayout(ui->global_supplementaire);
    ui->tabWidget->widget(2)->setLayout(ui->global_options);

    //On met les informations en place:
    if(nom.length()>0){
        ui->edit_nom->setText(nom);
    }

    //On met les informations en place:
    if(nom.length()>0){
        ui->edit_nom->setText(nom);
    }
    if(login.length()>0){
        ui->edit_login->setText(login);
    }
    if(ip.length()>0){
        ui->edit_ip->setText(ip);
    }
    if(mdp.length()>0){
        ui->edit_mdp_0->setText(mdp);
        ui->edit_mdp_1->setText(mdp);
    }
    if(address.length()>0){
        ui->edit_address->setText(address);
    }
    if(autres_informations.length()>0){
        ui->edit_autres_informations->document()->setPlainText(autres_informations.replace("DFRT","\n"));
    }

    //Connexion:
    QObject::connect(ui->bouton_valider,SIGNAL(clicked(bool)),this,SLOT(creerMdp()));
    QObject::connect(ui->bouton_clef,SIGNAL(clicked(bool)),this,SLOT(choisir_clef()));
    QObject::connect(this,SIGNAL(finish()),this,SLOT(close()));
    QObject::connect(ui->bouton_auto,SIGNAL(clicked()),this,SLOT(genMdp()));

    //On change le titre de la fenetre:
    if(action=="nouveau"){
#ifdef linux
        setWindowTitle(QString::fromUtf8("Générer un nouveau mot de passe"));
        setWindowIcon(QIcon("Images/ajouter_icon.png"));
#endif
#ifdef _WIN32
        setWindowTitle(QString::fromUtf8("Générer un nouveau mot de passe"));
        setWindowIcon(QIcon("Images/ajouter_icon.png"));
#endif
    }
    else{
#ifdef linux
        setWindowTitle(QString::fromUtf8("Modifier le mote de passe"));
        setWindowIcon(QIcon("Images/changer_icon.png"));
    }
#endif
#ifdef _WIN32
        setWindowTitle(QString::fromUtf8("Modifier le mote de passe"));
        setWindowIcon(QIcon("Images/changer_icon.png"));
    }
#endif

}
void CreerNouveauMdp::choisir_clef(){
#ifdef linux
    QString _clef=QFileDialog::getOpenFileName(this,tr("Open Image"),QDir::currentPath()+"/Clefs" , tr("Image Files (*.key)"));
#endif
#ifdef _WIN32
    QString _clef=QFileDialog::getOpenFileName(this,tr("Open Image"),QDir::currentPath()+"\\Clefs" , tr("Image Files (*.key)"));
#endif
    //Je suis obliger de passer par un setter car Qt sous windows a quelques bug chelou.....
    changer_clef(_clef);

}
void CreerNouveauMdp::changer_clef(QString new_clef){
    _clef=new_clef;
}
void CreerNouveauMdp::genMdp(){
    bool ok;
    int taille_gen=10+rand()%10;

   QString result="";
   int MIN=33;
   int MAX=125;

   srand(time(NULL));
   for(int i=0;i<taille_gen;i++){
       char c=(rand() % (MAX - MIN + 1)) + MIN;
       result+=c;
   }

   ui->edit_mdp_0->setText(result);
   ui->edit_mdp_1->setText(result);
}

void CreerNouveauMdp::creerMdp(){
    //Verification des informations:
    bool informations_ok=true;
    QString error="";
    QString nom_clef_final="";
    string nom_clef="";
    if(_clef.operator ==("")){
        nom_clef=Gestion_cryptage::getNomClef();
    }
    else{
        nom_clef=_clef.toStdString();
    }
    #ifdef linux
            nom_clef_final=QString(nom_clef.c_str()).split("Clefs/")[1];
    #endif
    #ifdef _WIN32
            if(_clef.size()==0)
                nom_clef_final=QString(nom_clef.c_str()).split("Clefs\\")[1];
            else
                nom_clef_final=QString(nom_clef.c_str()).split("Clefs/")[1];
    #endif

    if(ui->edit_mdp_0->text()!=ui->edit_mdp_1->text()){
        informations_ok=false;
        error+="Les deux mot de passe ne correspondent pas\n";
    }
    if(ui->edit_nom->text().length()==0 || ui->edit_ip->text().length()==0 || ui->edit_mdp_0->text().length()==0 ||ui->edit_address->text().length()==0 || ui->edit_login->text().length()==0){
        informations_ok=false;
        error+="Certrain champs sont vides";
    }

    if(informations_ok){
        #ifdef linux
            QSqlQuery query;
        #endif
        QString command="";
        if(_action=="nouveau")
            command="INSERT INTO password (nom,ip,address,login,mdp,autresInformations,clef) VALUES ('"+ui->edit_nom->text()+"','"+ui->edit_ip->text()+"','"+ui->edit_address->text()+"','"+ui->edit_login->text()+"','"+QString(Gestion_cryptage::crypt(ui->edit_mdp_0->text().toStdString()).c_str())+"','"+ui->edit_autres_informations->toPlainText().replace("\n","DFRT")+"','"+nom_clef_final+"')";
        else
            command="UPDATE password SET ip='"+ui->edit_ip->text()+"',address='"+ui->edit_address->text()+"',login='"+ui->edit_login->text()+"',mdp='"+QString(Gestion_cryptage::crypt(ui->edit_mdp_1->text().toStdString()).c_str())+"',autresInformations='"+ui->edit_autres_informations->toPlainText().replace("\n","DFRT")+"',clef='"+nom_clef_final+"' WHERE  nom='"+ui->edit_nom->text()+"'";

        //DFRT correspond à un retour à la ligne car celui ci fait bugger la BDD

        qDebug()<<command;
        #ifdef linux
            query.exec(command);
        #endif
        #ifdef _WIN32
            QProcess p;
            p.start(command);
            p.waitForFinished(-1);
            QString resultat=p.readAllStandardOutput();
            qDebug()<<resultat;
            qDebug()<<p.readAllStandardError();
        #endif



        emit finish();
    }
    if(error.size()==0){
        emit finish();
    }
    else{
        ui->label_erreur->setText(error);
    }


}

CreerNouveauMdp::~CreerNouveauMdp()
{
    delete ui;
}
