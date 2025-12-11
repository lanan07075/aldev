#ifndef SCENARIOANALYSIS_HPP
#define SCENARIOANALYSIS_HPP

#include "json/json.hpp"

#include "WsfScenario.hpp"

class SimEngineInterface;

class ScenarioAnalysis {

public:
    explicit ScenarioAnalysis(std::shared_ptr<WsfScenario> scenarioPtr);

    /**
     * @brief AnalysisPlatform 解析平台
     * @param platform_obj
     */
    void AnalysisPlatform(std::list<WsfPlatform*>& platforms, nlohmann::json platform_array);

private:
    std::shared_ptr<WsfScenario> _scenarioPtr;
};

#endif // SCENARIOANALYSIS_HPP
