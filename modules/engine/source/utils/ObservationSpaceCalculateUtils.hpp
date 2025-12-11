#ifndef OBSERVATIONSPACECALCULATEUTILS_HPP
#define OBSERVATIONSPACECALCULATEUTILS_HPP

#include <WsfP6DOF_Mover.hpp>
#include <WsfPlatform.hpp>

class ObservationSpaceCalculateUtils
{

public:
    /**
     * @brief CalculateCurrentStateInfo 计算当前状态信息
     * @param p6dofMover
     */
    static void CalculateCurrentStateInfo(WsfP6DOF_Mover* p6dofMover, double(&data)[8]);

    /**
     * @brief CalculateRelativeTargetInfo 计算相对目标信息
     * @param p6dofMover
     */
    static int CalculateRelativeTargetInfo(WsfP6DOF_Mover* sourceMover, double current_speed_mach_x, WsfP6DOF_Mover* targetMover, double target_speed_mach_x, double(&data)[5]);

};

#endif // OBSERVATIONSPACECALCULATEUTILS_HPP
