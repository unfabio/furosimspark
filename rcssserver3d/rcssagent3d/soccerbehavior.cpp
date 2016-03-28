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
    : mZG("." PACKAGE_NAME)
    , mMyPos(0, 0, 0)
{
}

SoccerBehavior::~SoccerBehavior()
{
}

void SoccerBehavior::SetupVisionObjectMap()
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
}

string SoccerBehavior::Init()
{
    mZG.GetCore()->ImportBundle("sexpparser");
    mParser = static_pointer_cast<BaseParser>(mZG.GetCore()->New("SexpParser"));

    if (mParser.get() == 0) {
        cerr << "unable to create SexpParser instance." << endl;
    }

    SetupVisionObjectMap();

    // use the scene effector to build the agent and beam to a
    // position near the center of the playing field
    return
    //     "(scene rsg/agent/soccerplayer.rsg)" ;
    "(scene rsg/agent/furo7x7.rsg)";
    //  "(beam -6 0 0)";
    ;
}

void SoccerBehavior::ParseObjectVision(const Predicate& predicate)
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
        string strId;
        if (predicate.GetValue(paramIter, "id", strId)) {
            name += strId;
        }

        // try to lookup the VisionObject
        TVisionObjectMap::iterator visiter = mVisionObjectMap.find(name);
        if (visiter == mVisionObjectMap.end()) {
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
        mVisionMap[vo] = sense;
    }
}

const SoccerBehavior::VisionSense& SoccerBehavior::GetVisionSense(VisionObject obj) const
{
    static VisionSense invalidSense;

    TVisionMap::const_iterator iter = mVisionMap.find(obj);

    if (iter == mVisionMap.end()) {
        cerr << "unknown VisionObject " << obj << "\n";
        return invalidSense;
    }

    return (*iter).second;
}

Vector3f SoccerBehavior::GetPosition(const VisionSense& sense) const
{
    return mMyPos + GetDriveVec(sense) * sense.distance;
}

Vector3f SoccerBehavior::GetPosition(const VisionObject& obj) const
{
    return GetPosition(GetVisionSense(obj));
}

Vector3f SoccerBehavior::GetDriveVec(const VisionSense& vision) const
{
    return Vector3f(
        vision.distance * gCos(gDegToRad(vision.theta)) * gSin(gDegToRad(90.0f - vision.phi)),

        vision.distance * gSin(gDegToRad(vision.theta)) * gSin(gDegToRad(90.0f - vision.phi)),

        vision.distance * gCos(gDegToRad(90.0f - vision.phi)));
}

void SoccerBehavior::ParseVision(const Predicate& predicate)
{
    ParseObjectVision(predicate);
    return;
    // find the PerfectVision data about the object
    Predicate::Iterator iter(predicate);

    // advance to the section about object 'name'

    if (! predicate.FindParameter(iter,"mypos")){
        return;
    }

    // read my position
    VisionSense sense;

    predicate.GetValue(iter, mMyPos);
}

bool SoccerBehavior::GameState(const Predicate& predicate)
{
    // find the PerfectVision data about the object
    Predicate::Iterator iter(predicate);

    // advance to the section about object 'name'

    string Value;
    if (predicate.FindParameter(iter, "unum")) {
         predicate.GetValue(iter, Value);
         unum= stoi(Value);
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
    // read my position
    predicate.GetValue(iter3, Estado);

   if (Estado ==STR_PM_PlayOn) {
       return true;
   }
   if(team == "right"){
      if((Estado == STR_PM_KickOff_Right) || (Estado == STR_PM_KickIn_Right) || (Estado == STR_PM_CORNER_KICK_RIGHT) || (Estado ==STR_PM_GOAL_KICK_RIGHT)){
         return true;
      }
   }else{
      if((Estado == STR_PM_KickOff_Left) || (Estado == STR_PM_KickIn_Left) || (Estado == STR_PM_CORNER_KICK_LEFT) || (Estado ==STR_PM_GOAL_KICK_LEFT)){
         return true;
      }
   }
    return false;
}

string SoccerBehavior::TurnLeft() const
{
    //     std::cerr << "turn left" << std::endl;
    return "(er1 -10)(er3 10)";
}

string SoccerBehavior::TurnRight() const
{
    //     std::cerr << "turn right" << std::endl;
    return "(er1 10)(er3 -10)";
}

string SoccerBehavior::Forward() const
{
    //     std::cerr << "forward" << std::endl;
    return "(er1 -15)(er3 -15)";
}

string SoccerBehavior::Kick() const
{
    //     std::cerr << "kick";
    return "(kick 0  10)";
}

string SoccerBehavior::SeekBall() const
{
    // const VisionSense& vs = GetVisionSense(VO_BALL);

    Vector3f b = GetPosition(VO_BALL);
    VisionObject G1, G2,F1,F2;
    if(team=="right"){
      G1=G1L;
      G2=G2L;
      F1=F1R;
      F2=F2R;
   }else{
        G1=G1R;
        G2=G2R;
        F1=F1L;
        F2=F2L;
   }
    Vector3f g1 = GetPosition(G1);
    Vector3f g2 = GetPosition(G2);
    Vector3f Dir = (g1 + g2) / 2-b;
    //   Vector3f Dir(10,1,0);
    // VisionSense sense;
    Dir = Dir.Normalize();
    float Pesc = (Dir.x() * b.x() + Dir.y() * b.y() + Dir.z() * b.z()) / b.Length(), fact=0;
   // cout << " Pesc " << Pesc <<" b.Length() " << b.Length() << endl;
    if (Pesc < 0.3 && b.Length() < 10) {
        g2 = GetPosition(F2);
        g1 = GetPosition(F1);
        if (g1.Length() < g2.Length()) {
            Dir = g1;
        }
        else {
            Dir = g2;
        }
    }
    else if(Pesc>0.9 && b.Length()<5) {
        Dir = Dir + b;
    }
   else {
      fact = -b.Length() *.3;
      Dir = Dir *fact + b;
   }
   return Ir(Dir);
}

string SoccerBehavior::Ir(const salt::Vector3f& Dir) const
{
       double theta = salt::gArcTan2(Dir[1], Dir[0]) * 57.2957;
       float d = gAbs(theta);
       float v1 = 0;
       float v2 = 0;
       float vmax = 0;
       if (d > 90) {
          if (theta > 0) {
             theta -= 180;
          }
          else {
             theta += 180;
          }
          d-=90;
          vmax=d*d/200-10;
       }else{
          d=90-d;
          vmax=-d*d/200+10;
       }
       v1 = vmax - theta*.25;
       v2 = vmax + theta*.25;
       stringstream ss;
       //ss <<"(syn)" << endl;
       ss << "(er1 " << v1 << ")(er3 " << v2 << ")";
       return ss.str();
}

string SoccerBehavior::Think(const std::string& message)
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
            }
            else if (predicate.name == "GS") {

                if (!GameState(predicate)) {
                    return "(er1 0)(er3 0)";
                }
            }
        }
    }

   if(unum!=4){
      return Ir(GetPosition(F2R));
   }if(unum==4){
      return SeekBall();
   }else{
      return "(er1 1)(er3 -1)";
   }
}
