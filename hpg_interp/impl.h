// ==============================================================================
// ICAP License
// ==============================================================================
// University of Illinois/NCSA
// Open Source License
// 
// Copyright (c) 2014-2016 University of Illinois at Urbana-Champaign.
// All rights reserved.
// 
// Developed by:
// 
//     Nils Oberg
//     Blake J. Landry, PhD
//     Arthur R. Schmidt, PhD
//     Ven Te Chow Hydrosystems Lab
// 
//     University of Illinois at Urbana-Champaign
// 
//     https://vtchl.illinois.edu
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal with
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// 
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimers.
// 
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimers in the
//       documentation and/or other materials provided with the distribution.
// 
//     * Neither the names of the Ven Te Chow Hydrosystems Lab, University of
// 	  Illinois at Urbana-Champaign, nor the names of its contributors may be
// 	  used to endorse or promote products derived from this Software without
// 	  specific prior written permission.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
// SOFTWARE.



#include <deque>

#include "hpg.hpp"
#include "spline.h"


namespace hpg
{
    class Hpg::Impl
    {
    public:
        unsigned int posFlowCount;            //< number of positive flows in HPG
        unsigned int advFlowCount;            //< number of adverse flows in HPG
        std::deque<double> posFlows;  //< list of flows in HPG
        std::deque<double> advFlows;  //< list of flows in HPG
        std::deque<hpgvec> posValues; //< list of vectors of HP curves
        std::deque<hpgvec> advValues; //< list of vectors of HP curves
        hpgvec posCritical;           //< critical line for positive flow
        hpgvec advCritical;           //< critical line for adverse flow
        //hpgvec ZeroFlowLine;          //< zero-flow line (for backwater effects)
        //hpgvec NormFlowLine;          //< normal-flow line
        double minPosFlow;            //< minimum positive flow in HPG
        double maxPosFlow;            //< maximum positive flow in HPG
        double minAdvFlow;            //< minimum adverse flow in HPG
        double maxAdvFlow;            //< maximum adverse flow in HPG
        // Splines are stored here.
        std::deque<Spline> SplPosUS_QDS;       //< spline for US = F(Q, DS) for positive flow
        std::deque<Spline> SplAdvUS_QDS;       //< spline for US = F(Q, DS) for adverse flow
        std::deque<Spline> SplPosDS_QUS;       //< spline for US = F(Q, DS) for positive flow
        std::deque<Spline> SplAdvDS_QUS;       //< spline for US = F(Q, DS) for adverse flow
        std::deque<Spline> SplPosVol;          //< spline for Volume = F(Q, DS) for positive flow
        std::deque<Spline> SplAdvVol;          //< spline for Volume = F(Q, DS) for adverse flow
		std::deque<Spline> SplPosHf;
		std::deque<Spline> SplAdvHf;
        Spline SplPosCritUS_Q;   //< spline for Q = F_crit(US) for positive flow
        Spline SplAdvCritUS_Q;   //< spline for Q = F_crit(US) for adverse flow
        Spline SplPosCritDS_Q;   //< spline for Q = F_crit(DS) for positive flow
        Spline SplAdvCritDS_Q;   //< spline for Q = F_crit(DS) for adverse flow
        std::deque<Spline>SplPosCritUS_DS;     //< spline for DS = F_crit(US) for positive flow
        std::deque<point> SplPosCritUS_DS_ranges;   //< range of flows for each of the splines in SplPosCritUS_DS
        Spline SplAdvCritUS_DS;  //< spline for DS = F_crit(US) for adverse flow
        int errorCode;

        std::string nodeId; /**< the Tunnel ID */
        double	dsInvert; /**< downstream channel bottom elevation - used for HPG header */
        bool	dsInvertValid;
        double	usInvert; /**< upstream channel bottom elevation */
        bool	usInvertValid;
        double	dsStation; /**< downstream stationing */
        bool	dsStationValid;
        double	usStation; /**< upstream stationing */
        bool	usStationValid;
        double	slope; /**< channel slope */
        bool	slopeValid;
        double	length; /**< channel length */
        bool	lengthValid;
        double	roughness; /**< channel roughness */
        bool	roughnessValid;
        double	maxDepth; /**< channel diameter */
        bool	maxDepthValid;
        double	unsteadyDepthPct; /**< maximum depth in percentage */
        bool	unsteadyDepthPctValid;

        int     version;

        void copyFrom(const Impl* copy)
        {
            this->dsInvertValid = copy->dsInvertValid;
            this->usInvertValid = copy->usInvertValid;
            this->dsStationValid = copy->dsStationValid;
            this->usStationValid = copy->usStationValid;
            this->slopeValid = copy->slopeValid;
            this->lengthValid = copy->lengthValid;
            this->roughnessValid = copy->roughnessValid;
            this->maxDepthValid = copy->maxDepthValid;
            this->unsteadyDepthPctValid = copy->unsteadyDepthPctValid;
            this->dsInvert = copy->dsInvert;
            this->usInvert = copy->usInvert;
            this->dsStation = copy->dsStation;
            this->usStation = copy->usStation;
            this->slope = copy->slope;
            this->length = copy->length;
            this->roughness = copy->roughness;
            this->maxDepth = copy->maxDepth;
            this->unsteadyDepthPct = copy->unsteadyDepthPct;
            this->nodeId = copy->nodeId;

            // Copy all of the primitives and STL arrays.
            this->advFlowCount = copy->advFlowCount;
            this->posFlowCount = copy->posFlowCount;
            this->advFlows = copy->advFlows;
            this->posFlows = copy->posFlows;
            this->advValues = copy->advValues;
            this->posValues = copy->posValues;
            this->posCritical = copy->posCritical;
            this->advCritical = copy->advCritical;
            this->errorCode = copy->errorCode;
            this->minPosFlow = copy->minPosFlow;
            this->maxPosFlow = copy->maxPosFlow;
            this->minAdvFlow = copy->minAdvFlow;
            this->maxAdvFlow = copy->maxAdvFlow;
            this->version = copy->version;
        }
    };
}

