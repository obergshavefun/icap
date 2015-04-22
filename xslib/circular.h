#ifndef CIRCULAR_H__
#define CIRCULAR_H__


#include "cross_section.h"

namespace xs
{
    class Circular : public CrossSection
    {
    private:
        double diameter;
        double lastDepth;
        double theta;
        double getTheta(double depth);
        void init();

    public:
        Circular();
        Circular(double diameter);
        Circular(const Circular* rhs);

        void setDiameter(double diameter) { this->diameter = diameter; }
        virtual double getMaxDepth() { return this->diameter; }

        virtual bool setParameters(std::vector<std::string>::const_iterator firstPart, std::vector<std::string>::const_iterator end);

        virtual double computeArea(double depth);
        virtual double computeWettedPerimiter(double depth);
        virtual double computeTopWidth(double depth);
        virtual double computeDpDy(double depth);
        virtual double computeDaDy(double depth);
        virtual double computeDtDy(double depth);

        virtual std::shared_ptr<CrossSection> clone();
    };
}


#endif//CIRCULAR_H__