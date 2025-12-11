//
// Created by dawn on 2025/9/30.
//

#include "SimEngineKernel.hpp"

#include <WsfP6DOF_Mover.hpp>
#include "WsfFrameStepSimulation.hpp"
#include "WsfEventStepSimulation.hpp"

SimEngineKernel::SimEngineKernel(std::string sign, std::shared_ptr<WsfScenario> scenarioPtr, SimType simulationType)
    : _sign(sign)
    , _simulationType(simulationType)
    , _scenarioPtr(scenarioPtr)
    , _regionSimulation("before Starting simulation", profiling::TimedRegion::Mode::SUBREGION)
{
    // 创建仿真对象
    _simulationPtr = CreateSimulation(scenarioPtr, simulationType);
    // 初始化属性解析
    _propertyConfigurationPtr = std::make_shared<PropertyConfiguration>(_simulationPtr);
    // 初始化属性解析
    _propertyAnalysisPtr = std::make_shared<PropertyAnalysis>(_simulationPtr);
    // 初始化仿真
    Initialize(_simulationPtr);
}

//! 设置年月日
void SimEngineKernel::SetStartDate(int year, int month, int day)
{
    WsfDateTime& dateTime = _simulationPtr->GetDateTime();
    //! 设置年月日
    dateTime.SetStartDate(year, month, day);
}
//! 设置时分秒
void SimEngineKernel::SetStartTime(int hour, int minute, double second)
{
    WsfDateTime& dateTime = _simulationPtr->GetDateTime();
    //! 设置时分秒
    dateTime.SetStartTime(hour, minute, second);
}
//! 设置仿真结束时间
void SimEngineKernel::SetEndTime(double end_time)
{
    //! 设置仿真结束时间
    _simulationPtr->SetEndTime(end_time);
}

void SimEngineKernel::AddPlatforms(std::list<WsfPlatform *> platforms)
{
    for(auto platform : platforms)
    {
        _simulationPtr->AddPlatform(platform);
    }
}

std::unique_ptr<WsfSimulation> SimEngineKernel::CreateSimulation(std::shared_ptr<WsfScenario> scenarioPtr, SimEngineKernel::SimType simulationType)
{
    std::unique_ptr<WsfSimulation> simPtr = nullptr;
    try
    {
        if (simulationType == cREAL_TIME)
        {
            simPtr = ut::make_unique<WsfFrameStepSimulation>(*scenarioPtr.get(), RUN_NUMBER);
            simPtr->SetRealtime(0, true);
        }
        else if (simulationType == cFRAME_STEPPED)
        {
            simPtr = ut::make_unique<WsfFrameStepSimulation>(*scenarioPtr.get(), RUN_NUMBER);
            simPtr->SetRealtime(0, false);
        }
        else
        {
            simPtr = ut::make_unique<WsfEventStepSimulation>(*scenarioPtr.get(), RUN_NUMBER);
        }
    }
    catch (WsfException& e)
    {
        { // RAII block
            auto out = ut::log::error() << "Creation of simulation failed.";
            out.AddNote() << e.what();
        }
        simPtr.reset();
    }
    return simPtr;
}

bool SimEngineKernel::Initialize(std::shared_ptr<WsfSimulation> simulationPtr)
{
    // If not pending initialize, skip
    if (simulationPtr->GetState() != WsfSimulation::cPENDING_INITIALIZE)
    {
        return true;
    }

    auto& classificationString = simulationPtr->GetScenario().GetClassificationString();
    if (!classificationString.empty())
    {
        ut::log::info() << "Classification: " << classificationString;
    }

    // Initialize all of the objects in the simulation.
    try
    {
        profiling::TimedRegion regionInitializingSim("Initializing simulation",
                                                     profiling::TimedRegion::Mode::REGION,
                                                     [simulationPtr](ut::log::MessageStream& stream) {
                                                         // Output run numbers only if there are multiple runs
                                                         if (simulationPtr->GetScenario().GetFinalRunNumber() > 1)
                                                         {
                                                             stream.AddNote() << "Run: " << simulationPtr->GetRunNumber();
                                                         }
                                                     });
        simulationPtr->Initialize();
    }
    catch (std::exception& e)
    {
        auto out = ut::log::error() << "Initialization of simulation failed due to unhandled exception.";
        out.AddNote() << "Type: " << ut::TypeNameOf(e);
        out.AddNote() << "What: " << e.what();
        return false;
    }

    if (simulationPtr->GetState() == WsfSimulation::cPENDING_START)
    {
        simulationPtr->Start();
    }

    return true;
}


bool SimEngineKernel::IsActive()
{
    return _simulationPtr->IsActive();
}

