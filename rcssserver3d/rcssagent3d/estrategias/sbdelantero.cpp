/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: SBDelantero.cpp 331 2013-02-13 08:31:45Z hedayat $

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
#include "sbdelantero.h"
// #include <iostream>
// #include <sstream>

using namespace salt;

SBDelantero::SBDelantero()
: SoccerBehavior()
{
}

SBDelantero::~SBDelantero()
{
}

string SBDelantero::Accion() const
{

    Vector3f b = soccerPerceptor.GetDriveVec(VO_BALL);
    Vector3f myPos = soccerPerceptor.getMyPos();
    int unum=soccerPerceptor.getUnum();
    VisionObject G1, G2, F1, F2;

    G1 = G1R;
    G2 = G2R;
    F1 = F1L;
    F2 = F2L;

    Vector3f g1 = soccerPerceptor.GetDriveVec(G1);
    Vector3f g2 = soccerPerceptor.GetDriveVec(G2);
    Vector3f Dir = (g1 + g2) / 2;

    if (myPos.x() <-10 && b.Length() > 5) {
        return Ir(Dir);
    }

    //Buscar centrar el Balon si esta cerca de las esquinas del contrario
    if (gAbs(myPos.y()) + myPos.x() > 28) {
        Dir += (soccerPerceptor.GetDriveVec(F2) + soccerPerceptor.GetDriveVec(F1))*.01;
    }

    Dir -= b;
    Dir = Dir.Normalize();

    float Pesc = Dir.Dot(b) / b.Length(), fact = 0;
    // cout << " Pesc " << Pesc <<" b.Length() " << b.Length() << endl;
    if (Pesc < 0.3 && b.Length() < 2) {
        if (unum % 2 == 0) {
            Dir = soccerPerceptor.GetDriveVec(F1);
        } else {
            Dir = soccerPerceptor.GetDriveVec(F2);
        }
        Dir = Dir.Normalize() - b.Normalize();
    } else if (Pesc > 0.75 && b.Length() < 5) {
        Dir = Dir + b;
    } else {
        fact = -b.Length() *.5 - 1;
        Dir = Dir * fact + b;
    }
    return Ir(Dir);
}
