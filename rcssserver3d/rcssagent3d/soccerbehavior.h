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
#ifndef SOCCERBEHAVIOR_H
#define SOCCERBEHAVIOR_H

#include "behavior.h"
#include <oxygen/gamecontrolserver/baseparser.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <zeitgeist/zeitgeist.h>
#include <string>

using namespace std;

#define STR_PM_BeforeKickOff "BeforeKickOff"
#define STR_PM_KickOff_Left "KickOff_Left"
#define STR_PM_KickOff_Right "KickOff_Right"
#define STR_PM_PlayOn "PlayOn"
#define STR_PM_KickIn_Left "KickIn_Left"
#define STR_PM_KickIn_Right "KickIn_Right"
#define STR_PM_CORNER_KICK_LEFT "corner_kick_left"
#define STR_PM_CORNER_KICK_RIGHT "corner_kick_right"
#define STR_PM_GOAL_KICK_LEFT "goal_kick_left"
#define STR_PM_GOAL_KICK_RIGHT "goal_kick_right"
#define STR_PM_OFFSIDE_LEFT "offside_left"
#define STR_PM_OFFSIDE_RIGHT "offside_right"
#define STR_PM_GameOver "GameOver"
#define STR_PM_Goal_Left "Goal_Left"
#define STR_PM_Goal_Right "Goal_Right"
#define STR_PM_FREE_KICK_LEFT "free_kick_left"
#define STR_PM_FREE_KICK_RIGHT "free_kick_right"
#define STR_PM_Unknown "unknown"

enum VisionObject
{
     VO_BALL   = 0,
     F1L   = 1,
     G1L   = 2,
     G2L   = 3,
     F2L   = 4,
     F1R   = 5,
     G1R   = 6,
     G2R   = 7,
     F2R   = 8,
     Pleft1,
     Pleft2,
     Pleft3,
     Pleft4,
     Pleft5,
     Pright1,
     Pright2,
     Pright3,
     Pright4,
     Pright5
};

class SoccerBehavior : public Behavior
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
    SoccerBehavior();
    virtual ~SoccerBehavior();

    virtual std::string Init();
    virtual std::string Think(const std::string& message);

    const VisionSense& GetVisionSense(VisionObject obj) const;

protected:
    void ParseObjectVision(const oxygen::Predicate& predicate);
    void ParseVision(const oxygen::Predicate& predicate);
    bool GameState(const oxygen::Predicate& predicate);
    void SetupVisionObjectMap();

    std::string Motores( float v1=0, float v2=0) const;
    virtual std::string Accion() const;

    salt::Vector3f GetPosition(const VisionSense& sense) const;
    salt::Vector3f GetPosition(const VisionObject& obj) const;
    salt::Vector3f GetDriveVec(const VisionSense& vision) const;
    salt::Vector3f GetDriveVec(const VisionObject& obj) const;
    std::string Ir(const salt::Vector3f& Dir) const;

    VisionObject RotarCancha(const VisionObject& obj) const;


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
    string team = "";
    VisionObject Equipo[5];
    VisionObject EquipoOp[5];
    string Estado= "";
    int unum = 0;
};

#endif // SOCCERBEHAVIOR_H
