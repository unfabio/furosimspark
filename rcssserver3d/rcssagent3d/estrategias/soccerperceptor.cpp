/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: SoccerPerceptor.cpp 331 2013-02-13 08:31:45Z hedayat $

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
#include "soccerperceptor.h"
#include <iostream>
#include <sstream>

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace salt;
using namespace boost;

SoccerPerceptor::SoccerPerceptor()
: mZG("." PACKAGE_NAME)
{
   mZG.GetCore()->ImportBundle("sexpparser");
   mParser = static_pointer_cast<BaseParser>(mZG.GetCore()->New("SexpParser"));

   if (mParser.get() == 0) {
      cerr << "unable to create SexpParser instance." << endl;
   }

   SetupVisionObjectMap();
}

SoccerPerceptor::~SoccerPerceptor()
{
}

void SoccerPerceptor::SetupVisionObjectMap()
{
    mVisionObjectMap.clear();
    mVisionObjectMap["B"] = VO_BALL;
    mVisionObjectMap["F1L"] = F1L;
    mVisionObjectMap["G1L"] = G1L;
    mVisionObjectMap["G2L"] = G2L;
    mVisionObjectMap["F2L"] = F2L;
    mVisionObjectMap["F1R"] = F1R;
    mVisionObjectMap["G1R"] = G1R;
    mVisionObjectMap["G2R"] = G2R;
    mVisionObjectMap["F2R"] = F2R;
    mVisionObjectMap["PL1"] = PL1;
    mVisionObjectMap["PL2"] = PL2;
    mVisionObjectMap["PL3"] = PL3;
    mVisionObjectMap["PL4"] = PL4;
    mVisionObjectMap["PL5"] = PL5;
    mVisionObjectMap["PR1"] = PR1;
    mVisionObjectMap["PR2"] = PR2;
    mVisionObjectMap["PR3"] = PR3;
    mVisionObjectMap["PR4"] = PR4;
    mVisionObjectMap["PR5"] = PR5;
}


void SoccerPerceptor::ParseObjectVision(const Predicate& predicate)
{
    for (
            Predicate::Iterator iter(predicate);
            iter != iter.end();
            ++iter) {
        // extract the element as a parameter list
        Predicate::Iterator paramIter = iter;
        if (!predicate.DescentList(paramIter)) {
            continue;
        }

        // read the object name
        string name;
        if (!predicate.GetValue(paramIter, name)) {
            continue;
        }

        // try read the 'id' section
        string strteam;
        if (predicate.GetValue(paramIter, "team", strteam)) {
            if (strteam.find("left") != std::string::npos) {
                name += "L";
            } else if (strteam.find("right") != std::string::npos) {
                name += "R";
            }
        }


        // try read the 'id' section
        string strId;
        if (predicate.GetValue(paramIter, "id", strId)) {
            name += strId;
        }

        // try to lookup the VisionObject
        TVisionObjectMap::iterator visiter = mVisionObjectMap.find(name);
        if (visiter == mVisionObjectMap.end()) {
            // cout<< " no existe name "<< name << endl;
            continue;
        }

        VisionObject vo = (*visiter).second;

        // find the 'pol' entry in the object's section
        Predicate::Iterator polIter = paramIter;
        if (!predicate.FindParameter(polIter, "pol")) {
            continue;
        }

        // read the position vector
        VisionSense sense;
        if (
                (!predicate.AdvanceValue(polIter, sense.distance)) || (!predicate.AdvanceValue(polIter, sense.theta)) || (!predicate.AdvanceValue(polIter, sense.phi))) {
            continue;
        }

        // update the vision map
        //             cerr << "+++" << endl;
        //             cerr << "VO " << vo << endl;
        //             cerr << "D " << sense.distance << endl;
        //             cerr << "T " << sense.theta << endl;
        //             cerr << "P " << sense.phi << endl;
        //             cerr << "---" << endl;
        mVisionMap[RotarCancha(vo)] = sense;
    }
}

const SoccerPerceptor::VisionSense& SoccerPerceptor::GetVisionSense(VisionObject obj) const
{
    static VisionSense invalidSense;

    TVisionMap::const_iterator iter = mVisionMap.find(obj);

    if (iter == mVisionMap.end()) {
        cerr << "unknown VisionObject " << obj << "\n";
        return invalidSense;
    }

    return (*iter).second;
}

