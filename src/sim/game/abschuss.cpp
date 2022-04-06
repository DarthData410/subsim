#include "abschuss.hpp"

#include "objekte/objekt.hpp"

Abschuss::Abschuss(const Objekt* o_sieger, const Objekt* o_opfer, const Objekt* waffe) :
    o_sieger(Objekt::copy(o_sieger)),
    o_opfer(Objekt::copy(o_opfer)),
    waffe(Objekt::copy(waffe))
{

}
