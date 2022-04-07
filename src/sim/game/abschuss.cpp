#include "abschuss.hpp"

#include "objekte/objekt.hpp"

Abschuss::Abschuss(const Objekt* o_sieger, const Objekt* o_opfer, const Objekt* waffe) :
    o_sieger(Objekt::copy(o_sieger)),
    o_opfer(Objekt::copy(o_opfer)),
    waffe(Objekt::copy(waffe))
{

}

Abschuss::Abschuss(const Abschuss& a) : Abschuss(a.o_sieger.get(), a.o_opfer.get(), a.waffe.get())
{
    this->text = text;
}

const std::string& Abschuss::get_as_text() const {
    if (text.empty()) {
        auto as_string = [](const Objekt* o) {
            if (!o) return std::string("unknown object");
            return "Object " + std::to_string(o->get_id());
        };
        text += as_string(o_sieger.get());
        text += " killed ";
        text += as_string(o_opfer.get());
        text += " with ";
        text += as_string(waffe.get()); // TODO Objekttypen übersetzen
    }
    return text;
}
