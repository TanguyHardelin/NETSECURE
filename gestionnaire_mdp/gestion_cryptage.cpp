#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <string>
#include <iostream>
#include <fstream>

#include "gestion_cryptage.h"
#include "ui_gestion_cryptage.h"

using namespace std;

Gestion_cryptage::Gestion_cryptage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gestion_cryptage)
{
    ui->setupUi(this);
}
std::string Gestion_cryptage::getNomClef(){
    //On recupere le nom de la clef courante:
    ifstream fichier_nom_clef("nom_clef.txt",ios::in);

    if(!fichier_nom_clef){
        cout<<"Impossible d'ouvrir le fichier contenant le nom de la clef . . .";
        return "";
    }
    string nom_clef;
    fichier_nom_clef>>nom_clef;;

    fichier_nom_clef.close();

    return nom_clef;
}

string Gestion_cryptage::crypt(string text_a_crypt){
    string nom_clef=Gestion_cryptage::getNomClef();
    ifstream f(nom_clef.c_str(),ios::in);
    if(!f){
        cout<<"Impossible d'ouvrir la clef . . .";
        qDebug()<<"Impossible d'ouvrir la clef";
        return "";
    }

    string result="";
    for(unsigned i=0;i<text_a_crypt.size();i++){
        char c;
        f>>c;
        unsigned char tmp=(int)(text_a_crypt[i]+c);
        result+=QString::number(tmp).toStdString()+'-';
    }
    f.close();
    qDebug()<<"result: "<<QString(result.c_str());
    return result;
}
string Gestion_cryptage::decrypt(string text_a_decrypt){
    string nom_clef=Gestion_cryptage::getNomClef();

    ifstream f(nom_clef.c_str(),ios::in);
    if(!f){
        cout<<"Impossible d'ouvrir la clef . . .";
        return "";
    }

    //On split le coontenu par '-'
    QString tmp(text_a_decrypt.c_str());
    QStringList list_carac=tmp.split('-');

    string result="";
    for(int i=0;i<list_carac.size();i++){
        char c;
        f>>c;
        if(list_carac[i]!="")
            result+=list_carac[i].toInt()-c;
    }





    f.close();
    return result;
}


Gestion_cryptage::~Gestion_cryptage()
{
    delete ui;
}
