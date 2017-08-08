#include <QClipboard>
#include <QProcess>
#include <QDebug>
#include <QCloseEvent>

#include "complement_recherche.h"
#include "ui_complement_recherche.h"

complement_recherche::complement_recherche(QString login,QString ip,QString address,QString complement,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::complement_recherche)
{
    ui->setupUi(this);
    ui->edit_adresse_complette->setText(address);
    ui->complement->setPlainText(complement.replace("DFRT","\n"));

    setWindowTitle("Complements de recherche");
    _login=login;
    _ip=ip;

    QObject::connect(ui->bouton_firefox,SIGNAL(clicked(bool)),this,SLOT(ouvrirFirefox()));
    QObject::connect(ui->bouton_chrome,SIGNAL(clicked(bool)),this,SLOT(ouvrirChrome()));
    QObject::connect(ui->bouton_internet_explorer,SIGNAL(clicked(bool)),this,SLOT(ouvrirIE()));
    QObject::connect(ui->bouton_ssh,SIGNAL(clicked(bool)),this,SLOT(ouvrirSSH()));
    QObject::connect(ui->bouton_telnet,SIGNAL(clicked(bool)),this,SLOT(ouvrirTelnet()));
    QObject::connect(ui->bouton_putty,SIGNAL(clicked(bool)),this,SLOT(ouvrirPutty()));
    QObject::connect(ui->bouton_DM,SIGNAL(clicked(bool)),this,SLOT(ouvrirDM()));
}
void complement_recherche::ouvrirFirefox(){
    QProcess p;
#ifdef linux
    p.start("firefox --new-tab "+ui->edit_adresse_complette->text());
#endif
#ifdef _WIN32
    p.start("\"C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe\" -url "+ui->edit_adresse_complette->text());
#endif
    p.waitForFinished(-1);
    QString p_stdout = p.readAllStandardOutput();
}

void complement_recherche::ouvrirChrome(){
    QProcess p;
#ifdef linux

#endif
#ifdef _WIN32
    p.start("start chrome \""+ui->edit_adresse_complette->text()+"\"");
#endif
    p.waitForFinished(-1);
    QString p_stdout = p.readAllStandardOutput();
}

void complement_recherche::ouvrirIE(){
    QProcess p;
#ifdef linux

#endif
#ifdef _WIN32
    p.start("start "" \""+ui->edit_adresse_complette->text()+"\"");
#endif
    p.waitForFinished(-1);
    QString p_stdout = p.readAllStandardOutput();
}

void complement_recherche::ouvrirSSH(){
    QProcess p;
#ifdef linux
    p.start("gnome-terminal -e \"ssh "+_login+"@"+_ip+"\"");
#endif
#ifdef _WIN32
    p.start("start putty -ssh \""+_ip+"\"");
#endif
    p.waitForFinished(-1);
    QString p_stdout = p.readAllStandardOutput();
}

void complement_recherche::ouvrirTelnet(){
    QProcess p;
#ifdef linux
    p.start("gnome-terminal -e \"telnet "+_login+"@"+_ip+"\"");
#endif
#ifdef _WIN32
    p.start("start putty -telnet \""+_ip+"\"");
#endif
    p.waitForFinished(-1);
    QString p_stdout = p.readAllStandardOutput();
}
void complement_recherche::ouvrirPutty(){
    QProcess p;
#ifdef linux
#endif
#ifdef _WIN32
    p.start("start putty");
#endif
    p.waitForFinished(-1);
    QString p_stdout = p.readAllStandardOutput();

}
void complement_recherche::ouvrirDM(){
    QProcess p;
#ifdef linux
#endif
#ifdef _WIN32
    p.start("java -jar \"C:\Program Files (x86)\Nortel\JDM\"\jdm.jar\"");
#endif
    p.waitForFinished(-1);
    QString p_stdout = p.readAllStandardOutput();

}

complement_recherche::~complement_recherche()
{
    delete ui;
}
