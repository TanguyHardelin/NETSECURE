#include <QLabel>
#include <QSqlQuery>

#include "dialog_log.h"
#include "ui_dialog_log.h"

Dialog_log::Dialog_log(QString id_porte,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_log)
{
    ui->setupUi(this);
    //On rajoute ces elements au scrollView:
    QWidget *contenu = new QWidget;
    QVBoxLayout *mLayout = new QVBoxLayout();
    contenu->setLayout(mLayout);

    //On recherche tout les logs correspondant à cette porte:
    QSqlQuery query;
    query.exec("SELECT log FROM log WHERE id_porte="+id_porte);
    while(query.next())
    {
        QHBoxLayout *layout = new QHBoxLayout();
        mLayout->addLayout(layout);

        layout->addWidget(new QLabel(query.value(0).toString()));
    }
    ui->zone_log->setWidget(contenu);

}

Dialog_log::~Dialog_log()
{
    delete ui;
}
