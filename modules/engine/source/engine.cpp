#include <iostream>
#include <memory>
#include <sstream>

#include "UtException.hpp"
#include "UtLog.hpp"
#include "WsfScenario.hpp"
#include "WsfSimulation.hpp"
#include "WsfSimulationInput.hpp"
#include "WsfStandardApplication.hpp"

// Includes all of the optional projects which can be compiled with WSF
#include "ProfilingCommon.hpp"
#include "ProfilingRegion.hpp"
#include "ProfilingSystem.hpp"
#include "wsf_extensions.hpp"
#include "wsf_version_defines.hpp"

#include "SimEngineManager.hpp"
#include "SimEngineInterface.hpp"

#include "SimEngineDataContainer.hpp"



int main(int argc, char* argv[])
{
    ut::SetApplicationErrorHandling(&ut::TerminateHandler);

    ut::SetupApplicationLog("engine", WSF_VERSION, "engine-exception.log");

    std::string  resources_path = "F:/Work/Simulation/HSimServiceFramework/build/resources";

//    std::string script_template_path = resources_path + "/template/SU-27/tests/p6dof/su-27_scenario.txt";
    std::string script_template_path = "F:\\Work\\PyProject\\AgentTestFramework\\envs\\Engine\\scripts\\p6dof\\tactical_maneuvers.txt";
//    std::string script_template_path = "F:\\Work\\PyProject\\AgentTestFramework\\envs\\Engine\\scripts\\SU-27\\0\\tests\\p6dof\\su-27_scenario.txt";

    std::list<std::string> plugin_paths;

    std::shared_ptr<SimEngineManager> manager = std::make_shared<SimEngineManager>(plugin_paths) ;
    manager->LoadScriptTemplate(script_template_path);

    //    SimEngineInterface* simEngineInterface = new SimEngineInterface("xxxxxxxxxxxx", scenarioPtr, cEVENT_STEPPED);
    SimEngineInterface* interface = new SimEngineInterface("xxxxxxxxxxxx", manager->GetScenario(), SimEngineKernel::cFRAME_STEPPED);
//    SimEngineInterface* simEngineInterface = new SimEngineInterface("xxxxxxxxxxxx", scenarioPtr, cREAL_TIME);

////    # 设置年月日
//    interface->SetStartDate(2024, 11, 11);
////    # 设置时分秒
//    interface->SetStartTime(12, 00, 00);
////    # 设置结束时间
//    interface->SetEndTime(86400);

    nlohmann::json platform_array;

    {
        nlohmann::json platform_obj;
        platform_obj["sign"] = "J-11B-1";
        platform_obj["name"] = "J-11B-1",
        platform_obj["type"] = "RedFighter";
//        platform_obj["type"] = "SU-27_P6DOF";
        platform_obj["side"] = "Red";
        platform_obj["icon"] = "Air";
        platform_array.push_back(platform_obj);
    }

    {
        nlohmann::json platform_obj;
        platform_obj["sign"] = "J-11B-2";
        platform_obj["name"] = "J-11B-2",
        platform_obj["type"] = "BlueFighter";
//        platform_obj["type"] = "SU-27_P6DOF";
        platform_obj["side"] = "Blud";
        platform_obj["icon"] = "Air";
        platform_array.push_back(platform_obj);
    }

    interface->AddInputPlatforms(platform_array);

//    interface->SetPropertyValue("J-11B-2", "ic/long-gc-deg", 120.0);
//    interface->SetPropertyValue("J-11B-2", "ic/lat-geod-deg", 60.0);
//    interface->SetPropertyValue("J-11B-2", "ic/h-sl-ft", 20000);

    int step = 1;
    while (true)
    {
        if(!interface->IsActive())
            break;
        ut::log::info() << "step - " << step;
        interface->Step();

        ut::log::info() << interface->GetSimInfo();

        step++;
    }

    interface->Complete();

   return 0;
}
