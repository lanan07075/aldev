//
// Created by dawn on 2025/10/24.
//

#ifndef AGNET_PROPERTYCONFIGURATION_HPP
#define AGNET_PROPERTYCONFIGURATION_HPP

#include "WsfSimulation.hpp"

/**
 * 属性配置
 */
class PropertyConfiguration {

public:
    PropertyConfiguration(std::shared_ptr<WsfSimulation> simulationPtr);
    ~PropertyConfiguration();

    void Configuration(std::string sign, std::string key, double value);

protected:
    //! WSF 仿真对象
    std::shared_ptr<WsfSimulation> _simulationPtr;


};


#endif //AGNET_PROPERTYCONFIGURATION_HPP
