#include <iostream>
#include <format>
#include <list>

#include "SimEngineDataContainer.hpp"


//#include <P6DofGuidanceAutopilot.hpp>
//#include <P6DofHardwareAutopilot.hpp>
#include <WsfP6DOF_Mover.hpp>

#include "UtLog.hpp"

#include "json/json.hpp"

#include "WsfDateTime.hpp"

#include "ObservationSpaceCalculateUtils.hpp"

#define M_PI 3.14159265358979323846

SimEngineDataContainer::SimEngineDataContainer()
{

}

void SimEngineDataContainer::SimulationClockRead(const std::string sign, const WsfSimulation &aSimulation)
{

}

void SimEngineDataContainer::SimulationClockWrite(const std::string sign, WsfSimulation &aSimulation)
{


}

void SimEngineDataContainer::WallClockRead(const std::string sign, const WsfSimulation& aSimulation)
{
   double mSimTime = aSimulation.GetSimTime();

   UtCalendar       mWallTime;
   aSimulation.GetDateTime().GetCurrentTime(mSimTime, mWallTime);

//   ut::log::info() << "SimTime: " << mSimTime;

    const WsfDateTime& dateTime = aSimulation.GetDateTime();

//   double mEndTime    = aSimulation.GetEndTime();
//   WsfSimulation::State mSimState = aSimulation.GetState();
//   bool mPaused     = aSimulation.IsActive() && aSimulation.GetClockSource()->IsStopped();
//   double mClockRate  = aSimulation.GetClockRate();
//   double mTimeBehind = aSimulation.GetTimeBehind();
//   bool mIsRealTime = aSimulation.IsRealTime();

//   WsfDisInterface* disInterface = WsfDisInterface::Find(aSimulation);



    nlohmann::json sim_data;

    sim_data["sim_time"] = mSimTime;

    nlohmann::json platform_array;
    for (size_t i = 0; i < aSimulation.GetPlatformCount(); ++i)
    {
        WsfPlatform* platformPtr = aSimulation.GetPlatformEntry(i);
        platformPtr->Update(aSimulation.GetSimTime());

        double lat, lon, alt;
        platformPtr->GetLocationLLA(lat, lon, alt);

        double heading, pitch, roll;
        platformPtr->GetOrientationNED(heading, pitch, roll);

        double roll_deg = roll * (180.0 / M_PI);
        double pitch_deg = pitch * (180.0 / M_PI);
        double yaw_deg = heading * (180.0 / M_PI);

        nlohmann::json platform_obj;
        platform_obj["sign"] = platformPtr->GetSign();
        platform_obj["name"] = platformPtr->GetName();
        platform_obj["side"] = platformPtr->GetSide();
        platform_obj["icon"] = platformPtr->GetIcon();
        platform_obj["type"] = platformPtr->GetType();
        platform_obj["lat"] = lat;
        platform_obj["lon"] = lon;
        platform_obj["alt"] = alt;
        platform_obj["yaw"] = yaw_deg;
        platform_obj["pitch"] = pitch_deg;
        platform_obj["roll"] = roll_deg;


        if (nullptr != platformPtr) {
            WsfMover *mover = platformPtr->GetMover();
            if (nullptr != mover) {
                if (mover->IsA_TypeOf("WSF_P6DOF_MOVER")) {
                    WsfP6DOF_Mover *p6dofMover = dynamic_cast<WsfP6DOF_Mover *>(mover);

                    P6DofVehicle *vehicle = p6dofMover->GetP6DofVehicle();
                    if (vehicle != nullptr) {
                        P6DofPilotManager *pilotMgr = vehicle->GetPilotManager();
                        if (pilotMgr != nullptr) {
                            // We need an active manual pilot to proceed
                            P6DofPilotObject *pilot = pilotMgr->GetActiveManualPilot();
                            if (pilot != nullptr) {
                                p6dofMover->EnableControls(false);
                                p6dofMover->EnableAutopilot(true);

                                if (pilot->ControlAugmentationModeIsActive()) {
                                    p6dofMover->SetRollRateMax(180.0);
                                    p6dofMover->SetPitchGLoadMax(8.0);
                                }

                            }
                        }
                    }

                    double plat_pursuit_fighter_state_info[8];
                    ObservationSpaceCalculateUtils::CalculateCurrentStateInfo(p6dofMover, plat_pursuit_fighter_state_info);

//                    ut::log::info()

                }
            }
        }

        platform_array.push_back(platform_obj);

//        ut::log::info() << "Platform: name=" << platformPtr->GetName() << ", lat, lon, alt=[" << lat << "," << lon << "," << alt;
//        ut::log::info() << "Platform: name=" << platformPtr->GetName() << ", heading, pitch, roll=[" << heading << "," << pitch << "," << roll;

    }
    sim_data["platforms"] = platform_array;

    ut::log::info() << sim_data;

    std::list<std::string> render_data;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << mSimTime;
    std::string result = oss.str();
    render_data.push_back("#" + result + "\n");
}

