#include "InstructionAnalysis.hpp"

#include <WsfSimulation.hpp>
#include <WsfP6DOF_Mover.hpp>
#include <WsfSixDOF_Mover.hpp>
#include <WsfSixDOF_PilotManager.hpp>

InstructionAnalysis::InstructionAnalysis()
{

}
//MANUAL,
//AUGMENTED,
//HARDWARE,
//SYNTHETIC,
//GUIDANCE
InstructionAnalysis::PilotType InstructionAnalysis::PilotTypeConvert(std::string pilot_type) {
    if(pilot_type == "MANUAL"){
        return InstructionAnalysis::MANUAL;
    }else if(pilot_type == "AUGMENTED"){
        return InstructionAnalysis::AUGMENTED;
    }else if(pilot_type == "HARDWARE"){
        return InstructionAnalysis::HARDWARE;
    }else if(pilot_type == "SYNTHETIC"){
        return InstructionAnalysis::SYNTHETIC;
    }else if(pilot_type == "GUIDANCE"){
        return InstructionAnalysis::GUIDANCE;
    }
    return InstructionAnalysis::MANUAL;
}

void InstructionAnalysis::ActivatePilotCommand(std::shared_ptr<WsfSimulation> simulationPtr, PilotType pilotType, std::string platform_sign)
{
    _lastSimTimeSec[platform_sign] = 0.00;

    WsfPlatform* plat = simulationPtr->GetPlatformBySign(platform_sign);
    if (nullptr != plat)
    {
        WsfMover* mover = plat->GetMover();
        if (nullptr != mover)
        {
            if (mover->IsA_TypeOf("WSF_P6DOF_MOVER"))
            {
                WsfP6DOF_Mover* p6dofMover = dynamic_cast<WsfP6DOF_Mover*>(mover);

                P6DofVehicle* vehicle = p6dofMover->GetP6DofVehicle();
                if (vehicle != nullptr)
                {
                    P6DofPilotManager* pilotMgr = vehicle->GetPilotManager();
                    if (pilotMgr != nullptr)
                    {
                        if (pilotType == SYNTHETIC)
                        {
                            if (pilotMgr->MakeSyntheticPilotActive())
                            {
                                ut::log::info() << "Changed Active P6DOF Pilot Type to Synthetic";
                            }
                            else
                            {
                                ut::log::warning() << "Unable to MakeSyntheticPilotActive() for " << vehicle->GetName();
                            }
                        }
                        else if (pilotType == HARDWARE)
                        {
                            if (pilotMgr->MakeHardwareAutopilotActive())
                            {
                                ut::log::info() << "Changed Active P6DOF Pilot Type to Hardware Autopilot";
                            }
                            else
                            {
                                ut::log::warning() << "Unable to MakeHardwareAutopilotActive() for " << vehicle->GetName();
                            }
                        }
                        else if (pilotType == GUIDANCE)
                        {
                            if (pilotMgr->MakeGuidanceAutopilotActive())
                            {
                                ut::log::info() << "Changed Active P6DOF Pilot Type to Guidance Autopilot";
                            }
                            else
                            {
                                ut::log::warning() << "Unable to MakeGuidanceAutopilotActive() for " << vehicle->GetName();
                            }
                        }
                        else if (pilotType == MANUAL)
                        {
                            if (pilotMgr->MakeManualPilotActive())
                            {
                                ut::log::info() << "Changed Active P6DOF Pilot Type to Manual";
                            }
                            else
                            {
                                ut::log::warning() << "Unable to MakeManualPilotActive() for " << vehicle->GetName();
                            }
                        }
                    }
                }
            }
            else if (mover->IsA_TypeOf("WSF_SIX_DOF_MOVER"))
            {
                auto                        sixDofMover = dynamic_cast<wsf::six_dof::Mover*>(mover);
                wsf::six_dof::PilotManager* pilotMgr    = sixDofMover->GetPilotManager();
                if (pilotMgr != nullptr)
                {
                    switch (pilotType)
                    {
                        case InstructionAnalysis::MANUAL:
                        {
                            if (pilotMgr->MakeSimpleManualPilotActive())
                            {
                                ut::log::info() << "Changed Active Pilot Type to Manual";
                            }
                            else if (!pilotMgr->MakeAugmentedManualPilotActive())
                            {
                                auto out = ut::log::error() << "Unable to MakeManualPilotActive().";
                                out.AddNote() << "T = " << simulationPtr->GetSimTime();
                                out.AddNote() << "Mover: " << sixDofMover->GetName();
                            }

                            break;
                        }
                        case InstructionAnalysis::AUGMENTED:
                        {
                            if (pilotMgr->MakeAugmentedManualPilotActive())
                            {
                                ut::log::info() << "Changed Active Pilot Type to Manual";
                            }
                            else if (!pilotMgr->MakeSimpleManualPilotActive())
                            {
                                auto out = ut::log::error() << "Unable to MakeManualPilotActive().";
                                out.AddNote() << "T = " << simulationPtr->GetSimTime();
                                out.AddNote() << "Mover: " << sixDofMover->GetName();
                            }

                            break;
                        }
                        case InstructionAnalysis::GUIDANCE:
                            // Fall-through to synthetic autopilot
                        case InstructionAnalysis::HARDWARE:
                            // Fall-through to synthetic autopilot
                        case InstructionAnalysis::SYNTHETIC:
                        {
                            if (pilotMgr->MakeSyntheticPilotActive())
                            {
                                ut::log::info() << "Changed Active Pilot Type to Synthetic";
                            }
                            else
                            {
                                auto out = ut::log::error() << "Unable to MakeSyntheticPilotActive().";
                                out.AddNote() << "T = " << simulationPtr->GetSimTime();
                                out.AddNote() << "Mover: " << sixDofMover->GetName();
                            }

                            break;
                        }
                    }
                }
            }
        }
    }
}

