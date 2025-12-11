#include "ScenarioTool.hpp"

#include <WsfMoverTypes.hpp>
#include <WsfPlatformTypes.hpp>
#include <WsfProcessorTypes.hpp>
#include <WsfRouteTypes.hpp>
#include <WsfSensor.hpp>
#include <WsfWeapon.hpp>
#include <WsfWeaponTypes.hpp>

void ScenarioTool::PrintTypeInfo(std::shared_ptr<WsfScenario> scenarioPtr)
{
    //! WsfPlatformTypes
    WsfPlatformTypes& platformTypes = scenarioPtr->GetPlatformTypes();
    WsfObjectTypeListBase::TypeIdList platformTypeIds;
    platformTypes.GetTypeIds(platformTypeIds);
    for (auto& platformTypeId : platformTypeIds)
    {
       const std::string& platformType = platformTypeId;
       WsfPlatform* platform =  platformTypes.Find(platformTypeId);

       std::cout << " WsfPlatformType " << platformType << std::endl;

        for (auto component : platform->GetComponents())
        {
           WsfSensor* sensor = dynamic_cast<WsfSensor*>(component);
           if (sensor != nullptr && sensor->IsTurnedOn())
           {
               std::cout << " WsfSensor - " << sensor->GetName() << std::endl;
           }

           WsfWeapon* weapon = dynamic_cast<WsfWeapon*>(component);
           if (weapon != nullptr)
           {
               std::cout << " WsfWeapon - " << weapon->GetName() << std::endl;
           }
        }

    }

    //! WsfMoverTypes
    WsfMoverTypes& moverTypes = scenarioPtr->GetMoverTypes();
    WsfObjectTypeListBase::TypeIdList moverTypeIds;
    moverTypes.GetTypeIds(moverTypeIds);
    for (auto& moverTypeId : moverTypeIds)
    {
       const std::string& moverType = moverTypeId;
       WsfMover* mover = moverTypes.Find(moverTypeId);

        std::cout << " WsfMoverType - " << moverType << std::endl;

    }

    //! WsfProcessorTypes
    WsfProcessorTypes& processorTypes = scenarioPtr->GetProcessorTypes();
    WsfObjectTypeListBase::TypeIdList processorTypeIds;
    processorTypes.GetTypeIds(processorTypeIds);
    for (auto& processorTypeId : processorTypeIds)
    {
       const std::string& processorType = processorTypeId;
       WsfProcessor* processor = processorTypes.Find(processorTypeId);

        std::cout << " WsfProcessorType - " << processorType << std::endl;

    }

    WsfRouteTypes& routeTypes = scenarioPtr->GetRouteTypes();
    WsfObjectTypeListBase::TypeIdList routeTypeIds;
    routeTypes.GetTypeIds(routeTypeIds);
    for (auto& routeTypeId : routeTypeIds)
    {
       const std::string& routeType = routeTypeId;
       WsfRoute* route = routeTypes.Find(routeTypeId);

        std::cout << " WsfRouteType - " << routeType << std::endl;

    }

    WsfWeaponTypes& weaponTypes = WsfWeaponTypes::Get(*scenarioPtr);
    WsfObjectTypeListBase::TypeIdList weaponTypeIds;
    weaponTypes.GetTypeIds(weaponTypeIds);
    for (auto& weaponTypeId : weaponTypeIds)
    {
       const std::string& weaponType = weaponTypeId;
       WsfWeapon* weapon = weaponTypes.Find(weaponTypeId);

        std::cout << " WsfWeaponType - " << weaponType << std::endl;
    }


}
