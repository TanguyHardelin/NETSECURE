#ifndef FENETRE_PRINCIPALE
#define FENETRE_PRINCIPALE

#include <QMainWindow>
#include <QObject>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <vector>

#include "fenetre_authentification.h"
#include "fenetre_recherche.h"
#include "resultat_recherche.h"
#include "creernouveaumdp.h"
#include "fenetre_generation_clef_mdp.h"
#include "complement_recherche.h"

class Fenetre_Principal: public QMainWindow
{
    Q_OBJECT
public:
    Fenetre_Principal();
    void show_resultat();

public slots:
    void construir_fen_recherche();
    void construir_fen_nouveau_mdp();
    void construir_fen_creer_mdp();
    void construir_fen_creer_key();
    void construir_fen_exporter_key();
    void construir_fen_importer_key();
    void actualiser_mdp(QString nom,QString login,QString pwd,QString ip,QString port,QString autresInformations);

    void faire_recherche(QList<QMap<QString,QString> > infos);


protected:
    Fenetre_authentification *_fen_auth;
    Fenetre_recherche *_fen_recherche;
    Resultat_recherche * _resultat_recherche;
    CreerNouveauMdp *_fen_creer_mdp;
    Fenetre_generation_clef_mdp *_fen_generation;

};

#endif // FENETRE_PRINCIPALE

