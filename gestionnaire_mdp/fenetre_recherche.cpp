#include <QDebug>
#include <QProcess>
#include <QWidget>
#include <QCompleter>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSqlQuery>

#include "fenetre_recherche.h"
#include "ui_fenetre_recherche.h"

/*
    Paramètres:
        QWidget * parent (optionnel): parent éventuel de la fenetre.

     Retour:
        aucun

     Fonction:
        Constructeur de Fenetre_recherche. Fenetre dont le role et de permettre à l'utilisateur de rechercher des mot de passes par IP ou bien par nom.
*/
Fenetre_recherche::Fenetre_recherche(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Fenetre_recherche)
{
    ui->setupUi(this);
    setLayout(ui->global);
    //On change le titre de la fenetre:
    setWindowTitle(QString::fromUtf8("Faire une recherche"));


    //On lit une première fois la BDD pour savoir quelles sont les IP et les nom des appareils présents.
    get_all_name_possible();
    get_all_ip_possible();


    //On met en place l'auto-completion:
    QCompleter *completer_ip=new QCompleter(*_ip_possibles,this);
    QCompleter *completer_name=new QCompleter(*_nom_possibles,this);

    completer_ip->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer_name->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

    completer_ip->setCaseSensitivity(Qt::CaseSensitive);
    completer_ip->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer_ip->setCompletionMode(QCompleter::PopupCompletion);
    completer_ip->setWrapAround(false);



    completer_name->setCaseSensitivity(Qt::CaseSensitive);
    completer_name->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer_name->setCompletionMode(QCompleter::PopupCompletion);
    completer_name->setWrapAround(false);

    ui->edit_ip->setCompleter(completer_ip);
    ui->edit_nom->setCompleter(completer_name);


    QObject::connect(ui->bouton_valider,SIGNAL(clicked(bool)),this,SLOT(finir_recherche()));
}
/*
    paramètres:
        aucun

     retour:
        aucun

     fonction:
        rempli l'attribut _nom_possibles (qstringlist) avec tout les nom des appareils contenus dans la BDD.
*/
void Fenetre_recherche::get_all_name_possible(){
    #ifdef linux
        _nom_possibles=new QStringList;
        QSqlQuery query;
        query.exec("SELECT nom FROM password");

        while(query.next()){
            qDebug()<<query.value(0).toString();
            _nom_possibles->append(query.value(0).toString());
        }
    #endif
    #ifdef _WIN32
        //On execute le script python d'interaction avec la BDD

        QProcess p;
        p.start("python Scripts/interaction_BDD.py --get_all_name");
        p.start("C:\\Python27\\python.exe Scripts\\interaction_BDD.py --get_all_name");
        p.waitForFinished(-1);
        QString p_stdout = p.readAllStandardOutput();
        QStringList tmp=p_stdout.split("|SEPARATEUR|");


        _nom_possibles=new QStringList;

        for(int i=0;i<tmp.size();i++){
            _nom_possibles->append(tmp[i]);
        }
    #endif
}
/*
    paramètres:
        aucun

     retour:
        aucun

     fonction:
        rempli l'attribut _ip_possibles (qstringlist) avec toute les IP des appareils contenus dans la BDD.
*/
void Fenetre_recherche::get_all_ip_possible(){
    #ifdef linux
        _ip_possibles=new QStringList;
        QSqlQuery query;
        query.exec("SELECT ip FROM password");

        while(query.next()){
            qDebug()<<query.value(0).toString();
            _ip_possibles->append(query.value(0).toString());
        }
    #endif
    #ifdef _WIN32
        QProcess p;
        p.start("C:\\Python27\\python.exe Scripts\\interaction_BDD.py --get_all_ip");
        p.waitForFinished(-1);

        QString p_stdout = p.readAllStandardOutput();
        QStringList tmp=p_stdout.split("|SEPARATEUR|");


        _ip_possibles=new QStringList;

        for(int i=0;i<(tmp.size()-1);i++){
            _ip_possibles->append(tmp[i]);
        }
    #endif
}
/*
    Paramètres:
        aucun

     Retour:
        aucun

     Fonction:
        Fonction dont le role est de récupérer toute les informations sur un appareil. C'est à dire nom,IP, adresse de connection, login, mot de passe, informations utiles,
        clef de cryptage utilisée.
*/
#ifdef linux
    void Fenetre_recherche::finir_recherche(){
        QString ip=ui->edit_ip->text();
        QString nom=ui->edit_nom->text();

        QSqlQuery query;
        if(ip.length()>1){
            query.exec("SELECT * FROM password WHERE ip='"+ip+"'");
        }
        if(nom.length()>1){
            query.exec("SELECT * FROM password WHERE nom='"+nom+"'");
        }


        while(query.next()){
            QMap<QString,QString> dictionnaire_tmp;

            dictionnaire_tmp["nom"]=query.value(0).toString();
            dictionnaire_tmp["ip"]=query.value(1).toString();
            dictionnaire_tmp["address"]=query.value(2).toString();
            dictionnaire_tmp["login"]=query.value(3).toString();
            dictionnaire_tmp["mdp"]=query.value(4).toString();
            dictionnaire_tmp["autresInformations"]=query.value(5).toString();
            dictionnaire_tmp["clef"]=query.value(6).toString();

            _list_info.append(dictionnaire_tmp);
        }
        emit recherche_finie(_list_info);
    }