void SimEngineDataContainer::WallClockWrite(const std::string sign, WsfSimulation &aSimulation)
{

}

void SimEngineDataContainer::SimulationInitializing(const std::string sign, const WsfSimulation& aSimulation)
{
//    LOGGER_DEBUG() << " SimulationInitializing ";
}

void SimEngineDataContainer::SimulationStarting(const std::string sign, const WsfSimulation& aSimulation)
{
//    LOGGER_DEBUG() << " SimulationStarting ";
}

void SimEngineDataContainer::SimulationComplete(const std::string sign, const WsfSimulation& aSimulation)
{
//    LOGGER_DEBUG() << " SimulationComplete ";

}

void SimEngineDataContainer::PlatformInitialized(const std::string sign, double aSimTime, const WsfPlatform &aPlatform)
{
//    LOGGER_DEBUG() << " PlatformInitialized => " << aSimTime << " " << " Name " << QString::fromStdString(aPlatform.GetName());
//    LOGGER_DEBUG() << " PlatformInitialized => " << aSimTime << " " << " Icon " << QString::fromStdString(aPlatform.GetIcon());
//
//    QJsonObject platform_obj;
//    platform_obj.insert("name", QString::fromStdString(aPlatform.GetName()));
//    platform_obj.insert("icon", QString::fromStdString(aPlatform.GetIcon()));
//    platform_obj.insert("side", QString::fromStdString(aPlatform.GetSide()));

    double lat;
    double lon;
    double alt;
    aPlatform.GetLocationLLA(lat, lon, alt);

//    platform_obj.insert("lat", lat);
//    platform_obj.insert("lon", lon);
//    platform_obj.insert("alt", alt);


}

void SimEngineDataContainer::PlatformAdded(const std::string sign, double aSimTime, const WsfPlatform &aPlatform)
{
//    LOGGER_DEBUG() << " PlatformAdded => " << aSimTime << " Name " << QString::fromStdString(aPlatform.GetName());
//    LOGGER_DEBUG() << " PlatformAdded => " << aSimTime << " Icon " << QString::fromStdString(aPlatform.GetIcon());

//    QJsonObject platform_obj;
//    platform_obj.insert("name", QString::fromStdString(aPlatform.GetName()));
//    platform_obj.insert("icon", QString::fromStdString(aPlatform.GetIcon()));
//    platform_obj.insert("side", QString::fromStdString(aPlatform.GetSide()));

//    double lat;
//    double lon;
//    double alt;
//    aPlatform.GetLocationLLA(lat, lon, alt);

//    platform_obj.insert("lat", lat);
//    platform_obj.insert("lon", lon);
//    platform_obj.insert("alt", alt);


}

void SimEngineDataContainer::PlatformDeleted(const std::string sign, double aSimTime, const WsfPlatform &aPlatform)
{
//    LOGGER_DEBUG() << " PlatformDeleted =>" << " Name " << QString::fromStdString(aPlatform.GetName());
//    LOGGER_DEBUG() << " PlatformDeleted =>" << " Icon " << QString::fromStdString(aPlatform.GetIcon());
//
//    QJsonObject platform_obj;
//    platform_obj.insert("name", QString::fromStdString(aPlatform.GetName()));
//    platform_obj.insert("icon", QString::fromStdString(aPlatform.GetIcon()));
//    platform_obj.insert("side", QString::fromStdString(aPlatform.GetSide()));

    double lat;
    double lon;
    double alt;
    aPlatform.GetLocationLLA(lat, lon, alt);

//    platform_obj.insert("lat", lat);
//    platform_obj.insert("lon", lon);
//    platform_obj.insert("alt", alt);

}
