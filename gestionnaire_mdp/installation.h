#ifndef INSTALLATION_H
#define INSTALLATION_H

#include <QWidget>
#include <string>

namespace Ui {
class Installation;
}

class Installation : public QWidget
{
    Q_OBJECT

public:
    explicit Installation(QWidget *parent = 0);
    void send_error_msg(QString error);
    ~Installation();

private slots:
    void on_bouton_valider_clicked();

private:
    Ui::Installation *ui;
};

//Prototypes des fonctions:
std::string get_value_of_tag(std::string tag,std::string text);

#endif // INSTALLATION_H
