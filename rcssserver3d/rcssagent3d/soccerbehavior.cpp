/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: soccerbehavior.cpp 331 2013-02-13 08:31:45Z hedayat $

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
#include "soccerbehavior.h"
#include <iostream>
#include <sstream>

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace salt;
using namespace boost;

SoccerBehavior::SoccerBehavior()
// : mMyPos(0, 0, 0)
{
}

SoccerBehavior::~SoccerBehavior()
{
}

string SoccerBehavior::Init()
{
    // use the scene effector to build the agent and beam to a
    // position near the center of the playing field
    return "(scene rsg/agent/furo7x7.rsg)";
}

string SoccerBehavior::Accion() const
{
    return Motores();
}

string SoccerBehavior::Ir(const salt::Vector3f& Dir) const {
    Vector3f vObj, vObjb, nDir = Dir, b = soccerPerceptor.GetDriveVec(VO_BALL);
   // float Dis = vObj.Length();
   float Dis = 0;
/*
    for (int i = 0; i < 5; i++) {
        vObj = soccerPerceptor.GetDriveVec(Equipo[i]);
        Dis = vObj.Length();
        vObjb = vObj - b;
        if (Dis > 0.1 && Dis < 10 && b.Length() > vObjb.Length()) {
            vObj.Normalize();
            vObj *= 5.0 / (Dis * Dis);
            nDir -= vObj;
        }
    }
    for (int i = 0; i < 5; i++) {
        vObj = soccerPerceptor.GetDriveVec(EquipoOp[i]);
        Dis = vObj.Length();
        if (Dis > 0.1 && Dis < 2 && b.Length() + 1 > Dis) {
            vObj.Normalize();
            vObj *= 1.0 / (Dis * Dis);
            nDir -= vObj;
        }
    }
*/
    double theta = salt::gArcTan2(nDir[1], nDir[0]) * 57.2957;
    float d = gAbs(theta);
    float v1 = 0;
    float v2 = 0;
    float vmax = 0;
    if (d > 90) {//andar en reversa
        if (theta > 0) {
            theta -= 180;
        } else {
            theta += 180;
        }
        //d max 180
        if (d > 160) {
            d -= 160;
            vmax = d * d * 0.6;
        }
    } else {//andar bien
        if (d < 20) {
            d = 20 - d;
            vmax = -d * d * 0.6;
        }
    }
    v1 = vmax - theta * .3;
    v2 = vmax + theta * .3;

    return Motores(v1, v2);
}

std::string SoccerBehavior::Motores(float v1, float v2) const
{
    stringstream ss;
    ss << "(er1 " << v1 << ")(er3 " << v2 << ")";
    return ss.str();
}
string SoccerBehavior::Think(const std::string& message)
{
   cout << "message: " << message << endl;
   if(soccerPerceptor.getSensation(message)){
      return Accion();
   }
   return Motores();
}
