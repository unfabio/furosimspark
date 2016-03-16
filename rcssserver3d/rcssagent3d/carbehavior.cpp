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
#include <map>

using namespace std;
using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace salt;

#define CASE_NUMBER \
 case'0':case'1':case'2':case'3':case'4':\
 case'5':case'6':case'7':case'8':case'9':case '.':case '-'
#define CASE_ALPHA_LOWER \
 case'a':case'b':case'c':case'd':\
 case'e':case'f':case'g':case'h':\
 case'i':case'j':case'k':case'l':\
 case'm':case'n':case'o':case'p':\
 case'q':case'r':case's':case't':\
 case'u':case'v':case'w':case'x':\
 case'y':case'z'
#define CASE_ALPHA_UPPER \
 case'A':case'B':case'C':case'D':\
 case'E':case'F':case'G':case'H':\
 case'I':case'J':case'K':case'L':\
 case'M':case'N':case'O':case'P':\
 case'Q':case'R':case'S':case'T':\
 case'U':case'V':case'W':case'X':\
 case'Y':case'Z'
#define CASE_ALPHA CASE_ALPHA_UPPER:CASE_ALPHA_LOWER
#define CASE_ALPHANUM CASE_ALPHA:CASE_NUMBER


CarBehavior::CarBehavior() : mZG("." PACKAGE_NAME)
{
}

string CarBehavior::Init()
{
    // use the scene effector to build the agent
  //  return "(scene rsg/agent/buggy.rsg)";
    //return "(scene rsg/agent/furo.rsg)";

    mZG.GetCore()->ImportBundle("sexpparser");

    mParser = static_pointer_cast<BaseParser>
        (mZG.GetCore()->New("SexpParser"));

    if (mParser.get() == 0)
    {
        cerr << "unable to create SexpParser instance." << endl;
    }

    stringstream ss;
    ss << "(scene rsg/agent/furo7x7.rsg) " << endl  << "(init (unum 0) (teamname FuroRobot)) " << endl  << "(beam "<< -10 << " " << 0 << " " << 90 << ")" << endl  << "(syn)";
    srand (time(NULL));

    return ss.str();
}




string CarBehavior::Think(const std::string& message)
{

   std::size_t pos = message.find("(B (pol ");
   std::size_t pos2 = message.find("(pm PlayOn)");

    std::string text = "";
    float BDistancia,BAngulo;

     float velocity1 = 0;
     float velocity2 = 0;




   if(pos>0 &&  (pos2 > 0)&&(pos2 < 1000)){
      text = message.substr(pos+8 );
      std::sscanf(text.c_str(),"%f %f",&BDistancia,&BAngulo);
      cout<< "Distancia " <<BDistancia<< " Angulo " << BAngulo<< std::endl;


      if(BAngulo>0){

         velocity2 = -500/( 1+BAngulo );
         velocity1 = -500;

      }else{
         velocity2 = -500;
         velocity1 = -500/(1-BAngulo);
      }
   }else{

      velocity1 = 0;
      velocity2 = 0;
   }

  cout << ">> " << message << endl;
   cout << ">> " << pos2<< endl;
   stringstream ss;
   //ss <<"(syn)" << endl;
   ss << "(er1 "  << velocity1 << ")(er3 "  << velocity2 << ")";
   return ss.str();
}


/*
string CarBehavior::Think(const std::string& message)
{
    // demo: just drive forward and backward...

    int n = 0;
    static float velocity1 = 0;
    static float velocity2 = 0;
    static float number = 0;
    static int longitud = 0;
    std::string val = "";
    std::string sangria = "                                      ";
    int NUMBER=0;
    int valores[] ={0, 0, 0, 0, 0}

string Perseptions[10][10][10][10];


std::map<string,int> first;

first['time']=0;
first['See']=0;
first['GS']=0;



int pos = 0;
char str1[32];
std::stringstream ss;
while (pos < message.length())
{
   switch (message[pos]) {
      case '(':
         std::cout << std::endl << sangria.substr(0, n*5)<<  "[";
         n++;
         valores[n]++;
         //std::cout << std::endl << sangria.substr(0, n*3)<< n << "[";
         pos++;
      break;
      case ')':
         n--;
         std::cout << std::endl << sangria.substr(0, n*5)<<  "]";
            pos++;
      break;
      CASE_NUMBER:

            //std::getline(ss, message);
            std::stringstream(message.substr(pos))>>number;
            //std::cout << std::endl << "(" << number  << ")" << std::endl;
            std::cout << std::endl << sangria.substr(0, n*5)<< number;

            pos++;
         //std::cout << message[pos];
         break;
         CASE_ALPHA:

                     //std::getline(ss, message);
                     std::stringstream(message.substr(pos))>>val;
                     longitud = 0;
                     //std::cout << std::endl << sangria.substr(0, n*5) << "$" << val <<"$"<< std::endl  ;
                     std::sscanf(val.c_str(),"%30[a-zA-Z0-9]s",str1);
                        val =str1;
                        std::cout << std::endl << sangria.substr(0, n*5) << "\'" << val<<"\'" ;
                        pos+=val.length();


                     if(val.compare("B")==0){
                        //std::cout << std::endl << sangria.substr(0, n*5) << "{BALON}" ;

                     }

                     //std::cout << std::endl << sangria.substr(0, n*5) << "{" << val <<"}" ;
                     //std::cout << std::endl << sangria.substr(0, n*5) << "\"" << val <<"\"" ;

                  //std::cout << message[pos];
                  break;
      default:
         //std::cout << "|" << message[pos];
         pos++;
   }
};
   //message.substr(pos) >> val;

   //std::cout << message.substr(start, end);


   /* boost::shared_ptr<PredicateList> predList =*/
      //  mParser->Parse(message);
/*
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
               //cout << "predicate.name " << predicate.name;
                // check for a joint percept
                switch(predicate.name[0])
                {
                case 'H': // hinge joint (HJ)
               //     ParseHingeJointInfo(predicate);
                    break;
                case 'U': // universal joint (UJ)
                  //  ParseUniversalJointInfo(predicate);
                    break;
                default:
                    break;
                }
            }
        }* /
        cout << ">> " << message << endl;
    ++n;

    if (n > 200)
        {
            velocity1 = ((rand() % 10)-5);
            velocity2 = ((rand() % 10)-5);
            n = 0;
        }
   // stringstream ss;
   // if()
    //ss << "(er1 "  << velocity1 << ")(er3 "  << velocity2 << ")";

    ss <<"(syn)" << endl;

    return ss.str();
}
*/
