#ifndef PROFIL
#define PROFIL

#include <string>

class Profil{
public:
    Profil();//std::string nom,std::string clef,std::string nom_fichier);

protected:
    std::string _nom;
    std::string _clef_cryptage;
    std::string _nom_fichier_cryptage;
};


#endif // PROFIL

