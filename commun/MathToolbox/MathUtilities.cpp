// MathUtilities.cpp

#include "MathUtilities.h"
#include "../Units/Units.h"

// Instanciations explicites du code, facilite la vérification d'erreur
template class NumericValue<Angle, Angle::ValueType>;
template class NumericValue<Distance, Distance::ValueType>;
template class NumericValue<Surface, Surface::ValueType>;
template class NumericValue<Duree, Duree::ValueType>;
template class NumericValue<Masse, Masse::ValueType>;
template class NumericValue<VitesseLineaire, VitesseLineaire::ValueType>;
template class NumericValue<VitesseAngulaire, VitesseAngulaire::ValueType>;