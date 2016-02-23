#include "aps.hpp"

namespace aps_demo {
    aps::aps(types::object vehicle_) : _ammo_count(10), _last_engagement_time(0), _vehicle(vehicle_)
    {

    }

    aps::~aps()
    {
    }

    void aps::on_frame()
    {
        vector3 current_pos = sqf::get_pos_asl(_vehicle);
        //auto projectiles = sqf::nearest_objects(current_pos, { "MissileBase" }, 2000.0f);

    }

    void aps::add_track(types::object & projectile_)
    {
        _tracking.push_back(projectile_);
    }

    void aps::engage(types::object & projectile_)
    {

    }
}