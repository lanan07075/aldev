//
// Created by dawn on 2025/9/18.
//

#ifndef AGNET_TESTCOMMAND_HPP
#define AGNET_TESTCOMMAND_HPP

#include "SimEngineCommand.hpp"

class TestCommand : public SimEngineCommand
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

    TestCommand(const std::string& platform_sign, PilotType aPilotType)
            : _platform_sign(platform_sign)
            , mPilotType(aPilotType)
    {
    }

    void Process(WsfSimulation& aSimulation) override;

private:
    std::string _platform_sign;
    PilotType   mPilotType;
};


#endif //AGNET_TESTCOMMAND_HPP
