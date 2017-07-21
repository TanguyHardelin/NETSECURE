#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <QLabel>
#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>
#include <QGraphicsScene>
#include <qqrencode.h>


#include "generation_qrcode.h"
#include "ui_generation_qrcode.h"


Generation_QrCode::Generation_QrCode(QString const & nom,QString const & clef,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Generation_QrCode)
{
    ui->setupUi(this);
    //On change l'icon et le titre:
    setWindowIcon(QIcon("Images/icon_qrcode.png"));
    setWindowTitle("Clef du salarié: "+nom);





    ui->zone_QrCode->resize(ui->zone_QrCode->width(),ui->zone_QrCode->width());

    this->setStyleSheet("background-color:white;");
    ui->zone_QrCode->setStyleSheet("background-color:white;");


    QQREncode encoder;
    encoder.encode(clef);
    QImage qrcode = encoder.toQImage();
    qrcode.scaled(500,500).save("Clefs/qrcode_"+nom+"_save.jpg","JPG");
    ui->label_qrcode->setPixmap(QPixmap("Clefs/qrcode_"+nom+"_save.jpg"));
    QObject::connect(ui->bouton_imprimer,SIGNAL(clicked()),this,SLOT(imprimer()));
}
void Generation_QrCode::imprimer(){
    //On change les texte pour rendre bien à l'impréssion:
    ui->bouton_imprimer->hide();
    ui->label_mode_demploi->setText("Le QrCode ci-dessous contient votre indentifiant vous permettant d'ouvrir les portes de votre entreprise.\n\nScannez celui-ci avec l'application qui vous a étée fournie lors de votre arrivé.\nUne fois ce QrCode scanné il ne vous reste plus que à poser votre téléphone sur les badgeuses pour ouvrir les portes.");

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));

    if (dialog->exec() != QDialog::Accepted)
             return;

    QPainter painter;
    painter.begin(&printer);
    double xscale = printer.pageRect().width()/double(this->width());
    double yscale = printer.pageRect().height()/double(this->height());
    double scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                       printer.paperRect().y() + printer.pageRect().height()/2);
    painter.scale(scale, scale);
    painter.translate(-width()/2, -height()/2);

    this->render(&painter);

    this->close();
}

Generation_QrCode::~Generation_QrCode()
{
    delete ui;
}
