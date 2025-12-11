#include "ObservationSpaceCalculateUtils.hpp"

#include <CalculateUtils.hpp>

void ObservationSpaceCalculateUtils::CalculateCurrentStateInfo(WsfP6DOF_Mover* p6dofMover, double (&data)[8])
{
    double lat;
    double lon;
    double alt;
    p6dofMover->GetPlatform()->GetLocationLLA(lat, lon, alt);

    double roll = p6dofMover->GetRoll();
    //! 横滚轴:旋转的角度的正弦值
    double roll_angle_sin = std::sin(roll * M_PI / 180.0);
    data[0] = roll_angle_sin;
    //! 横滚轴:旋转的角度的余弦值
    double roll_angle_cos = std::cos(roll * M_PI / 180.0);
    data[1] = roll_angle_cos;

    double pitch = p6dofMover->GetPitch();
    //! 俯仰轴:旋转的角度的正弦值
    double pitch_angle_sin = std::sin(pitch * M_PI / 180.0);
    data[2] = pitch_angle_sin;
    //! 俯仰轴:旋转的角度的余弦值
    double pitch_angle_cos = std::cos(pitch * M_PI / 180.0);
    data[3] = pitch_angle_cos;

    //! 计算自身坐标系下沿N轴方向的速度（单位：马赫）
    //! 空速
    double speed_kts = p6dofMover->GetPlatform()->GetSpeed() * UtMath::cNMPH_PER_MPS;
    //! 空速转米/秒
    double speed_ms = speed_kts * 0.514444;
    //! 攻角
    double alpha     = p6dofMover->GetAlpha_deg();
    //! 侧滑角
    double beta      = p6dofMover->GetBeta_deg();
    //! 攻角转弧度
    double alpha_rad = alpha * M_PI / 180.0;
    //! 侧滑角转弧度
    double beta_rad = beta * M_PI / 180.0;
    //! 计算当前高度下的声速
    double speed_of_sound = 340.39 - 0.003939 * alt;
    //! X轴方向的速度
//             double vx_ms = speed_ms * std::cos(beta_rad);
    //! X轴轴速分量
    double vx_ms = speed_ms * std::cos(alpha_rad) * std::cos(beta_rad);
    double speed_mach_x = std::fabs(vx_ms / speed_of_sound);
    data[4] = speed_mach_x;
    //! Y轴方向的速度
//             double vy_ms = speed_ms * std::sin(beta_rad);
    //! Y轴轴速分量
    double vy_ms = speed_ms * std::sin(beta_rad);
    double speed_mach_y = std::fabs(vy_ms / speed_of_sound);
    data[5] = speed_mach_y;
    //! Z轴方向的速度
//             double vz_ms = speed_ms * std::sin(alpha_rad);
    //! Z轴轴速分量
    double vz_ms = speed_ms * std::sin(alpha_rad) * std::cos(beta_rad);
    double speed_mach_z = std::fabs(vz_ms / speed_of_sound);
    data[6] = speed_mach_z;

    //! 自身速度
    double v_total_ms = std::sqrt(vx_ms * vx_ms + vy_ms * vy_ms + vz_ms * vz_ms);
    double speed_mach = std::fabs(v_total_ms / speed_of_sound);
    data[7] = speed_mach;

}

int ObservationSpaceCalculateUtils::CalculateRelativeTargetInfo(WsfP6DOF_Mover* sourceMover, double current_speed_mach_x, WsfP6DOF_Mover *targetMover, double target_speed_mach_x, double (&data)[5])
{
    double source_lat;
    double source_lon;
    double source_alt;
    sourceMover->GetPlatform()->GetLocationLLA(source_lat, source_lon, source_alt);

    double source_heading, source_pitch, source_roll;
    sourceMover->GetPlatform()->GetLocationLLA(source_heading, source_pitch, source_roll);

    double target_lat;
    double target_lon;
    double target_alt;
    targetMover->GetPlatform()->GetLocationLLA(target_lat, target_lon, target_alt);

    double target_heading, target_pitch, target_roll;
    targetMover->GetPlatform()->GetLocationLLA(target_heading, target_pitch, target_roll);

    //! 相对自身坐标系下，X轴方向上与其他平台的速度差
    double velocity_difference = std::fabs(current_speed_mach_x - target_speed_mach_x);
    data[0] = velocity_difference;

    //! 相对于目标平台的高度差，单位为公里
    double altitude_difference = std::fabs((source_alt - target_alt) / 1000.0);
    data[1] = altitude_difference;

    //! 当前纬度转弧度
    double current_lat_rad = CalculateUtils::deg2rad(source_lat);
    //! 当前经度转弧度
    double current_lon_rad = CalculateUtils::deg2rad(source_lon);
    //! 目标纬度转弧度
    double target_lat_rad = CalculateUtils::deg2rad(target_lat);
    //! 目标经度转弧度
    double target_lon_rad = CalculateUtils::deg2rad(target_lon);

    double delta_lon = target_lon_rad - current_lon_rad;

    double x = std::cos(current_lat_rad) * std::sin(target_lat_rad) - std::sin(current_lat_rad) * std::cos(target_lat_rad) * std::cos(delta_lon);
    double y = std::sin(delta_lon) * std::cos(target_lat_rad);

    //! 自身与目标平台之间的方位角
    double bearing_rad = std::atan2(y, x);
    bearing_rad = std::fmod(bearing_rad + 2 * M_PI, 2 * M_PI);
    data[2] = bearing_rad;


    //! 自身与目标平台之间的航迹角
    double heading = sourceMover->GetHeading();
    double heading_rad = heading * M_PI / 180.0;
    double track_angle_rad = bearing_rad - heading_rad;
    track_angle_rad = std::fmod(track_angle_rad + M_PI, 2 * M_PI) - M_PI;
    data[3] = track_angle_rad;

//    LOGGER_INFO() << QString::fromStdString(sourceMover->GetPlatform()->GetName()) << heading;

    //! 自身与目标平台之间的距离（单位：10公里）
    double d_lat = target_lat_rad - current_lat_rad;
    double d_lon = target_lon_rad - current_lon_rad;
    double a = std::sin(d_lat / 2.0) * std::sin(d_lat / 2.0) + std::cos(current_lat_rad) * std::cos(target_lat_rad) * std::sin(d_lon / 2.0) * std::sin(d_lon / 2.0);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    double distance_km = 6371 * c;
    double distance_10km = distance_km / 10.0;
    data[4] = distance_10km;

    return CalculateUtils::getRelativeDirection(source_lat, source_lon, source_heading, target_lat, target_lon);


//    //! 计算目标平台相对于自身的位置信息
//    double angle_diff = bearing_rad - heading_rad;
//    angle_diff = std::fmod(angle_diff + M_PI, 2 * M_PI) - M_PI;

//    double rule_angle = M_PI / 2.0;
//    if(angle_diff > rule_angle){
//        return -1;
//    }else if(angle_diff < -rule_angle){
//        return 1;
//    }else {
//        return 0;
//    }
}
