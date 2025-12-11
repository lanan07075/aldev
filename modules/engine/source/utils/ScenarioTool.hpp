#ifndef SCENARIOTOOL_HPP
#define SCENARIOTOOL_HPP

#include <WsfScenario.hpp>



class ScenarioTool
{
public:
    /**
     * @brief PrintTypeInfo
     * @param mScenarioPtr
     */
    static void PrintTypeInfo(std::shared_ptr<WsfScenario> scenarioPtr);
};

#endif // SCENARIOTOOL_HPP
