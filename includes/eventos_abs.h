#pragma once
//tags para tag dispatching 
//estos structs no ocupan memoria solo sirve al compilador para enrutar llamadas
struct evento_frenado_brusco {};
struct evento_recuperacion_traccion {};
struct evento_fallo_sensor {};