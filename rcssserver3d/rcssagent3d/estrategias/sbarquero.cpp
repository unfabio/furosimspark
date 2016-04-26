/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: SBArquero.cpp 331 2013-02-13 08:31:45Z hedayat $

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
#include "sbarquero.h"

using namespace salt;

SBArquero::SBArquero()
: SoccerBehavior()
{
    ng=0;
}

SBArquero::~SBArquero()
{
}

string SBArquero::Accion()
{


   Vector3f b = soccerPerceptor.GetDriveVec(VO_BALL);

  Vector3f g1 = soccerPerceptor.GetDriveVec(G1L);
  Vector3f g2 = soccerPerceptor.GetDriveVec(G2L);
  Vector3f centro = g1 + g2, d1, d2, contrario, myPos = soccerPerceptor.getMyPos();

  //evitar que quede al costado de la cancha
  if (myPos.x()<-24 && gAbs(myPos.y()) > 4) {
       return Ir(soccerPerceptor.GetDriveVec(G1R));
  }


  //volver al arco si se alejo mucho
  if (myPos.x()>-5 || gAbs(myPos.y()) > 6) {
       return Ir(centro);
  }
  //no se salga de la meta
  if (myPos.x()<-25) {
       return Ir(b);
  }

  if (centro.Dot(b) > 0 ) {
       return Ir(centro);
  }

  if (b.Length() < 4) {
       //Si esta cercar intentar botarla al arco contrario
       contrario = soccerPerceptor.GetDriveVec(G1R);
       contrario.Normalize();
       if (contrario.Dot(b) > .5) {
           return Ir(b);
       }
       return Ir(b - contrario);
  }
  d1 = g1 + b * .3;
  d2 = g2 + b * .3;
  if(d1.Length()<4){
     ng=0;
  }
  if(d2.Length()<4){
     ng=1;
  }
  return Ir(ng?d1:d2);

}
