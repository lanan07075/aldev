//
// Created by dawn on 2025/9/30.
//

#ifndef AGNET_SIMENGINEKERNEL_HPP
#define AGNET_SIMENGINEKERNEL_HPP

#include "json/json.hpp"

#include "TimedRegion.hpp"
#include <WsfSimulation.hpp>
#include <WsfScenario.hpp>
#include <UtConcurrentQueue.hpp>
#include <WsfStandardApplication.hpp>

#include "SimEngineCommand.hpp"
#include "PropertyConfiguration.hpp"
#include "PropertyAnalysis.hpp"

#define M_PI 3.14159265358979323846

class SimEngineKernel {

    using SimCommandQueue = UtConcurrentQueue<std::unique_ptr<SimEngineCommand>>;

public:
    //! The simulation type requested from the command line.
    enum SimType
    {
        cEVENT_STEPPED, //!< '-es' was specified
        cFRAME_STEPPED, //!< '-fs' was specified
        cREAL_TIME      //!< '-rt' was specified
    };

    //! The type of run from the command line.
    enum RunMode
    {
        cRUN_SCENARIO,   //!< Run the scenario (implied if no other command specified)
        cLIST_VARIABLES, //!< List the preprocessor variables.
        cSHOW_USAGE,     //!< Show command line options and exit
        cWRITE_GRAMMAR   //!< Write the grammar file out, used by the IDE
    };

public:
    SimEngineKernel(std::string sign, std::shared_ptr<WsfScenario> scenarioPtr, SimType simulationType);
    ~SimEngineKernel();

    //! 设置年月日
    void SetStartDate(int year, int month, int day);
    //! 设置时分秒
    void SetStartTime(int hour, int minute, double second);
    //! 设置仿真结束时间
    void SetEndTime(double end_time);

    /**
     * 添加平台
     * @param platforms
     */
    void AddPlatforms(std::list<WsfPlatform*> platforms);

    /**
     * 创建仿真对象
     */
    std::unique_ptr<WsfSimulation> CreateSimulation(std::shared_ptr<WsfScenario> scenarioPtr, SimEngineKernel::SimType aSimulationType);
    /**
     * 初始化仿真
     */
    bool Initialize(std::shared_ptr<WsfSimulation> simulationPtr);

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

public:
    /**
     * ID
     * @return
     */
    std::string sign() const;

    /**
     *
     * @return
     */
    std::shared_ptr<WsfSimulation> GetSimulationPtr();

protected:
    //! 仿真场景ID
    std::string _sign;
    //! 仿真类型
    SimType _simulationType;
    //! WSF 场景对象
    std::shared_ptr<WsfScenario> _scenarioPtr;
    //! WSF 仿真对象
    std::shared_ptr<WsfSimulation> _simulationPtr;

private:
    //! 属性配置
    std::shared_ptr<PropertyConfiguration> _propertyConfigurationPtr;
    //! 属性解析
    std::shared_ptr<PropertyAnalysis> _propertyAnalysisPtr;

protected:
    SimCommandQueue _simCommandsWallClock;
    SimCommandQueue _simCommandsSimClock;

protected:
    double DEFERRED_CONNECTION_TIME  = 0.0;
    //! 消息间隔
    double MESSAGE_INTERVAL          = 1000.0;
    double REALTIME_MESSAGE_INTERVAL = 1.0;

    unsigned int RUN_NUMBER          = 1;

protected:
    //! 仿真时区
    profiling::TimedRegion _regionSimulation;
    // Run the simulation
    double _messageInterval         = MESSAGE_INTERVAL;
    double _previousMessageInterval = _messageInterval;
    int    _messageSkip             = 0;
    double _lastMessageTime         = 0.0;
    bool   _deferred                = DEFERRED_CONNECTION_TIME > 0.0;

    double simTime = 0.0;
};


#endif //AGNET_SIMENGINEKERNEL_HPP
