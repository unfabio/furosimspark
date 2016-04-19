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
    mVisionObjectMap["Pleft1"] = Pleft1;
    mVisionObjectMap["Pleft2"] = Pleft2;
    mVisionObjectMap["Pleft3"] = Pleft3;
    mVisionObjectMap["Pleft4"] = Pleft4;
    mVisionObjectMap["Pleft5"] = Pleft5;
    mVisionObjectMap["Pright1"] = Pright1;
    mVisionObjectMap["Pright2"] = Pright2;
    mVisionObjectMap["Pright3"] = Pright3;
    mVisionObjectMap["Pright4"] = Pright4;
    mVisionObjectMap["Pright5"] = Pright5;
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
    return "(scene rsg/agent/furo7x7.rsg)";
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
        string strteam;
        if (predicate.GetValue(paramIter, "team", strteam)) {
            if (strteam.find("left") != std::string::npos) {
                name += "left";
            } else if (strteam.find("right") != std::string::npos) {
                name += "right";
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
Vector3f SoccerBehavior::GetDriveVec(const VisionObject& obj) const 
{
    return GetDriveVec(GetVisionSense(obj));
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

bool SoccerBehavior::GameState(const Predicate& predicate) 
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


    VisionObject Objetos[] = {Pleft1, Pleft2, Pleft3, Pleft4, Pleft5, Pright1, Pright2, Pright3, Pright4, Pright5};
    for (int i = 0; i < 5; i++) {
        if (team == "right") {
            Equipo[i] = Objetos[i + 5];
            EquipoOp[i] = Objetos[i];
        } else {
            Equipo[i] = Objetos[i];
            EquipoOp[i] = Objetos[i + 5];
        }
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

string SoccerBehavior::TurnLeft() const 
{
    return Motores(-10, 10);
}

string SoccerBehavior::TurnRight() const 
{
    return Motores(10, -10);
}

string SoccerBehavior::Forward() const 
{
    return Motores(-15, 15);
}

string SoccerBehavior::Kick() const 
{
    return "(kick 0  10)";
}

string SoccerBehavior::Arquero() const
{

    Vector3f b = GetDriveVec(VO_BALL);

    Vector3f g1 = GetDriveVec(RotarCancha(G1L));
    Vector3f g2 = GetDriveVec(RotarCancha(G2L));
    Vector3f centro = g1 + g2, d1, d2, contrario;

    //evitar que quede al costado de la cancha
    if (mMyPos.x()<-24 && gAbs(mMyPos.y()) > 4) {
        return Ir(GetDriveVec(RotarCancha(G1R)));
    }


    //volver al arco si se alejo mucho
    if (centro.Dot(b) > 0 || mMyPos.x()>-5 || gAbs(mMyPos.y()) > 6) {
        return Ir(centro);
    }
    //no se salga de la meta
    if (mMyPos.x()<-26) {
        return Ir(b);
    }
    if (b.Length() < 10) {
        //Si esta cercar intentar botarla al arco contrario
        contrario = GetDriveVec(RotarCancha(G1R));
        contrario.Normalize();
        if (contrario.Dot(b) > .5) {
            return Ir(b);
        }
        return Ir(b - contrario);
    }
    d1 = b - g1;
    d2 = b - g2;
    if (d1.Length() < d2.Length()) {
        return Ir(g1 + g2 * .5 + b * .3);
    } else {
        return Ir(g2 + g1 * .5 + b * .3);
    }

}

string SoccerBehavior::Accion() const
{
    return Motores();
}

VisionObject SoccerBehavior::RotarCancha(const VisionObject& obj) const {
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
        case Pleft1:return Pright1;
        case Pleft2:return Pright2;
        case Pleft3:return Pright3;
        case Pleft4:return Pright4;
        case Pleft5:return Pright5;
        case Pright1:return Pleft1;
        case Pright2:return Pleft2;
        case Pright3:return Pleft3;
        case Pright4:return Pleft4;
        case Pright5:return Pleft5;
        default:
            return obj;
    }
    return obj;
}

string SoccerBehavior::SeekBall() const {
    // const VisionSense& vision = GetVisionSense(VO_BALL);

    Vector3f b = GetDriveVec(VO_BALL);
    VisionObject G1, G2, F1, F2;

    G1 = RotarCancha(G1R);
    G2 = RotarCancha(G2R);
    F1 = RotarCancha(F1L);
    F2 = RotarCancha(F2L);

    Vector3f g1 = GetDriveVec(G1);
    Vector3f g2 = GetDriveVec(G2);
    Vector3f Dir = (g1 + g2) / 2;

    if (mMyPos.x() <-10 && b.Length() > 5) {
        return Ir(Dir);
    }

    //Buscar centrar el Balon si esta cerca de las esquinas del contrario
    if (gAbs(mMyPos.y()) + mMyPos.x() > 28) {
        Dir += (GetDriveVec(F2) + GetDriveVec(F1))*.01;
    }

    Dir -= b;
    Dir = Dir.Normalize();

    float Pesc = Dir.Dot(b) / b.Length(), fact = 0;
    // cout << " Pesc " << Pesc <<" b.Length() " << b.Length() << endl;
    if (Pesc < 0.3 && b.Length() < 2) {
        if (unum % 2 == 0) {
            Dir = GetDriveVec(F1);
        } else {
            Dir = GetDriveVec(F2);
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

string SoccerBehavior::Defensa() const {
    // const VisionSense& vision = GetVisionSense(VO_BALL);

    Vector3f b = GetDriveVec(VO_BALL);
    VisionObject G1, G2, F1, F2;

    G1 = RotarCancha(G1R);
    G2 = RotarCancha(G2R);
    F1 = RotarCancha(F1L);
    F2 = RotarCancha(F2L);

    Vector3f g1 = GetDriveVec(G1);
    Vector3f g2 = GetDriveVec(G2);
    Vector3f Dir = (g1 + g2) / 2;

    if (mMyPos.x() <-25) {
        return Ir(Dir);
    }

    Dir -= b;
    Dir = Dir.Normalize();

    //Buscar centrar el Balon
    if (mMyPos.x() > 0 && b.Length() > 3) {
        if (unum % 2 == 0) {
            return Ir(b + GetDriveVec(RotarCancha(G1L)));
        } else {
            return Ir(b + GetDriveVec(RotarCancha(G2L)));
        }
    }

    float Pesc = Dir.Dot(b) / b.Length(), fact = 0;
    // cout << " Pesc " << Pesc <<" b.Length() " << b.Length() << endl;
    if (Pesc < 0.3 && b.Length() < 2) {
        if (unum % 2 == 0) {
            Dir = GetDriveVec(F1);
        } else {
            Dir = GetDriveVec(F2);
        }
        Dir = Dir.Normalize() - b.Normalize();
    } else if (Pesc > 0.75 && b.Length() < 5) {
        Dir = Dir + b;
    } else {
        fact = -b.Length() *.7;
        Dir = Dir * fact + b;
    }
    return Ir(Dir);
}

string SoccerBehavior::Ir(const salt::Vector3f& Dir) const {
    Vector3f vObj, vObjb, nDir = Dir, b = GetDriveVec(VO_BALL);
    float Dis = vObj.Length();

    for (int i = 0; i < 5; i++) {
        vObj = GetDriveVec(Equipo[i]);
        Dis = vObj.Length();
        vObjb = vObj - b;
        if (Dis > 0.1 && Dis < 10 && b.Length() > vObjb.Length()) {
            vObj.Normalize();
            vObj *= 5.0 / (Dis * Dis);
            nDir -= vObj;
        }
    }
    for (int i = 0; i < 5; i++) {
        vObj = GetDriveVec(EquipoOp[i]);
        Dis = vObj.Length();
        if (Dis > 0.1 && Dis < 2 && b.Length() + 1 > Dis) {
            vObj.Normalize();
            vObj *= 1.0 / (Dis * Dis);
            nDir -= vObj;
        }
    }

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
                    return Motores();
                }
            }
        }
    }

    return Accion();
}
