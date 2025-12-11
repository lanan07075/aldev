#ifndef SIMENGINEINTERFACE_HPP
#define SIMENGINEINTERFACE_HPP



#include "SimEngineKernel.hpp"

class WsfScenario;
class ScenarioAnalysis;
class InstructionAnalysis;

class SimEngineInterface
{

public:
    SimEngineInterface(std::string sign, std::shared_ptr<WsfScenario> scenarioPtr, SimEngineKernel::SimType simulationType);
    ~SimEngineInterface();

    //! 设置年月日
    void SetStartDate(int year, int month, int day);
    //! 设置时分秒
    void SetStartTime(int hour, int minute, double second);
    //! 设置仿真结束时间
    void SetEndTime(double end_time);

    /**
     * 添加输入平台
     * @param platforms
     */
    void AddInputPlatforms(nlohmann::json& platform_array);

    /**
     * 激活控制系统
     * @param platform_sign 平台ID
     * @param pilot_type 控制系统类型
     */
    void ActivatePilotCommand(std::string platform_sign, std::string pilot_type);

    /**
     * 飞行动作控制
     * @param platform_sign
     * @param actions
     */
    void ActionControlCommand(std::string platform_sign, nlohmann::json actions);

    void AddEvent(std::unique_ptr<WsfEvent> aEventPtr);
    void AddWallEvent(std::unique_ptr<WsfEvent> aEventPtr);

    void AddSimCommand(std::unique_ptr<SimEngineCommand> aCommand);

public:
    /**
 * 检查仿真是否为活动状态
 */
    bool IsActive();

    /**
     * 仿真推进
     */
    void Step();
    /**
     * 仿真完成
     */
    void Complete();

    /**
     * 重新开始
     */
    void Reset();

    /**
     * 获取仿真信息
     */
    nlohmann::json GetSimInfo();

    /**
     * 设置属性值
     * @param sign
     * @param key
     * @return
     */
    void SetPropertyValue(std::string sign, std::string key, double value);

    /**
     * 获取属性值
     * @param sign
     * @param key
     * @return
     */
    double GetPropertyValue(std::string sign, std::string key);

//    /**
//     * 获取平台详细数据 sign
//     */
//    nlohmann::json GetPlatformDetailBySign(std::string sign);
//
//    /**
//     * 获取全部平台详细数据
//     */
//    nlohmann::json GetAllPlatformDetail();

//    /**
//     * 获取观测空间
//     * @return
//     */
//    nlohmann::json GetObs();

protected:
    UtCallbackHolder _callbacks { };
    //! 仿真引擎核心
    std::shared_ptr<SimEngineKernel> _simEngineKernel;
    //! 场景解析
    std::shared_ptr<ScenarioAnalysis> _scenarioAnalysis;
    //! 指令解析
    std::shared_ptr<InstructionAnalysis> _instructionAnalysis;

};

#endif // SIMENGINEINTERFACE_HPP
