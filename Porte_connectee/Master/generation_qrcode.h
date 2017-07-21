#ifndef GENERATION_QRCODE_H
#define GENERATION_QRCODE_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>

namespace Ui {
class Generation_QrCode;
}


class Generation_QrCode : public QWidget
{
    Q_OBJECT

public:
    explicit Generation_QrCode(QString const& nom,QString const& clef,QWidget *parent = 0);
    ~Generation_QrCode();

public slots:
    void imprimer();

private:
    Ui::Generation_QrCode *ui;
    QPainter *_qrCodePainter;

    int x,y,dw,dh;



};

#endif // GENERATION_QRCODE_H
