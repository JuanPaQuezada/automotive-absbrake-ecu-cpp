
#pragma once
#include "eventos_abs.h"

enum class abs_state_enum {monitoreo, activo, fallo};
//hacemos gestion de eventos con handle_event para cada uno de los tags posibles, si un estado no sobre escribe un evento, simplemente devuelve su propio estado ignorando el evento
class estado_abs {
    public:
        virtual abs_state_enum handle_event(evento_frenado_brusco){return get_state_enum();}
        virtual abs_state_enum handle_event(evento_recuperacion_traccion) {return get_state_enum();}
        virtual abs_state_enum handle_event(evento_fallo_sensor) {return get_state_enum();}

        virtual abs_state_enum get_state_enum() = 0;
        virtual ~estado_abs() = default;
};