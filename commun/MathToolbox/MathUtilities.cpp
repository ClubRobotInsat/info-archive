// MathUtilities.cpp

#include "MathUtilities.h"
#include "../Units/Units.h"

// Instanciations explicites du code, facilite la vérification d'erreur
template class Unit<1, 1, 1, true>;
template class Unit<1, 1, 0, true>;
template class Unit<1, 0, 1, true>;
template class Unit<1, 0, 0, true>;
template class Unit<0, 1, 1, true>;
template class Unit<0, 1, 0, true>;
template class Unit<0, 0, 1, true>;
