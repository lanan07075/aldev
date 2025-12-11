#ifndef CALCULATEUTILS_HPP
#define CALCULATEUTILS_HPP

#define M_PI 3.14159265358979323846
// 地球半径，单位：千米
//#define M_PIX = 6371.0

#include <iostream>
//#include <cmath>

class CalculateUtils
{

public:
    // 角度转弧度
    static double deg2rad(double deg) {
        return deg * M_PI / 180.0;
    }

    // 弧度转角度
    static double rad2deg(double rad) {
        return rad * 180.0 / M_PI;
    }

    /**
     * 计算目标相对自身的方位角（单位：度）
     * @param lat1 自身纬度（度）
     * @param lon1 自身经度（度）
     * @param lat2 目标纬度（度）
     * @param lon2 目标经度（度）
     * @return 方位角（正北为0°，顺时针递增，范围0°~360°）
     */
    static double calculateBearing(double lat1, double lon1, double lat2, double lon2) {
        double phi1 = deg2rad(lat1);
        double phi2 = deg2rad(lat2);
        double deltaLambda = deg2rad(lon2 - lon1);

        // 计算方位角（弧度）
        double y = sin(deltaLambda) * cos(phi2);
        double x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(deltaLambda);
        double bearing_rad = atan2(y, x);

        // 转换为角度并调整到0°~360°
        double bearing_deg = rad2deg(bearing_rad);
        return fmod(bearing_deg + 360.0, 360.0);
    }

    /**
     * 判断目标相对自身的方向
     * @param ownLat 自身纬度（度）
     * @param ownLon 自身经度（度）
     * @param ownHeading 自身偏航角（度，正北0°，顺时针递增）
     * @param targetLat 目标纬度（度）
     * @param targetLon 目标经度（度）
     * @return 方向字符串（"正前方"、"右侧"、"正后方"、"左侧"）
     */
    static int getRelativeDirection(double ownLat, double ownLon, double ownHeading,
                               double targetLat, double targetLon) {
        // 计算目标相对自身的方位角
        double targetBearing = calculateBearing(ownLat, ownLon, targetLat, targetLon);

        // 计算相对方位角（目标方位角 - 自身偏航角，范围：-180°~180°）
        double relativeBearing = targetBearing - ownHeading;
        if (relativeBearing > 180.0) {
            relativeBearing -= 360.0;
        } else if (relativeBearing <= -180.0) {
            relativeBearing += 360.0;
        }

        // 判断方向（使用22.5°作为象限划分阈值）
        const double threshold = 22.5;
        if (relativeBearing >= -threshold && relativeBearing <= threshold) {
            return 0;
        } else if (relativeBearing > threshold && relativeBearing <= 180.0 - threshold) {
            return 1;
        } else if ((relativeBearing > 180.0 - threshold && relativeBearing <= 180.0) ||
                   (relativeBearing < - (180.0 - threshold) && relativeBearing >= -180.0)) {
            return 0;
        } else {
            return -1;
        }
    }

};

#endif // CALCULATEUTILS_HPP
