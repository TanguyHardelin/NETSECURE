#ifndef FENETRE_RECHERCHE_H
#define FENETRE_RECHERCHE_H

#include <QWidget>
#include <QStringList>
#include <QMap>


namespace Ui {
class Fenetre_recherche;
}

class Fenetre_recherche : public QWidget
{
    Q_OBJECT

public:
    explicit Fenetre_recherche(QWidget *parent = 0);
    void get_all_name_possible();
    void get_all_ip_possible();
    #ifdef _win32
        void analyse_sortie_BBD(QString p_stdout);
    #endif


    ~Fenetre_recherche();
public slots:
    void finir_recherche();

signals:
    void recherche_finie(QList<QMap<QString,QString> > all_info);


private:
    Ui::Fenetre_recherche *ui;
    QStringList *_nom_possibles;
    QStringList *_ip_possibles;
    QList<QMap<QString,QString> > _list_info;

};

#endif // FENETRE_RECHERCHE_H
