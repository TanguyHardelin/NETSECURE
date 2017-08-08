#ifndef CREERAUTHAURISATION_H
#define CREERAUTHAURISATION_H

#include <QDialog>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class CreerAuthaurisation;
}

class CreerAuthaurisation : public QDialog
{
    Q_OBJECT

public:
    explicit CreerAuthaurisation(QStringList const &all_name,QWidget *parent = 0);
    void update();
    ~CreerAuthaurisation();

signals:
    void finish(QString nom);

public slots:
    void readSerial();
    bool update_BDD();

private:
    Ui::CreerAuthaurisation *ui;
    QStringList _list_name;
    QString _clef;


    //Communication serie:
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 10755;  //INFO A REMPLIR POUR CHAQUE NOUVEL ARDUINO
    static const quint16 arduino_uno_product_id = 67;  //INFO A REMPLIR POUR CHAQUE NOUVEL ARDUINO
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
};

#endif // CREERAUTHAURISATION_H
