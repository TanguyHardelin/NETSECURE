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
    ui->NFC_OK->hide();

    /*
    ui->widget_plage_horraire->hide();
    ui->widget_periode->hide();
*/
    //Mettre la taille mini
    this->adjustSize();


    //On change le titre:
    setWindowTitle("Ajouter un salarié");

    //On connect l'arduino:
    arduino = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";

       /*
        *  Testing code, prints the description, vendor id, and product id of all ports.
        *  Used it to determine the values for the arduino uno.
        *
        */
       /*
        *
        * Le code suivant est a lancer pour chaque nouveau arduino connecté ... remplir les données dans le fenetre.h
        *
        *
        */
       qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
       foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
           qDebug() << "Description: " << serialPortInfo.description() << "\n";
           qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
           qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
           qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
           qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
       }

       /*
        *   Identify the port the arduino uno is on.
        */
       bool arduino_is_available = false;
       QString arduino_uno_port_name;
       //
       //  For each available serial port
       //
       foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
           //  check if the serialport has both a product identifier and a vendor identifier
           if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
               //  check if the product ID and the vendor ID match those of the arduino uno
               if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
                       && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                   arduino_is_available = true; //    arduino uno is available on this port
                   arduino_uno_port_name = serialPortInfo.portName();
               }
           }
       }

       /*
        *  Open and configure the arduino port if available
        */
       if(arduino_is_available){
           qDebug() << "Found the arduino port...\n";
           arduino->setPortName(arduino_uno_port_name);
           arduino->open(QSerialPort::ReadOnly);
           arduino->setBaudRate(QSerialPort::Baud115200);
           arduino->setDataBits(QSerialPort::Data8);
           arduino->setFlowControl(QSerialPort::NoFlowControl);
           arduino->setParity(QSerialPort::NoParity);
           arduino->setStopBits(QSerialPort::OneStop);
           QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));

       }else{
           qDebug() << "Couldn't find the correct port for the arduino.\n";
       }


    //Les connects:
    QObject::connect(ui->bouton_annuler,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(ui->bouton_valider,SIGNAL(clicked(bool)),this,SLOT(update_BDD()));
}
void CreerAuthaurisation::update(){
    //On surcharge la méthode pour permettre de passer pas QSesigner:
    this->adjustSize();
}

void CreerAuthaurisation::readSerial(){
    serialData = arduino->readAll();
    serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
    serialData.clear();
    QStringList tmp=serialBuffer.split("\r\n");
    if(tmp.size()>3){
        if(ui->check_bague->isChecked() &&tmp[tmp.size()-2].size()>=28){
            _clef=tmp[tmp.size()-2];
            qDebug()<<_clef;

            //On cache l'indication du NFC et on montre la petite vignette NFC_OK
            ui->indication_NFC->hide();

            ui->NFC_OK->show();

        }
        else if(tmp[tmp.size()-2].size()>=19 && !ui->check_bague->isChecked()){
            _clef=tmp[tmp.size()-2];

            //On cache l'indication du NFC et on montre la petite vignette NFC_OK
            ui->indication_NFC->hide();

            ui->NFC_OK->show();
            _clef=tmp[tmp.size()-2];

            //On cache l'indication du NFC et on montre la petite vignette NFC_OK
            ui->indication_NFC->hide();

            ui->NFC_OK->show();
        }
    }
}
bool CreerAuthaurisation::update_BDD(){
    bool permition=false;
    bool horraire=false;
    if(_list_name.contains(ui->edit_nom_prenom->text())){
        //Afficher: "ce nom existe déjà"
        qDebug()<<"Ce nom est déjà pris !";
        ui->label_error->setText("Ce nom existe déjà dans la base de donnée. Allez dans \"Rechercher utilisateur\" pour editer ses permitions");
        return false;
    }
    else if(_clef.length()<19){
        qDebug()<<"Scannez la carte  NFC d'abord !";
        ui->label_error->setText("Merci de scanner une carte NFC");
        return false;

    }
    else if(ui->edit_nom_prenom->text().length()<3){
        qDebug()<<"Entrez un nom valide";
        ui->label_error->setText("Merci d'entrer un nom valide");
        return false;

    }
    if(ui->widget_plage_horraire->isVisible()){
        horraire=true;
        qDebug()<<"Horraire TRUE";
    }
    if(ui->widget_periode->isVisible()){
        permition=true;
        qDebug()<<"Permition TRUE";
    }
    QSqlQuery query;
    if(permition && horraire)
        query.exec("INSERT INTO utilisateurs(nom,clef,heure_debut,heure_fin,date_debut,date_fin) VALUES('"+ui->edit_nom_prenom->text()+"','"+_clef+"','"+ui->_heure_debut->text()+"','"+ui->_heure_fin->text()+"','"+ui->_date_debut->text()+"','"+ui->_date_fin->text()+"')");

    else if(permition){
        query.exec("INSERT INTO utilisateurs(nom,clef,heure_debut,heure_fin,date_debut,date_fin) VALUES('"+ui->edit_nom_prenom->text()+"','"+_clef+"','NONE','NONE','"+ui->_date_debut->text()+"','"+ui->_date_fin->text()+"')");
        qDebug()<<"INSERT INTO utilisateurs(nom,clef,heure_debut,heure_fin,date_debut,date_fin) VALUES('"+ui->edit_nom_prenom->text()+"','"+_clef+"','NONE','NONE','"+ui->_date_debut->text()+"','"+ui->_date_fin->text()+"')";
    }

    else if(horraire)
        query.exec("INSERT INTO utilisateurs(nom,clef,heure_debut,heure_fin,date_debut,date_fin) VALUES('"+ui->edit_nom_prenom->text()+"','"+_clef+"','"+ui->_heure_debut->text()+"','"+ui->_heure_fin->text()+"','NONE','NONE')");

    else
        query.exec("INSERT INTO utilisateurs(nom,clef,heure_debut,heure_fin,date_debut,date_fin) VALUES('"+ui->edit_nom_prenom->text()+"','"+_clef+"','NONE','NONE','NONE','NONE')");

    while(query.next());

    emit finish(ui->edit_nom_prenom->text());
    return true;
}
CreerAuthaurisation::~CreerAuthaurisation()
{
    delete ui;
}

