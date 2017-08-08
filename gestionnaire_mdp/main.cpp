#include <iostream>
#include <fstream>
#include <string>
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>

#include "fenetre_principale.h"
#include "gestion_cryptage.h"
#include "installation.h"

using namespace std;



int main(int argc,char **argv){
    QApplication app(argc,argv);

    Fenetre_Principal *fen;
    Installation fen_installation;

    //On cherche à voir si le programme est déjà installé:
    ifstream fichier_config("config.xml",std::ifstream::in);
    if(fichier_config){
        //Lecture du fichier de conf:
        string config="";
        char c=1;
        while(EOF!=(c=fichier_config.get()))
            config+=c;
        fichier_config.close();

        #ifdef linux
            //Conexion à la BDD:
            QSqlDatabase db = QSqlDatabase::addDatabase(get_value_of_tag("sql",config).c_str());
                db.setHostName(get_value_of_tag("hote",config).c_str());
                db.setDatabaseName(get_value_of_tag("name_database",config).c_str());
                db.setUserName(get_value_of_tag("login",config).c_str());
                db.setPassword(get_value_of_tag("pwd",config).c_str());

            bool ok = db.open();


            if(ok){
                cout<<"Connection avec la BDD reussie"<<endl;
                fen=new Fenetre_Principal;
                fen->show();
            }
            else{
                cout<<"Impossible de se connecter à la BDD"<<endl;
                fen_installation.show();
                fen_installation.send_error_msg("Impossible de se connecter à la BDD");
            }
        #endif
        #ifdef _WIN32
            //Windows gère mal les plugins mysql de Qt du coup on prit très fort pour que la connexion à la BDD soit ok.
            fen=new Fenetre_Principal;
            fen->show();
        #endif
    }
    else{
        fen_installation.show();
    }



    return app.exec();
}
