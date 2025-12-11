#include "ScenarioAnalysis.hpp"

#include <WsfAirMover.hpp>
#include <WsfMoverTypes.hpp>

#include <WsfPlatformTypes.hpp>
#include <WsfProcessorTypes.hpp>
#include <WsfRouteTypes.hpp>
#include <WsfSensor.hpp>
#include <WsfWeaponTypes.hpp>
#include <WsfZoneDefinition.hpp>
#include <WsfZoneTypes.hpp>
#include <WsfSimulation.hpp>

#include <SimEngineInterface.hpp>

//#include <WsfP6DOF_Mover.hpp>

ScenarioAnalysis::ScenarioAnalysis(std::shared_ptr<WsfScenario> scenarioPtr)
    : _scenarioPtr(scenarioPtr)
{

}

void ScenarioAnalysis::AnalysisPlatform(std::list<WsfPlatform*>& platforms, nlohmann::json platform_array)
{
    WsfPlatformTypes& platformTypes = _scenarioPtr->GetPlatformTypes();

    WsfMoverTypes& moverTypes = _scenarioPtr->GetMoverTypes();

    WsfProcessorTypes& processorTypes = _scenarioPtr->GetProcessorTypes();

    WsfRouteTypes& routeTypes = _scenarioPtr->GetRouteTypes();

    for (auto platform_obj : platform_array) {

        if(platform_obj.contains("sign") &&
            platform_obj.contains("name") &&
            platform_obj.contains("type") &&
            platform_obj.contains("side") &&
            platform_obj.contains("icon"))
        {
            std::string sign = platform_obj.at("sign");
            std::string name = platform_obj.at("name");
            std::string type = platform_obj.at("type");
            std::string side = platform_obj.at("side");
            std::string icon = platform_obj.at("icon");

            WsfPlatform* platformPtr = platformTypes.Clone(type);

            if (platformPtr != nullptr)
            {
                platformPtr->SetIndex(static_cast<unsigned int>(0));
                platformPtr->SetSign(sign);
                platformPtr->SetName(name);
                platformPtr->SetSide(side);
                platformPtr->SetIcon(icon);

                //! 设置不可摧毁
                platformPtr->SetIsIndestructible(false);
                platforms.push_back(platformPtr);

                ut::log::info() << " AddPlatform -> " << platform_obj.dump(4);
            }
        }
    }
}