void SimEngineKernel::Step()
{
    // Loop until the end of the Monte-Carlo run is complete, or an external
    // control has indicated that we should quit or reset.

    if (_simulationPtr->IsActive())
    {
        _previousMessageInterval = _messageInterval;
        // Determine the messageInterval
        if (_simulationPtr->IsRealTime())
        {
            // If RealTime and not Deferred
            if (!_deferred)
            {
                _messageInterval = REALTIME_MESSAGE_INTERVAL;
            }
                // Check to see if Deferred is complete
            else if (simTime >= DEFERRED_CONNECTION_TIME)
            {
                _messageInterval = REALTIME_MESSAGE_INTERVAL;
                _deferred        = false;
            }
                // use standard interval while in Deferred connection state
            else
            {
                _messageInterval = MESSAGE_INTERVAL;
            }
        }
        else // Use standard interval if not RealTime
        {
            _messageInterval = MESSAGE_INTERVAL;
        }
        // If the interval changed
        if (_messageInterval != _previousMessageInterval)
        {
            // Set the last message time to the current time
            // If we don't update the lastMessageTime, we can get the following to happen:
            //  messageInterval was 1000 and changes to 10, with lastMessageTime = 1000 and simTim = 1500
            //  This will cause a message to appear 50 times (1500-1000) / 10, instead of just once
            _lastMessageTime = simTime;
            // Print a message when the messageInterval changes
            ut::log::info() << "T = " << simTime;
        }

        _simulationPtr->WaitForAdvanceTime();
        simTime = _simulationPtr->AdvanceTime();
        // The following block periodically prints a T=(current sim time) message.
        if ((simTime - _lastMessageTime) >= _messageInterval)
        {
            auto out = ut::log::info();
            out.GetStream().setf(std::ios::fixed | std::ios::showpoint, std::ios::floatfield | std::ios::showpoint);
            out.GetStream().precision(3);

            // For real time simulations, only print the message every 10th pass,
            // unless it is falling behind, in which case we print the message every pass.
            if (_simulationPtr->IsRealTime())
            {
                double clockTime  = _simulationPtr->GetRealTime();
                double timeBehind = clockTime - simTime;

                if (--_messageSkip < 0)
                {
                    _messageSkip = 9;
                }
                if (timeBehind < 0.5 || _simulationPtr->IsFlexibleRealtime())
                {
                    if (_messageSkip == 0)
                    {
                        out << "T = " << simTime;
                    }
                }
                else
                {
                    // These are going to stay on a single line instead of being made notes
                    out << "T = " << simTime << " FALLING BEHIND (" << timeBehind << " sec)";
                }
            }
            else
            {
                out << "T = " << simTime;
            }
            _lastMessageTime += _messageInterval;
        }
    }
}

void SimEngineKernel::Complete()
{
    _simulationPtr->Complete(simTime);

    std::string completionReasonStr = _simulationPtr->GetCompletionReasonString();
    switch (_simulationPtr->GetCompletionReason())
    {
        case WsfSimulation::cEND_TIME_REACHED:
            completionReasonStr = "complete";
            break;
        case WsfSimulation::cRESET:
            bool mResetRequested = true;
            break;
    }

    UtStringUtil::ToLower(completionReasonStr);

    _regionSimulation.StopSubregion("Simulation " + completionReasonStr);

    {
        std::stringstream oss;
        oss.setf(std::ios::fixed | std::ios::showpoint, std::ios::floatfield | std::ios::showpoint);
        oss.precision(3);
        oss << completionReasonStr << ' ' << simTime << ' ' << _regionSimulation.GetWallTime() << ' '
            << _regionSimulation.GetCpuTime();
        ut::log::info() << oss.str();
    }
}

void SimEngineKernel::Reset()
{
    // Run the simulation
    _messageInterval         = MESSAGE_INTERVAL;
    _previousMessageInterval = _messageInterval;
    _messageSkip             = 0;
    _lastMessageTime         = 0.0;
    _deferred                = DEFERRED_CONNECTION_TIME > 0.0;

    // 创建仿真对象
    _simulationPtr = nullptr;
    _simulationPtr = CreateSimulation(_scenarioPtr, _simulationType);
    // 初始化属性解析
    _propertyConfigurationPtr = nullptr;
    _propertyConfigurationPtr = std::make_shared<PropertyConfiguration>(_simulationPtr);
    // 初始化属性解析
    _propertyAnalysisPtr = nullptr;
    _propertyAnalysisPtr = std::make_shared<PropertyAnalysis>(_simulationPtr);

    // 初始化仿真
    Initialize(_simulationPtr);
}

nlohmann::json SimEngineKernel::GetSimInfo()
{
    nlohmann::json sim_data;
    sim_data["sim_time"] = _simulationPtr->GetSimTime();

    nlohmann::json platform_array;
    for (size_t i = 0; i < _simulationPtr->GetPlatformCount(); ++i)
    {
        WsfPlatform* platformPtr = _simulationPtr->GetPlatformEntry(i);
        platformPtr->Update(_simulationPtr->GetSimTime());

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
                }
            }
        }
        platform_array.push_back(platform_obj);
    }
    sim_data["platforms"] = platform_array;

    return sim_data;
}

void SimEngineKernel::SetPropertyValue(std::string sign, std::string key, double value)
{
    _propertyConfigurationPtr->Configuration(sign, key, value);
}

double SimEngineKernel::GetPropertyValue(std::string sign, std::string key)
{
    return _propertyAnalysisPtr->Analysis(sign, key);
}

std::string SimEngineKernel::sign() const
{
    return _sign;
}


std::shared_ptr<WsfSimulation> SimEngineKernel::GetSimulationPtr() {
    return _simulationPtr;
}

SimEngineKernel::~SimEngineKernel()
{

}



