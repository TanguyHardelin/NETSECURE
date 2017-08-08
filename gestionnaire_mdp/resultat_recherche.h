#ifndef RESULTAT_RECHERCHE_H
#define RESULTAT_RECHERCHE_H

#include <QWidget>
#include "complement_recherche.h"

namespace Ui {
class Resultat_recherche;
}

class Resultat_recherche : public QWidget
{
    Q_OBJECT

public:
    explicit Resultat_recherche(QString nom,QString login,QString pwd,QString ip,QString address,QString autresInformations,QString clef,QWidget *parent = 0);
    ~Resultat_recherche();
signals:
    void changer_informations(QString nom,QString login,QString pwd,QString ip,QString port,QString autresInformations);

public slots:
    void copier_login();
    void copier_pwd();
    void modifier_informations();


private:
    QString _nom,_login,_pwd,_ip,_address,_autresInformations;
    complement_recherche* _complements;

    Ui::Resultat_recherche *ui;
};

#endif // RESULTAT_RECHERCHE_H
