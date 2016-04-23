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
#ifndef SOCCERPERCEPTOR_H
#define SOCCERPERCEPTOR_H

#include "playmode.h"
#include <oxygen/gamecontrolserver/baseparser.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <zeitgeist/zeitgeist.h>

using namespace std;


class SoccerPerceptor
{

   public:
   struct VisionSense
   {
       /** distance perceptor to object */
       float distance;

       /** theta is the angle in the X-Y (horizontal) plane */
       float theta;

       /** phi is the latitude angle */
       float phi;

       VisionSense() : distance(0), theta(0), phi(0) {};
   };

public:
    SoccerPerceptor();
    virtual ~SoccerPerceptor();

    bool getSensation(const std::string& message);

    const VisionSense& GetVisionSense(VisionObject obj) const;

    void ParseObjectVision(const oxygen::Predicate& predicate);
    void ParseVision(const oxygen::Predicate& predicate);
    bool GameState(const oxygen::Predicate& predicate);
    void SetupVisionObjectMap();

    salt::Vector3f GetDriveVec(const VisionSense& vision) const;
    salt::Vector3f GetDriveVec(const VisionObject& obj) const;
    VisionObject RotarCancha(const VisionObject& obj) const;
    int getUnum() const;
    string getEstado() const;
    string getTeam() const;
    salt::Vector3f getMyPos() const;


protected:
    zeitgeist::Zeitgeist mZG;
    boost::shared_ptr<oxygen::BaseParser> mParser;

    // mapping from vision object to VisionSense
    typedef std::map<VisionObject, VisionSense> TVisionMap;
    TVisionMap mVisionMap;

    // mapping from <ObjectName>+<ID> to VisionObject
    typedef std::map<std::string, VisionObject> TVisionObjectMap;
    TVisionObjectMap mVisionObjectMap;

    // the players position
    salt::Vector3f mMyPos;
    VisionObject Equipo[5];
    VisionObject EquipoOp[5];
    string team = "";
    string Estado= "";
    int unum = 0;

};

#endif // SOCCERPERCEPTOR_H
