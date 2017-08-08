#ifndef FENETRE_GENERATION_CLEF_MDP_H
#define FENETRE_GENERATION_CLEF_MDP_H

#include <QWidget>

namespace Ui {
class Fenetre_generation_clef_mdp;
}

class Fenetre_generation_clef_mdp : public QWidget
{
    Q_OBJECT

public:
    explicit Fenetre_generation_clef_mdp(QString type,QWidget *parent = 0);


    ~Fenetre_generation_clef_mdp();

public slots:
    void gen_clef_ou_mdp();
    void copier();
    void enregistrer();


private:
    QString _type;
    Ui::Fenetre_generation_clef_mdp *ui;
};

#endif // FENETRE_GENERATION_CLEF_MDP_H
