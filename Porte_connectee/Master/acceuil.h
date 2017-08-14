#ifndef ACCEUIL_H
#define ACCEUIL_H

#include <QWidget>
#include <QProcess>

#include "creerauthaurisation.h"

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
    void ajouter_fen_auth(CreerAuthaurisation *fen_auth);


    ~Acceuil();
public slots:
    void creerNouvelleAutorisation();
    void faire_recherche(QString nom="");
    bool update_BDD();
    void supprimer_utilisateur();
    void fermer_fenetre_creation_utilisateur();

private:
    Ui::Acceuil *ui;
    CreerAuthaurisation* fen_nouvelle_authorisation;


    QStringList *_list_name_porte;
    QStringList *_list_name_utilisateur;
    QString nom_utilisateur;
    QString old_numero;
    QSerialPort* arduino;
};

#endif // ACCEUIL_H