void InstructionAnalysis::ActionControlCommand(std::shared_ptr<WsfSimulation> simulationPtr, nlohmann::json actions, std::string platform_sign)
{
//    ut::log::info() << actions.dump(4);

    if(!actions.contains("roll") &&
       !actions.contains("pitch") &&
       !actions.contains("rudder") &&
       !actions.contains("throttle"))
        return;

    double currentTime_sec = simulationPtr->GetSimTime();
    double deltaT_sec      = -1.0;
    if (_lastSimTimeSec[platform_sign] >= 0.0)
    {
        deltaT_sec = currentTime_sec - _lastSimTimeSec[platform_sign];
    }
    _lastSimTimeSec[platform_sign] = currentTime_sec;

    WsfPlatform* plat = simulationPtr->GetPlatformBySign(platform_sign);
    if (nullptr != plat)
    {
        WsfMover* mover = plat->GetMover();
        if (nullptr != mover)
        {
            if (mover->IsA_TypeOf("WSF_P6DOF_MOVER"))
            {
                WsfP6DOF_Mover* p6dofMover = dynamic_cast<WsfP6DOF_Mover*>(mover);

                P6DofVehicle* vehicle = p6dofMover->GetP6DofVehicle();
                if (vehicle != nullptr)
                {
                    P6DofPilotManager* pilotMgr = vehicle->GetPilotManager();
                    if (pilotMgr != nullptr)
                    {
                        // We need an active manual pilot to proceed
                        P6DofPilotObject* pilot = pilotMgr->GetActiveManualPilot();
                        if (pilot != nullptr)
                        {
                            p6dofMover->EnableControls(true);
                            p6dofMover->EnableAutopilot(false);

                            if (pilot->ControlAugmentationModeIsActive())
                            {
                                p6dofMover->SetRollRateMax(180.0);
                                p6dofMover->SetPitchGLoadMax(8.0);
                            }

                            pilot->TakeManualControl();

                            //! 驾驶杆左右位置
                            double stickRightPosition       = actions["roll"];
                            //! 驾驶杆前后位置
                            double stickAftPosition         = actions["pitch"];
                            //! 方向舵
                            double rudderRightPosition      = actions["rudder"];
                            //! 油门杆
                            double throttleLeverPosition    = actions["throttle"];
                            //! 减速板
                            double spdBrakeLeverPosition = pilot->GetSpeedBrakesControllerPosition();
                            //! 扰流板
                            double spoilerLeverPosition     = 0.0;
                            //! 襟翼
                            double flapsLeverPosition       = 1.0;
                            //! 起落架
                            double landingGearLeverPosition = 0.0;
                            double noseUpTrimValue          = 0.0;
                            double noseUpTrimDeltaT_sec     = 0.0;

//                            ut::log::info() << "control - roll " << stickRightPosition;
//                            ut::log::info() << "control - pitch " << stickAftPosition;
//                            ut::log::info() << "control - rudder " << rudderRightPosition;
//                            ut::log::info() << "control - throttle " << throttleLeverPosition;

                            if (deltaT_sec > 0.0)
                            {
                                if (noseUpTrimValue > 0.001)
                                {
                                    noseUpTrimDeltaT_sec = deltaT_sec;
                                }
                                if (noseUpTrimValue < -0.001)
                                {
                                    noseUpTrimDeltaT_sec = -deltaT_sec;
                                }
                            }

                            double rollRightTrimValue      = 0.0f;
                            double rollRightTrimDeltaT_sec = 0.0;

                            if (deltaT_sec > 0.0)
                            {
                                if (rollRightTrimValue > 0.001)
                                {
                                    rollRightTrimDeltaT_sec = deltaT_sec;
                                }
                                if (rollRightTrimValue < -0.001)
                                {
                                    rollRightTrimDeltaT_sec = -deltaT_sec;
                                }
                            }

                            double yawRightTrimValue      = 0.0f;
                            double yawRightTrimDeltaT_sec = 0.0;

                            if (deltaT_sec > 0.0)
                            {
                                if (yawRightTrimValue > 0.001)
                                {
                                    yawRightTrimDeltaT_sec = deltaT_sec;
                                }
                                if (yawRightTrimValue < -0.001)
                                {
                                    yawRightTrimDeltaT_sec = -deltaT_sec;
                                }
                            }

                            double currentSpdBrkInputPosition = pilot->GetSpeedBrakesControllerPosition();

                            double speedBrakeValue      = 0.0f;
                            double speedBrakeDeltaT_sec = 0.0;

                            if (deltaT_sec > 0.0)
                            {
                                if (speedBrakeValue > 0.001)
                                {
                                    speedBrakeDeltaT_sec = deltaT_sec;
                                }
                                if (speedBrakeValue < -0.001)
                                {
                                    speedBrakeDeltaT_sec = -deltaT_sec;
                                }
                            }

                            currentSpdBrkInputPosition += speedBrakeDeltaT_sec;

                            if (currentSpdBrkInputPosition < 0.0)
                            {
                                currentSpdBrkInputPosition = 0.0;
                            }
                            else if (currentSpdBrkInputPosition > 1.0)
                            {
                                currentSpdBrkInputPosition = 1.0;
                            }

                            spdBrakeLeverPosition = currentSpdBrkInputPosition;


                            pilot->SetTrimManualControlData(noseUpTrimDeltaT_sec, rollRightTrimDeltaT_sec, yawRightTrimDeltaT_sec);

                            pilot->SetManualControlData(stickRightPosition,
                                                        stickAftPosition,
                                                        rudderRightPosition,
                                                        throttleLeverPosition,
                                                        spdBrakeLeverPosition,
                                                        spoilerLeverPosition,
                                                        flapsLeverPosition,
                                                        landingGearLeverPosition);

                        }
                    }
                }
            }
        }
    }
}

InstructionAnalysis::~InstructionAnalysis()
{

}





