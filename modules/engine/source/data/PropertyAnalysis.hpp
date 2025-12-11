//
// Created by dawn on 2025/10/24.
//

#ifndef AGNET_PROPERTYANALYSIS_HPP
#define AGNET_PROPERTYANALYSIS_HPP

#include "WsfSimulation.hpp"

/**
 * 属性解析
 */
class PropertyAnalysis {
public:
    PropertyAnalysis(std::shared_ptr<WsfSimulation> simulationPtr);
    ~PropertyAnalysis();

    double Analysis(std::string sign, std::string key);

protected:
    //! WSF 仿真对象
    std::shared_ptr<WsfSimulation> _simulationPtr;

};


#endif //AGNET_PROPERTYANALYSIS_HPP