Vector3f SoccerPerceptor::GetDriveVec(const VisionObject& obj) const
{
    return GetDriveVec(GetVisionSense(obj));
}

Vector3f SoccerPerceptor::GetDriveVec(const VisionSense& vision) const
{
    return Vector3f(
            vision.distance * gCos(gDegToRad(vision.theta)) * gSin(gDegToRad(90.0f - vision.phi)),

            vision.distance * gSin(gDegToRad(vision.theta)) * gSin(gDegToRad(90.0f - vision.phi)),

            vision.distance * gCos(gDegToRad(90.0f - vision.phi)));
}

void SoccerPerceptor::ParseVision(const Predicate& predicate)
{
    ParseObjectVision(predicate);
    // return;
    // find the PerfectVision data about the object
    Predicate::Iterator iter(predicate);

    // advance to the section about object 'name'

    if (!predicate.FindParameter(iter, "mypos")) {
        return;
    }

    // read my position
    VisionSense sense;

    predicate.GetValue(iter, mMyPos);
}

bool SoccerPerceptor::GameState(const Predicate& predicate)
{
    // find the PerfectVision data about the object
    Predicate::Iterator iter(predicate);

    // advance to the section about object 'name'

    string Value;
    if (predicate.FindParameter(iter, "unum")) {
        predicate.GetValue(iter, Value);
        unum = stoi(Value);
        cout << "FindParameter unum: " << unum << endl;
    }
    Predicate::Iterator iter2(predicate);
    if (predicate.FindParameter(iter2, "team")) {
        predicate.GetValue(iter2, team);
        cout << "FindParameter team: " << team << endl;
    }

    Predicate::Iterator iter3(predicate);
    if (!predicate.FindParameter(iter3, "pm")) {
        return false;
    }

    predicate.GetValue(iter3, Estado);

    if (Estado == STR_PM_PlayOn) {
        return true;
    }
    if (team == "right") {
        if ((Estado == STR_PM_KickOff_Right) || (Estado == STR_PM_KickIn_Right) || (Estado == STR_PM_CORNER_KICK_RIGHT) || (Estado == STR_PM_GOAL_KICK_RIGHT)) {
            return true;
        }
    } else {
        if ((Estado == STR_PM_KickOff_Left) || (Estado == STR_PM_KickIn_Left) || (Estado == STR_PM_CORNER_KICK_LEFT) || (Estado == STR_PM_GOAL_KICK_LEFT)) {
            return true;
        }
    }
    return false;
}

VisionObject SoccerPerceptor::RotarCancha(const VisionObject& obj) const {
    if (team == "left")
        return obj;
    switch (obj) {
        case F1L:return F2R;
        case G1L:return G2R;
        case G2L:return G1R;
        case F2L:return F1R;
        case F1R:return F2L;
        case G1R:return G2L;
        case G2R:return G1L;
        case F2R:return F1L;
        case PL1:return PR1;
        case PL2:return PR2;
        case PL3:return PR3;
        case PL4:return PR4;
        case PL5:return PR5;
        case PR1:return PL1;
        case PR2:return PL2;
        case PR3:return PL3;
        case PR4:return PL4;
        case PR5:return PL5;
        default:
            return obj;
    }
    return obj;
}

int SoccerPerceptor::getUnum() const {
    return unum;
}

string SoccerPerceptor::getEstado() const {
    return Estado;
}

string SoccerPerceptor::getTeam() const {
    return team;
}

salt::Vector3f SoccerPerceptor::getMyPos() const {
    return mMyPos;
}



bool SoccerPerceptor::getSensation(const std::string& message)
{

    //return Forward();
    cout << "message: " << message << endl;
    boost::shared_ptr<PredicateList> predList = mParser->Parse(message);
    if (predList.get() != 0) {
        PredicateList& list = *predList;

        for (
                PredicateList::TList::const_iterator iter = list.begin();
                iter != list.end();
                ++iter) {
            const Predicate& predicate = (*iter);

            // check for the Vision perceptor

            if (predicate.name == "See") {
                ParseVision(predicate);
                continue;
            } else if (predicate.name == "GS") {

                if (!GameState(predicate)) {
                    return false;
                }
            }
        }
    }

    return true;
}
