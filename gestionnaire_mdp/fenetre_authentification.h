#ifndef FENETRE_AUTHENTIFICATION_H
#define FENETRE_AUTHENTIFICATION_H

#include <QWidget>

namespace Ui {
class Fenetre_authentification;
}

class Fenetre_authentification : public QWidget
{
    Q_OBJECT

public:
    explicit Fenetre_authentification(QWidget *parent = 0);


    ~Fenetre_authentification();
public slots:
    void authentification();
    void changer_clef();

signals:
    void authentification_complete();

private:
    Ui::Fenetre_authentification *ui;
};

#endif // FENETRE_AUTHENTIFICATION_H
