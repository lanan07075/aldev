#ifndef ACTIVATEPILOTCOMMAND_HPP
#define ACTIVATEPILOTCOMMAND_HPP

#include "SimEngineCommand.hpp"

class ActivatePilotCommand : public SimEngineCommand
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

    ActivatePilotCommand(const std::string& platform_sign, PilotType aPilotType)
       : _platform_sign(platform_sign)
       , mPilotType(aPilotType)
    {
    }

    void Process(WsfSimulation& aSimulation) override;

 private:
    std::string _platform_sign;
    PilotType   mPilotType;
};

#endif // ACTIVATEPILOTCOMMAND_HPP
