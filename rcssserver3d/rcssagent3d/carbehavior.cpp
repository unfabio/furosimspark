/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: carbehavior.cpp,v 1.2 2008/03/27 21:11:53 rollmark Exp $

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
#include "carbehavior.h"
#include <iostream>
#include <sstream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

CarBehavior::CarBehavior()
{
}

string CarBehavior::Init()
{
    // use the scene effector to build the agent
  //  return "(scene rsg/agent/buggy.rsg)";
    //return "(scene rsg/agent/furo.rsg)";

    stringstream ss;
    ss << "(scene rsg/agent/furo7x7.rsg) " << endl  << "(init (unum 0) (teamname FuroRobot)) " << endl  << "(beam "<< -10 << " " << 0 << " " << 90 << ")" << endl  << "(syn)";
    srand (time(NULL));

    return ss.str();
}

string CarBehavior::Think(const std::string& message)
{
    // demo: just drive forward and backward...

    static int n = 0;
    static float velocity1 = 0;
    static float velocity2 = 0;
    ++n;
       cout << ">> " << message << endl;
    if (n > 200)
        {
            velocity1 = ((rand() % 10)-5);
            velocity2 = ((rand() % 10)-5);
            n = 0;
        }
    stringstream ss;
    //ss << "(er1 "  << velocity1 << ")(er3 "  << velocity2 << ")";

    ss <<"(syn)" << endl;

    return ss.str();
}
