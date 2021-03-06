// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2015 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later.
 *
 */

#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <yarp/math/Math.h>

#include <hapticdevice/IHapticDevice.h>

using namespace std;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;
using namespace hapticdevice;


/**********************************************************/
class TestModule: public RFModule
{
protected:
    PolyDriver driver;
    IHapticDevice *igeo;

    Vector force;
    double t0;
    
public:
    /**********************************************************/
    bool configure(ResourceFinder &rf)
    {
        string mode=rf.check("mode",Value("physical")).asString().c_str();

        Property option;
        if (mode=="physical")
            option.put("device","geomagicdriver");
        else
        {
            option.put("device","hapticdeviceclient"); 
            option.put("remote","/hapticdevice");
            option.put("local","/client");
        }

        if (!driver.open(option))
            return false;

        driver.view(igeo);
        igeo->setCartesianForceMode();

        igeo->getMaxFeedback(force);

        force[0]/=3.0;
        force[1]=force[2]=0.0;

        t0=Time::now();
        return true;
    }

    /**********************************************************/
    bool close()
    {
        igeo->stopFeedback();
        yInfo("stopping feedback control");

        driver.close();
        return true;
    }

    /**********************************************************/
    double getPeriod()
    {
        return 0.1;
    }

    /**********************************************************/
    bool updateModule()
    {
        double t=Time::now();
        if (t-t0>2.0)
        {
            force=-1.0*force;
            t0=t;
        }

        igeo->setFeedback(force);
        yInfo("applying (%s) [N] force feedback",
              force.toString(3,3).c_str());

        return true; 
    }
};


/**********************************************************/
int main(int argc,char *argv[])
{
    Network yarp;
    if (!yarp.checkNetwork())
    {
        yError("YARP server not found!");
        return 1;
    }
    
    ResourceFinder rf;
    rf.configure(argc,argv);

    TestModule test;
    return test.runModule(rf);
}


