
#include "SimEngineManager.hpp"

#include <iostream>
#include <memory>

#include "JsonToList.hpp"

#include "wsf_extensions.hpp"

#include "WsfApplication.hpp"
#include "WsfScenario.hpp"

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

SimEngineManager::SimEngineManager(nlohmann::json plugin_paths)
    : _application(new WsfApplication("PyEngine", 0, {}, JsonToList(plugin_paths)))
{
    ut::SetApplicationErrorHandling(&ut::TerminateHandler);

    ut::SetupApplicationLog("py_engine", "1.0.0", "py_engine-exception.log");

    // Load built-in extensions (defined in wsf_extensions.hpp)
    RegisterBuiltinExtensions(*_application);

    // Load optional extensions (defined in wsf_extensions.hpp)
    RegisterOptionalExtensions(*_application);

    // Register the XIO simulation interface.
    WSF_REGISTER_EXTENSION(*_application, xio_interface);

    _scenarioPtr = std::make_shared<WsfScenario>(*_application);
}

void SimEngineManager::LoadScriptTemplate(std::string script_path)
{
    _scenarioPtr->LoadFromFile(script_path);
    _scenarioPtr->CompleteLoad();
    PrintTypeInfo(_scenarioPtr);
}

std::shared_ptr<WsfScenario> SimEngineManager::GetScenario()
{
    return _scenarioPtr;
}

SimEngineManager::~SimEngineManager()
{

}


void SimEngineManager::PrintTypeInfo(std::shared_ptr<WsfScenario> scenarioPtr)
{
    //! WsfPlatformTypes
    WsfPlatformTypes& platformTypes = scenarioPtr->GetPlatformTypes();
    WsfObjectTypeListBase::TypeIdList platformTypeIds;
    platformTypes.GetTypeIds(platformTypeIds);
    for (auto& platformTypeId : platformTypeIds)
    {
        const std::string& platformType = platformTypeId;
        WsfPlatform* platform =  platformTypes.Find(platformTypeId);

        ut::log::info() << " WsfPlatformType - " << platformType;

//       QString platformTypeStr = QString::fromStdString(platformType);
//        if(!platformTypeStr.startsWith("WSF"))
//        {
//            LOGGER_INFO() << " WsfPlatformType - " << QString::fromStdString(platformType);
//        }
//
//
//        for (auto component : platform->GetComponents())
//        {
//           WsfSensor* sensor = dynamic_cast<WsfSensor*>(component);
//           if (sensor != nullptr && sensor->IsTurnedOn())
//           {
//                LOGGER_INFO() << " WsfSensor - " << QString::fromStdString(sensor->GetName());
//           }
//
//           WsfWeapon* weapon = dynamic_cast<WsfWeapon*>(component);
//           if (weapon != nullptr)
//           {
//               LOGGER_INFO() << " WsfWeapon - " << QString::fromStdString(weapon->GetName());
//           }
//        }

    }

    //! WsfMoverTypes
    WsfMoverTypes& moverTypes = scenarioPtr->GetMoverTypes();
    WsfObjectTypeListBase::TypeIdList moverTypeIds;
    moverTypes.GetTypeIds(moverTypeIds);
    for (auto& moverTypeId : moverTypeIds)
    {
        const std::string& moverType = moverTypeId;
        WsfMover* mover = moverTypes.Find(moverTypeId);

        ut::log::info() << " WsfMoverType - " << moverType;

//       QString moverTypeStr = QString::fromStdString(moverType);
//        if(!moverTypeStr.startsWith("WSF"))
//        {
//            LOGGER_INFO() << " WsfMoverType - " << QString::fromStdString(moverType);
//        }

    }

    //! WsfProcessorTypes
    WsfProcessorTypes& processorTypes = scenarioPtr->GetProcessorTypes();
    WsfObjectTypeListBase::TypeIdList processorTypeIds;
    processorTypes.GetTypeIds(processorTypeIds);
    for (auto& processorTypeId : processorTypeIds)
    {
        const std::string& processorType = processorTypeId;
        WsfProcessor* processor = processorTypes.Find(processorTypeId);

        ut::log::info() << " WsfProcessorType - " << processorType;

//       QString processorTypeStr = QString::fromStdString(processorType);
//        if(!processorTypeStr.startsWith("WSF"))
//        {
//            LOGGER_INFO() << " WsfProcessorType - " << QString::fromStdString(processorType);
//        }

    }

    WsfRouteTypes& routeTypes = scenarioPtr->GetRouteTypes();
    WsfObjectTypeListBase::TypeIdList routeTypeIds;
    routeTypes.GetTypeIds(routeTypeIds);
    for (auto& routeTypeId : routeTypeIds)
    {
        const std::string& routeType = routeTypeId;
        WsfRoute* route = routeTypes.Find(routeTypeId);

        ut::log::info() << " WsfRouteType - " << routeType;

//       QString routeTypeStr = QString::fromStdString(routeType);
//        if(!routeTypeStr.startsWith("WSF"))
//        {
//            LOGGER_INFO() << " WsfRouteType - " << QString::fromStdString(routeType);
//        }

    }

    WsfWeaponTypes& weaponTypes = WsfWeaponTypes::Get(*scenarioPtr);
    WsfObjectTypeListBase::TypeIdList weaponTypeIds;
    weaponTypes.GetTypeIds(weaponTypeIds);
    for (auto& weaponTypeId : weaponTypeIds)
    {
        const std::string& weaponType = weaponTypeId;
        WsfWeapon* weapon = weaponTypes.Find(weaponTypeId);

        ut::log::info() << " WsfWeaponType - " << weaponType;

//       QString weaponTypeStr = QString::fromStdString(weaponType);
//        if(!weaponTypeStr.startsWith("WSF"))
//        {
////            LOGGER_INFO() << " WsfWeaponType - " << QString::fromStdString(weaponType);
//        }

    }


}
