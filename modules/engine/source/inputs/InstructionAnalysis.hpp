#ifndef INSTRUCTIONANALYSIS_HPP
#define INSTRUCTIONANALYSIS_HPP

#include "json/json.hpp"

class WsfSimulation;

class InstructionAnalysis
{

public:
    enum PilotType
    {
        MANUAL,
        AUGMENTED,
        HARDWARE,
        SYNTHETIC,
        GUIDANCE
    };


public:
    InstructionAnalysis();
    ~InstructionAnalysis();

    /**
     * 控制系统类型转换 String => PilotType
     * @param pilot_type
     * @return
     */
    InstructionAnalysis::PilotType PilotTypeConvert(std::string pilot_type);

    /**
     * 激活控制系统
     * @param simulationPtr
     * @param pilotType
     * @param platform_sign
     */
    void ActivatePilotCommand(std::shared_ptr<WsfSimulation> simulationPtr, PilotType pilotType, std::string platform_sign);

    /**
     * 动作控制指令
     * @param simulationPtr
     * @param actions
     * @param platform_sign
     */
    void ActionControlCommand(std::shared_ptr<WsfSimulation> simulationPtr, nlohmann::json actions, std::string platform_sign);


private:
    std::map<std::string, double> _lastSimTimeSec;
};

#endif // INSTRUCTIONANALYSIS_HPP
