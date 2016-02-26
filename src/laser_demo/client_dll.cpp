#include <Windows.h>
#include <stdio.h>
#include <cstdint>
#include <atomic>

#include <sstream>

#include "intercept.hpp"
#include "logging.hpp"
#include "client\client.hpp"
#include "client\pointers.hpp"
#include "client\sqf\uncategorized.hpp"
#include "glm\gtx\rotate_vector.hpp"
#include "glm\gtx\polar_coordinates.hpp"

#define DIVERGENCE 0.3


#define PI 3.14159265f

#define DEG2RAD(deg) ((deg * PI)/180.0f)

INITIALIZE_EASYLOGGINGPP

using namespace intercept;

struct laser_spot {
    float ratio;
    float power;
    vector3 position_asl;
    vector3 normal;
};

struct laser_return {
    float longest;
    float shortest;
    std::vector<laser_spot> laser_spots;
};



laser_return shoot_cone(const vector3 &pos_, const vector3 &vec_, float divergence_ = DIVERGENCE, uint32_t count_ = 10) {
    float longest_return = -100000.0f;
    float shortest_return = 100000.0f;


    glm::vec3 rotate_axis = glm::vec3(vec_.x, vec_.z, vec_.y);

    auto polar = glm::polar(rotate_axis);
    auto rotate_normal = glm::euclidean(glm::vec2(polar.x + DEG2RAD(90.f), polar.y));


    vector3 rotated_vec;
    //sqf::draw_line_3d(sqf::asl_to_atl(pos_), sqf::asl_to_atl(pos_ + vec_*5000.0f), { 1,0,0,1 });
    object player = sqf::player();

    auto intersections = sqf::line_intersects_surfaces(pos_, pos_ + vec_*5000.0f, player);
    std::list<sqf::intersect_surfaces> points;

    if (intersections.size() > 0) {
        float distance = pos_.distance(intersections[0].intersect_pos_asl);
        longest_return = std::max(longest_return, distance);
        shortest_return = std::min(shortest_return, distance);
        points.push_back(intersections[0]);
        //sqf::draw_line_3d(sqf::asl_to_atl(pos_), sqf::asl_to_atl(intersections[0].intersect_pos_asl), { 0,1,0,1 });
    }
    
    uint32_t inner_count = (uint32_t)(count_ * 0.3f);
    for (uint32_t d = 0; d < inner_count; ++d) {
        auto rotated_vec_glm = glm::rotate(rotate_normal, DEG2RAD((360.0f / (float)inner_count)*(float)d), rotate_axis);

        rotated_vec = vector3(rotated_vec_glm.x, rotated_vec_glm.z, rotated_vec_glm.y);
        vector3 pos2 = pos_ + ((vec_ * 1000.0f) + rotated_vec * divergence_ * 0.5f);
        pos2 = pos_ + (pos2 - pos_).normalize()*5000.0f;
        auto intersections = sqf::line_intersects_surfaces(pos_, pos_ + (pos2 - pos_).normalize()*5000.0f, player);


        if (intersections.size() > 0) {
            float distance = pos_.distance(intersections[0].intersect_pos_asl);
            longest_return = std::max(longest_return, distance);
            shortest_return = std::min(shortest_return, distance);
            points.push_back(intersections[0]);
            //sqf::draw_line_3d(sqf::asl_to_atl(pos_), sqf::asl_to_atl(intersections[0].intersect_pos_asl), { 1,0,0,1 });
        }

    }
    uint32_t outter_count = (uint32_t)(count_ * 0.7f);;
    for (uint32_t d = 0; d < outter_count; ++d) {
        auto rotated_vec_glm = glm::rotate(rotate_normal, DEG2RAD((360.0f / (float)outter_count)*(float)d), rotate_axis);

        rotated_vec = vector3(rotated_vec_glm.x, rotated_vec_glm.z, rotated_vec_glm.y);
        vector3 pos2 = pos_ + ((vec_ * 1000.0f) + rotated_vec * divergence_);
        pos2 = pos_ + (pos2 - pos_).normalize()*5000.0f;
        auto intersections = sqf::line_intersects_surfaces(pos_, pos_ + (pos2 - pos_).normalize()*5000.0f, player);


        if (intersections.size() > 0) {
            float distance = pos_.distance(intersections[0].intersect_pos_asl);
            longest_return = std::max(longest_return, distance);
            shortest_return = std::min(shortest_return, distance);
            points.push_back(intersections[0]);
            //sqf::draw_line_3d(sqf::asl_to_atl(pos_), sqf::asl_to_atl(intersections[0].intersect_pos_asl), { 1,0,0,1 });
        }

    }

    
    std::vector<std::vector<std::tuple<vector3,vector3>>> buckets;

    if (points.size() > 1) {
        do {
            auto test_point_it = points.begin();
            auto test_point = *test_point_it;
            points.erase(test_point_it);
            std::vector<std::tuple<vector3, vector3>> bucket;
            bucket.push_back(std::tuple<vector3, vector3>{ test_point.intersect_pos_asl, test_point.surface_normal });

            auto check_point = points.begin();
            float test_distance = (pos_.distance(test_point.intersect_pos_asl) / 1000.0f) * (divergence_ * 8.0f);
            while(check_point != points.end()) {
                if (test_point.intersect_pos_asl.distance(check_point->intersect_pos_asl) < test_distance) {
                    bucket.push_back(std::tuple<vector3, vector3>{check_point->intersect_pos_asl, check_point->surface_normal});
                    points.erase(check_point++);
                }
                else {
                    check_point++;
                }
            }
            buckets.push_back(bucket);
        } while (points.size() > 0);
    }

    std::vector<laser_spot> spots;

    for (auto spot : buckets) {
        vector3 center;
        vector3 normal;
        for (auto point : spot) {
            center = center + std::get<0>(point);
            normal = normal + std::get<1>(point);
        }
        laser_spot new_spot;
        new_spot.position_asl = center / (float)spot.size();
        new_spot.normal = normal / (float)spot.size();
        new_spot.ratio = (float)spot.size() / (float)(count_ + 1);
        spots.push_back(new_spot);
    }

    return laser_return{ longest_return, shortest_return, spots };
}


