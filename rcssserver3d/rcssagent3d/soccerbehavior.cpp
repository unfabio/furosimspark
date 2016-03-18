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

SoccerBehavior::SoccerBehavior() : mZG("." PACKAGE_NAME), mMyPos(0,0,0)
{
}

SoccerBehavior::~SoccerBehavior()
{
}

void SoccerBehavior::SetupVisionObjectMap()
{
    mVisionObjectMap.clear();
    mVisionObjectMap["B"]    = VO_BALL;
}

string SoccerBehavior::Init()
{
    mZG.GetCore()->ImportBundle("sexpparser");
    mParser = static_pointer_cast<BaseParser>
        (mZG.GetCore()->New("SexpParser"));

    if (mParser.get() == 0)
        {
            cerr << "unable to create SexpParser instance." << endl;
        }

    SetupVisionObjectMap();

    // use the scene effector to build the agent and beam to a
    // position near the center of the playing field
    return
   //     "(scene rsg/agent/soccerplayer.rsg)" ;
         "(scene rsg/agent/furo7x7.rsg)" ;
      //  "(beam -6 0 0)";

;
}

void SoccerBehavior::ParseObjectVision(const Predicate& predicate)
{
    for (
         Predicate::Iterator iter(predicate);
         iter != iter.end();
         ++iter
         )
        {
            // extract the element as a parameter list
            Predicate::Iterator paramIter = iter;
            if (! predicate.DescentList(paramIter))
                {
                    continue;
                }

            // read the object name
            string name;
            if (! predicate.GetValue(paramIter,name))
            {
                continue;
            }

            // try read the 'id' section
            string strId;
            if (predicate.GetValue(paramIter,"id", strId))
                {
                    name += strId;
                }

            // try to lookup the VisionObject
            TVisionObjectMap::iterator visiter = mVisionObjectMap.find(name);
            if (visiter == mVisionObjectMap.end())
                {
                    continue;
                }

            VisionObject vo = (*visiter).second;

            // find the 'pol' entry in the object's section
            Predicate::Iterator polIter = paramIter;
            if (! predicate.FindParameter(polIter,"pol"))
                {
                    continue;
                }

            // read the position vector
            VisionSense sense;
            if (
                (! predicate.AdvanceValue(polIter,sense.distance)) ||
                (! predicate.AdvanceValue(polIter,sense.theta)) ||
                (! predicate.AdvanceValue(polIter,sense.phi))
                )
            {
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

    if (iter == mVisionMap.end())
        {
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
    return Vector3f
        (
         vision.distance * gCos(gDegToRad(vision.theta)) *
         gSin(gDegToRad(90.0f - vision.phi)),

         vision.distance * gSin(gDegToRad(vision.theta)) *
         gSin(gDegToRad(90.0f - vision.phi)),

         vision.distance * gCos(gDegToRad(90.0f - vision.phi))
         );
}

void SoccerBehavior::ParseVision(const Predicate& predicate)
{
    ParseObjectVision(predicate);

    // find the PerfectVision data about the object
    Predicate::Iterator iter(predicate);

    // advance to the section about object 'name'

    //if (! predicate.FindParameter(iter,"B"))
        if (! predicate.FindParameter(iter,"F1L"))
        {
            return;
        }

    // read my position
    VisionSense sense;

    predicate.GetValue(iter,mMyPos);
}


bool SoccerBehavior::GameState(const Predicate& predicate)
{
    ParseObjectVision(predicate);

    // find the PerfectVision data about the object
    Predicate::Iterator iter(predicate);

    // advance to the section about object 'name'

    if (! predicate.FindParameter(iter,"pm"))
        {
            return false;
        }

    // read my position
   string Estado;

    predicate.GetValue(iter,Estado);

    if(Estado!="PlayOn"){

       return false;
     }
     return true;

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
    return "(er1 -50)(er3 -50)";
}

string SoccerBehavior::Kick() const
{
//     std::cerr << "kick";
    return "(kick 0  10)";
}

string SoccerBehavior::SeekBall() const
{
    const VisionSense& vs = GetVisionSense(VO_BALL);
    float d = gAbs(vs.theta);

    if (d > 90)
        {
            return TurnLeft();
        } else if (d < 5)
        {
            return Forward();
        } else
        {
            if (vs.theta <  0)
                {
                    return TurnRight();
                } else
                {
                    return TurnLeft();
                }
        }
}




string SoccerBehavior::Think(const std::string& message)
{

   //return Forward();

   cout << message << endl;
    boost::shared_ptr<PredicateList> predList =
        mParser->Parse(message);

    if (predList.get() != 0)
    {
        PredicateList& list = *predList;

        for (
             PredicateList::TList::const_iterator iter = list.begin();
             iter != list.end();
             ++iter
             )
            {
                const Predicate& predicate = (*iter);

                // check for the Vision perceptor

              if (predicate.name == "See")
               {
                     ParseVision(predicate);
                     continue;
               }else if (predicate.name == "GS")
               {


                  if(!GameState(predicate)){
                     return "(er1 0)(er3 0)";
                  }

               }
            }
    }

    const VisionSense& vs = GetVisionSense(VO_BALL);

/*    stringstream ss;
   ss <<"(syn)" << endl;
   //ss << "(er1 "  << velocity1 << ")(er3 "  << velocity2 << ")";
   return ss.str();*/

   /* if (vs.distance <= 1.5)
        {
            return Kick();
        } else
        {*/
            return SeekBall();
       /* }*/
}
