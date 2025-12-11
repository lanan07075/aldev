#ifndef SIMENGINEUPDATECONTROLLER_HPP
#define SIMENGINEUPDATECONTROLLER_HPP

#include <string>

#include "json/json.hpp"

class SimEngineInterface;

/**
 * @brief The SimEngineUpdateController class 更新控制器
 */
class SimEngineUpdateController
{

public:
    SimEngineUpdateController();

    void ControlCommand(std::string platform_sign, nlohmann::json commands);

};

#endif // SIMENGINEUPDATECONTROLLER_HPP
