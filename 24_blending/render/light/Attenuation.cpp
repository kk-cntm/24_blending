#include <functional>
#include "Attenuation.h"

using namespace render::light;

Attenuation Attenuation::FromDistance(Attenuation::Distance distance) {
    return s_attenuationTable[distance];
}

std::map<Attenuation::Distance, Attenuation> Attenuation::s_attenuationTable = []() {
    std::map<Attenuation::Distance, Attenuation> table;
    
    table[Attenuation::Distance::n_7] = { 1.0f, 0.7, 1.8f };
    table[Attenuation::Distance::n_32] = { 1.0f, 0.14f, 0.07f };
    
    return table;
}();
