#ifndef ROW_OF_LIST_ACCES_H
#define ROW_OF_LIST_ACCES_H

#include <QWidget>
#include "dialog_log.h"

namespace Ui {
class row_of_list_acces;
}

class row_of_list_acces : public QWidget
{
    Q_OBJECT

public:
    explicit row_of_list_acces(QString nom_porte,QString id_porte,QString id_utilisateur,QWidget *parent = 0);
    ~row_of_list_acces();

public slots:
    void autoriser_acces();
    void supprimer_acces();
    void show_log();

private:
    Ui::row_of_list_acces *ui;
    QString id_utilisateur,id_porte;

    Dialog_log *fenetre_log;
};

#endif // ROW_OF_LIST_ACCES_H
