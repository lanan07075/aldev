#include "ActivatePilotCommand.hpp"

#include <WsfP6DOF_Mover.hpp>

void ActivatePilotCommand::Process(WsfSimulation& aSimulation)
{
   WsfPlatform* plat = aSimulation.GetPlatformBySign(_platform_sign);
   if (nullptr != plat)
   {
      WsfMover* mover = plat->GetMover();
      if (nullptr != mover)
      {
          ut::log::info() << " -------------------- " << mover->GetType();

         if (mover->IsA_TypeOf("WSF_P6DOF_MOVER"))
         {
            WsfP6DOF_Mover* p6dofMover = dynamic_cast<WsfP6DOF_Mover*>(mover);

            P6DofVehicle* vehicle = p6dofMover->GetP6DofVehicle();
            if (vehicle != nullptr)
            {
               P6DofPilotManager* pilotMgr = vehicle->GetPilotManager();
               if (pilotMgr != nullptr)
               {
                  if (mPilotType == SYNTHETIC)
                  {
                     if (pilotMgr->MakeSyntheticPilotActive())
                     {
                         ut::log::info() << "Changed Active P6DOF Pilot Type to Synthetic";
                     }
                     else
                     {
                         ut::log::warning() << "Unable to MakeSyntheticPilotActive() for " << vehicle->GetName();
                     }
                  }
                  else if (mPilotType == HARDWARE)
                  {
                     if (pilotMgr->MakeHardwareAutopilotActive())
                     {
                         ut::log::info() << "Changed Active P6DOF Pilot Type to Hardware Autopilot";
                     }
                     else
                     {
                         ut::log::warning() << "Unable to MakeHardwareAutopilotActive() for " << vehicle->GetName();
                     }
                  }
                  else if (mPilotType == GUIDANCE)
                  {
                     if (pilotMgr->MakeGuidanceAutopilotActive())
                     {
                         ut::log::info() << "Changed Active P6DOF Pilot Type to Guidance Autopilot";
                     }
                     else
                     {
                         ut::log::warning() << "Unable to MakeGuidanceAutopilotActive() for " << vehicle->GetName();
                     }
                  }
                  else if (mPilotType == MANUAL)
                  {
                     if (pilotMgr->MakeManualPilotActive())
                     {
                         ut::log::info() << "Changed Active P6DOF Pilot Type to Manual";
                     }
                     else
                     {
                         ut::log::warning() << "Unable to MakeManualPilotActive() for " << vehicle->GetName();
                     }
                  }
               }
            }
         }
      }
   }
}
