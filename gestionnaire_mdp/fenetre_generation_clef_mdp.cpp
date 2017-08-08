#include <time.h>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include "fenetre_generation_clef_mdp.h"
#include "ui_fenetre_generation_clef_mdp.h"

Fenetre_generation_clef_mdp::Fenetre_generation_clef_mdp(QString type,QWidget *parent) :
    QWidget(parent),_type(type),
    ui(new Ui::Fenetre_generation_clef_mdp)
{
    ui->setupUi(this);
    setLayout(ui->global);
    //On change le titre de la fenetre:
    if(type=="Clef"){
        setWindowTitle(QString::fromUtf8("Générer une clef"));
#ifdef linux
        setWindowIcon(QIcon("Images/Generate_keys_icon.png"));
#endif
#ifdef _WIN32
        setWindowIcon(QIcon("Images/Generate_keys_icon.png"));
#endif
        ui->bouton_copier->setText("Enregistrer dans un fichier");
        QObject::connect(ui->bouton_copier,SIGNAL(clicked(bool)),this,SLOT(enregistrer()));
    }
    else{
        setWindowTitle(QString::fromUtf8("Générer un mot de passe"));
#ifdef linux
        setWindowIcon(QIcon("Images/Generate_password_icon.png"));
#endif
#ifdef _WIN32
        setWindowIcon(QIcon("Images/Generate_password_icon.png"));
#endif
        ui->bouton_copier->setText("Copier");
        QObject::connect(ui->bouton_copier,SIGNAL(clicked(bool)),this,SLOT(copier()));
    }

    QObject::connect(ui->bouton_generer,SIGNAL(clicked(bool)),this,SLOT(gen_clef_ou_mdp()));
}
void Fenetre_generation_clef_mdp::copier(){
    QClipboard *pressePapiers = QApplication::clipboard();
    pressePapiers->setText(ui->resultat->toPlainText());
}
void Fenetre_generation_clef_mdp::enregistrer(){
#ifdef linux
    QString file_name= QFileDialog::QFileDialog::getSaveFileName(this, tr("Enregistrer la clef de cryptage"),
                                                 "Clefs/",
                                                 tr("Clefs (*.key)"));
#endif
#ifdef _WIN32
    QString file_name= QFileDialog::QFileDialog::getSaveFileName(this, tr("Enregistrer la clef de cryptage"),
                                                 "Clefs\\",
                                                 tr("Clefs (*.key)"));
#endif


    QFile f(file_name);
    f.open( QIODevice::WriteOnly );
    QTextStream out(&f);
    out<<ui->resultat->toPlainText();
    f.close();
}


void Fenetre_generation_clef_mdp::gen_clef_ou_mdp(){
    QString taille=ui->edit_taille->text();

    bool ok;
    int taille_gen=taille.toInt(&ok,10);

    if(!ok){
        ui->error->setText("Veillez rentrer un nombre entier pour la taille !");
    }
    else{
       QString result="";
       int MIN=33;
       int MAX=125;

       srand(time(NULL));
       for(int i=0;i<taille_gen;i++){
           char c=(rand() % (MAX - MIN + 1)) + MIN;
           result+=c;
       }

       ui->resultat->setPlainText(result);
    }



}

Fenetre_generation_clef_mdp::~Fenetre_generation_clef_mdp()
{
    delete ui;
}