int __cdecl intercept::api_version() {
    return 1;
}

void __cdecl intercept::on_frame() {
    vector3 pos = sqf::eye_pos(sqf::player());
    auto res = shoot_cone(pos, sqf::eye_direction(sqf::player()));
    
    std::stringstream ss;
    ss << "Longest: " << res.longest << "m Shortest: " << res.shortest << "m Total: " << res.laser_spots.size();
    for (auto spot : res.laser_spots) {
        sqf::draw_line_3d(sqf::asl_to_atl(pos), sqf::asl_to_atl(spot.position_asl), { 0,1,0,1 });
        sqf::draw_line_3d(sqf::asl_to_atl(spot.position_asl), sqf::asl_to_atl(spot.position_asl + spot.normal), { 1,0,0,1 });
    }
    sqf::side_chat(sqf::player(), ss.str());
    
}


void __cdecl intercept::post_init() {

}
void __cdecl intercept::mission_stopped() {

}



void Init(void) {
    el::Configurations conf;

    conf.setGlobally(el::ConfigurationType::Filename, "logs/intercept_laser_demo.log");
    conf.setGlobally(el::ConfigurationType::MaxLogFileSize, "10240");
#ifdef _DEBUG
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "[%datetime] - %level - {%loc}t:%thread- %msg");
    conf.setGlobally(el::ConfigurationType::PerformanceTracking, "true");
#else
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime-{%level}- %msg");
#endif
    el::Loggers::setDefaultConfigurations(conf, true);

    LOG(INFO) << "Intercept Example DLL Loaded";
}

void Cleanup(void) {

}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Init();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        Cleanup();
        break;
    }
    return TRUE;
}
