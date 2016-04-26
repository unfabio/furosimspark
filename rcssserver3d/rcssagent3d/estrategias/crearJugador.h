/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: soccerbehavior.h 9 2008-11-24 02:39:02Z hedayat $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef CREARJUGADOR_H
#define CREARJUGADOR_H

#include "leftFuroBlue/sbarquerol.h"
#include "leftFuroBlue/sbdefensal.h"
#include "leftFuroBlue/sbdelanterol.h"
#include "rightFuroRed/sbarqueror.h"
#include "rightFuroRed/sbdefensar.h"
#include "rightFuroRed/sbdelanteror.h"

using namespace std;

typedef SoccerBehavior* ( *SoccerBehavior_Crear)(void);


class CrearJugador
{
private:

       CrearJugador &operator=(const CrearJugador &) { return *this; }
public:
   CrearJugador();
   CrearJugador(const CrearJugador &) { }
    virtual ~CrearJugador();

    static CrearJugador *Get()
    {
       static CrearJugador instance;
       return &instance;
    }
    SoccerBehavior *Crear(const string &nombreJugador);

protected:
    typedef std::map<std::string, SoccerBehavior_Crear> JugadoresMap;
    JugadoresMap mJugadoresMap;
};

#endif // CREARJUGADOR