#endif
#ifdef _WIN32
    void Fenetre_recherche::finir_recherche(){
        QString ip=ui->edit_ip->text();
        QString nom=ui->edit_nom->text();

        qDebug()<<"Nom : "<<nom;
        qDebug()<<"IP : "<<ip;

        QString resultat_ip="";
        QString resultat_nom="";

        bool nom_ok=false;
        bool ip_ok=false;
        if(ip.length()>1){
            QProcess p;
            p.start("C:\\Python27\\python.exe Scripts\\interaction_BDD.py --execute_command SELECT_*_FROM_password_WHERE_ip=\'"+ip+"\'");
            p.waitForFinished(-1);
            resultat_ip=p.readAllStandardOutput();
            ip_ok=true;
            qDebug()<<"IP OK";
        }
        if(nom.length()>1){
            QProcess p;
            p.start("C:\\Python27\\python.exe Scripts\\interaction_BDD.py --execute_command SELECT_*_FROM_password_WHERE_nom=\'"+nom.replace(" ","_")+"\'");
            p.waitForFinished(-1);
            resultat_nom=p.readAllStandardOutput();
            nom_ok=true;
            qDebug()<<"NOM OK";
        }
        if(resultat_ip!=resultat_nom && nom_ok && ip_ok){
            analyse_sortie_BBD(resultat_ip);
            analyse_sortie_BBD(resultat_nom);
        }
        else if(nom_ok){
            analyse_sortie_BBD(resultat_nom);
        }
        else if(ip_ok){
            analyse_sortie_BBD(resultat_ip);
        }
        qDebug()<<"Resultat nom : "<<resultat_nom;
        qDebug()<<"Resultat IP : "<<resultat_ip;
        emit recherche_finie(_list_info);
    }

    void Fenetre_recherche::analyse_sortie_BBD(QString p_stdout){
        int taille_stdout=p_stdout.split("|SEPARATEUR|").size()-1;
        for(int i=0;i<taille_stdout;i++){
            QMap<QString,QString> dictionnaire_tmp;
            QStringList tmp=p_stdout.split("|SEPARATEUR|")[i].split("-SEPARATEUR-");

            dictionnaire_tmp["nom"]=(QString)tmp[0];
            dictionnaire_tmp["ip"]=(QString)tmp[1];
            dictionnaire_tmp["address"]=(QString)tmp[2];
            dictionnaire_tmp["login"]=(QString)tmp[3];
            dictionnaire_tmp["mdp"]=(QString)tmp[4];
            dictionnaire_tmp["autresInformations"]=(QString)tmp[5];
            dictionnaire_tmp["clef"]=(QString)tmp[6];

            _list_info.append(dictionnaire_tmp);

        }
    }
#endif
/*
    Paramètres:
        aucun

     Retour:
        aucun

     Fonction:
        Destructeur de la class Fenetre_recherche.
*/
Fenetre_recherche::~Fenetre_recherche()
{
    delete ui;
}
