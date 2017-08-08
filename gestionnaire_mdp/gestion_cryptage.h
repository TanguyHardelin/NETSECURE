#ifndef GESTION_CRYPTAGE_H
#define GESTION_CRYPTAGE_H

#include <QWidget>
#include <string>

namespace Ui {
class Gestion_cryptage;
}

class Gestion_cryptage : public QWidget
{
    Q_OBJECT

public:
    explicit Gestion_cryptage(QWidget *parent = 0);

    static std::string crypt(std::string text_a_crypt);
    static std::string decrypt(std::string text_a_decrypt);
    static std::string getNomClef();

    ~Gestion_cryptage();

private:
    Ui::Gestion_cryptage *ui;
};
static QString current_key;



#endif // GESTION_CRYPTAGE_H
