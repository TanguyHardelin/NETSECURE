#ifndef ACCEUIL_H
#define ACCEUIL_H

#include <QWidget>

#include "creerauthaurisation.h"
#include "generation_qrcode.h"

namespace Ui {
class Acceuil;
}

class Acceuil : public QWidget
{
    Q_OBJECT

public:
    explicit Acceuil(QWidget *parent = 0);

    void get_all_name_utilisateur();
    void get_all_name_porte();
    void actualise_adapteur();


    ~Acceuil();
public slots:
    void creerNouvelleAutorisation();
    void faire_recherche();
    void montrerFenetreQrCode(QString clef);

private:
    Ui::Acceuil *ui;
    CreerAuthaurisation* fen_nouvelle_authorisation;
    Generation_QrCode* fen_qrcode;

    QStringList *_list_name_porte;
    QStringList *_list_name_utilisateur;
};

#endif // ACCEUIL_H
