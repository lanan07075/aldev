#ifndef SIMENGINECOMMAND_HPP
#define SIMENGINECOMMAND_HPP

#include <WsfSimulation.hpp>

#include "xio/WsfXIO_PacketRegistry.hpp"

class SimEngineCommand
{
public:
   explicit SimEngineCommand(bool aUseWallClock = false)
      : mUseWallClock(aUseWallClock)
   {
   }

   virtual ~SimEngineCommand()         = default;
   SimEngineCommand(const SimEngineCommand&) = delete;
   SimEngineCommand(SimEngineCommand&&)      = delete;
   SimEngineCommand& operator=(const SimEngineCommand&) = delete;
   SimEngineCommand& operator=(SimEngineCommand&&) = delete;

   virtual void Process(WsfSimulation& aSimulation) = 0;

   bool UseWallClock() const { return mUseWallClock; }

private:
   bool mUseWallClock;

protected:
    static void SendXIO_Command(WsfSimulation& aSimulation, WsfXIO_SimTimeCommandPkt::CommandType aType, double aValue = 0.0);
};

class PauseCommand : public SimEngineCommand
{
public:
    PauseCommand(bool aSendDIS) : SimEngineCommand()
            , mSendDIS(aSendDIS)
    {
    }

    void Process(WsfSimulation& aSimulation) override;

private:
    bool mSendDIS;
};

class ResumeCommand : public SimEngineCommand
{
public:
    ResumeCommand(bool aSendDIS)
            : SimEngineCommand()
            , mSendDIS(aSendDIS)
    {
    }

    void Process(WsfSimulation& aSimulation) override;

private:
    bool mSendDIS;
};

class TerminateCommand : public SimEngineCommand
{
public:
    void Process(WsfSimulation& aSimulation) override;
};

class ResetCommand : public SimEngineCommand
{
public:
    void Process(WsfSimulation& aSimulation) override;
};

class SetClockRateCommand : public SimEngineCommand
{
public:
    SetClockRateCommand(double aClockRate) : SimEngineCommand()
            , mClockRate(aClockRate)
    {
    }

    void Process(WsfSimulation& aSimulation) override;

private:
    double mClockRate;
};

class AdvanceToTimeCommand : public SimEngineCommand
{
public:
    AdvanceToTimeCommand(double aSimTime) : SimEngineCommand()
            , mSimTime(aSimTime)
    {
    }

    void Process(WsfSimulation& aSimulation) override;

private:
    double mSimTime;
};

#endif // SIMENGINECOMMAND_HPP
