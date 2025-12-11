//
// Created by dawn on 2025/10/24.
//

#include "PropertyConfiguration.hpp"

PropertyConfiguration::PropertyConfiguration(std::shared_ptr<WsfSimulation> simulationPtr)
    : _simulationPtr(simulationPtr)
{

}

PropertyConfiguration::~PropertyConfiguration()
{

}

void PropertyConfiguration::Configuration(std::string sign, std::string key, double value)
{
    WsfPlatform* platform = _simulationPtr->GetPlatformBySign(sign);
    if(platform != nullptr)
    {
        ut::log::info() << " PropertyConfiguration::Configuration -> " << platform->GetName();
    }
//    interface->SetPropertyValue("J-11B-2", "ic/long-gc-deg", 120.0);
//    interface->SetPropertyValue("J-11B-2", "ic/lat-geod-deg", 60.0);
//    interface->SetPropertyValue("J-11B-2", "ic/h-sl-ft", 20000);
}

