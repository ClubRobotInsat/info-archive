//
// Created by benji on 26/01/17.
//

#ifndef ROOT_REPERE_H
#define ROOT_REPERE_H

#include "Units.h"
#include "MathToolbox/MathToolbox.h"


/**
 * Définition d'un repère unique pour avoir la même norme partout dans le code
 */

namespace repere {
    class Repere;

    struct Coordonnees {
        Coordonnees(const Repere& repere_parent, Vector3m position = Vector3m(0_m, 0_m, 0_m), Angle angle = 0_rad);
        Coordonnees(const Repere& repere_parent, Vector2m position, Angle angle = 0_rad);
        Coordonnees(Coordonnees const& coords);

        //Coordonnees& operator=(const Coordonnees other);

        Vector3m _pos;
        Angle _angle;
        const Repere& _repere_parent;
    };

    enum Multiplicateur {
        SENS_POSITIF = 1,
        SENS_NEGATIF = -1
    };

    class Repere {
    public:
        Repere(Vector2m origine = {0_m, 0_m}, Multiplicateur multX = SENS_POSITIF,
                                              Multiplicateur multY = SENS_POSITIF) {
            _origine = origine;
            _multX = multX;
            _multY = multY;
        }

        Coordonnees get_coordonnees(Coordonnees coordonnees) const;

    private:
        Multiplicateur _multX, _multY;
        Vector2m _origine;
    };
}


#endif //ROOT_REPERE_H
