#ifndef CREERNOUVEAUMDP_H
#define CREERNOUVEAUMDP_H

#include <QWidget>

namespace Ui {
class CreerNouveauMdp;
}

class CreerNouveauMdp : public QWidget
{
    Q_OBJECT

public:
    explicit CreerNouveauMdp(QString action="nouveau",QString nom="",QString login="",QString ip="",QString mdp="",QString address="", QString autres_informations="",QWidget *parent = 0);
    void changer_clef(QString new_clef);

    ~CreerNouveauMdp();

signals:
    void finish();

public slots:
    void creerMdp();
    void choisir_clef();
    void genMdp();

private:
    QString _action;
    QString _clef;
    Ui::CreerNouveauMdp *ui;
};

#endif // CREERNOUVEAUMDP_H
