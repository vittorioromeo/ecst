// Completely decoupled from ECST
namespace component
{
    struct velocity
    {
        int _x, _y;
    };

    struct acceleration
    {
        int _x, _y;
    };
}

// Completely decoupled from ECST
// TDD friendly
namespace process
{
    void accelerate(velocity& v, const acceleration& a)
    {
        v._x += a._x;
        v._y += a._y;
    }
}

namespace system
{
    struct accelerate
    {
        // ECST coupling: component-getting behavior
        template <typename TData>
        void process(TData& data)
        {
            data.for_entities([&](auto eid)
                {
                    auto& c_velocity = // .
                        ecst::get<c::velocity>(data, eid);

                    const auto& c_acceleration = // .
                        ecst::get<c::acceleration>(data, eid);

                    process::accelerate(c_velocity, c_acceleration);
                });
        }
    };
}