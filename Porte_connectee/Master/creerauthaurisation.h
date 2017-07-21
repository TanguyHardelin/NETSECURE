#ifndef CREERAUTHAURISATION_H
#define CREERAUTHAURISATION_H

#include <QDialog>

namespace Ui {
class CreerAuthaurisation;
}

class CreerAuthaurisation : public QDialog
{
    Q_OBJECT

public:
    explicit CreerAuthaurisation(QStringList const &all_name,QWidget *parent = 0);
    bool update_BDD();
    ~CreerAuthaurisation();

signals:
    void clef_finie(QString clef);

public slots:
    void generation_clef();

private:

    Ui::CreerAuthaurisation *ui;
    QStringList _list_name;
    QString _clef;
};

#endif // CREERAUTHAURISATION_H
