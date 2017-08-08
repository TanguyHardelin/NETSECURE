#ifndef COMPLEMENT_RECHERCHE_H
#define COMPLEMENT_RECHERCHE_H

#include <QWidget>

namespace Ui {
class complement_recherche;
}

class complement_recherche : public QWidget
{
    Q_OBJECT

public:
    explicit complement_recherche(QString login,QString ip,QString address,QString complement,QWidget *parent = 0);
    ~complement_recherche();

public slots:
    void ouvrirFirefox();
    void ouvrirChrome();
    void ouvrirIE();
    void ouvrirSSH();
    void ouvrirTelnet();
    void ouvrirPutty();
    void ouvrirDM();

private:
    Ui::complement_recherche *ui;

protected:
    QString _login;
    QString _ip;
};

#endif // COMPLEMENT_RECHERCHE_H
