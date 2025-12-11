#ifndef SIMENGINEMANAGER_HPP
#define SIMENGINEMANAGER_HPP

#include "json/json.hpp"

class WsfApplication;
class WsfScenario;

class ScenarioAnalysis;

class SimEngineManager
{

public:
    SimEngineManager(nlohmann::json plugin_paths);
    ~SimEngineManager();

    static SimEngineManager& getInstance(nlohmann::json plugin_paths)
    {
        static SimEngineManager instance(plugin_paths);
        return instance;
    }

    /**
     * @brief PrintTypeInfo
     * @param mScenarioPtr
     */
    void PrintTypeInfo(std::shared_ptr<WsfScenario> scenarioPtr);

    /**
     * 加载脚本模板
     */
    void LoadScriptTemplate(std::string script_path);

    std::shared_ptr<WsfScenario> GetScenario();

public:
    std::string sign() const;

protected:
    WsfApplication* _application;
    std::shared_ptr<WsfScenario> _scenarioPtr;
};

#endif // SIMENGINEINTERFACE_HPP
