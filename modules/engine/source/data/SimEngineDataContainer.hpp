#ifndef SIMENGINEDATACONTAINER_HPP
#define SIMENGINEDATACONTAINER_HPP


#include "WsfSimulation.hpp"

class SimEngineInterface;

class SimEngineDataContainer
{
public:
    SimEngineDataContainer();

protected:
   void SimulationClockRead(const std::string sign, const WsfSimulation& aSimulation);
   void SimulationClockWrite(const std::string sign, WsfSimulation& aSimulation);
   void WallClockRead(const std::string sign, const WsfSimulation& aSimulation);
   void WallClockWrite(const std::string sign, WsfSimulation& aSimulation);

protected:
   void SimulationInitializing(const std::string sign, const WsfSimulation& aSimulation);
   void SimulationStarting(const std::string sign, const WsfSimulation& aSimulation);
   void SimulationComplete(const std::string sign, const WsfSimulation& aSimulation);

protected:
   void PlatformInitialized(const std::string sign, double aSimTime, const WsfPlatform& aPlatform);
   void PlatformAdded(const std::string sign, double aSimTime, const WsfPlatform& aPlatform);
   void PlatformDeleted(const std::string sign, double aSimTime, const WsfPlatform& aPlatform);


};

#endif // SIMENGINEDATACONTAINER_HPP
