#ifndef SIMENGINECOMMANDCONTAINER_HPP
#define SIMENGINECOMMANDCONTAINER_HPP


#include "WsfSimulation.hpp"

class SimEngineInterface;

class SimEngineCommandContainer
{
public:
    SimEngineCommandContainer();

private:
    double _lastSimulationTime_sec;


};

#endif // SIMENGINECOMMANDCONTAINER_HPP
