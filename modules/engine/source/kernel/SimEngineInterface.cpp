
#include "SimEngineInterface.hpp"

#include "ScenarioAnalysis.hpp"
#include "InstructionAnalysis.hpp"

SimEngineInterface::SimEngineInterface(std::string sign, std::shared_ptr<WsfScenario> scenarioPtr, SimEngineKernel::SimType simulationType)
    : _simEngineKernel(std::make_shared<SimEngineKernel>(sign, scenarioPtr, simulationType))
    , _scenarioAnalysis(std::make_shared<ScenarioAnalysis>(scenarioPtr))
    , _instructionAnalysis(std::make_shared<InstructionAnalysis>())
{

}

//! 设置年月日
void SimEngineInterface::SetStartDate(int year, int month, int day)
{
    _simEngineKernel->SetStartDate(year, month, day);
}
//! 设置时分秒
void SimEngineInterface::SetStartTime(int hour, int minute, double second)
{
    _simEngineKernel->SetStartTime(hour, minute, second);
}
//! 设置仿真结束时间
void SimEngineInterface::SetEndTime(double end_time)
{
    //! 设置仿真结束时间
    _simEngineKernel->SetEndTime(end_time);
}

void SimEngineInterface::AddInputPlatforms(nlohmann::json& platform_array)
{
    std::list<WsfPlatform*> platforms;
    // 解析平台
    _scenarioAnalysis->AnalysisPlatform(platforms, platform_array);
    // 添加平台
    _simEngineKernel->AddPlatforms(platforms);
}

void SimEngineInterface::ActivatePilotCommand(std::string platform_sign, std::string pilot_type)
{
    _instructionAnalysis->ActivatePilotCommand(_simEngineKernel->GetSimulationPtr(), _instructionAnalysis->PilotTypeConvert(pilot_type), platform_sign);
}

void SimEngineInterface::ActionControlCommand(std::string platform_sign, nlohmann::json actions)
{
    _instructionAnalysis->ActionControlCommand(_simEngineKernel->GetSimulationPtr(), actions, platform_sign);
}

bool SimEngineInterface::IsActive()
{
    return _simEngineKernel->IsActive();
}

void SimEngineInterface::Step()
{
    _simEngineKernel->Step();
}

void SimEngineInterface::Complete()
{
    _simEngineKernel->Complete();
}

void SimEngineInterface::Reset()
{
    _simEngineKernel->Reset();
}

nlohmann::json SimEngineInterface::GetSimInfo()
{
    return _simEngineKernel->GetSimInfo();
}

void SimEngineInterface::SetPropertyValue(std::string sign, std::string key, double value)
{
    _simEngineKernel->SetPropertyValue(sign, key, value);
}

double SimEngineInterface::GetPropertyValue(std::string sign, std::string key)
{
    return _simEngineKernel->GetPropertyValue(sign, key);
}

SimEngineInterface::~SimEngineInterface()
{

}






