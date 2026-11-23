namespace ZeroZoneCI
{
enum class ETheater : unsigned char
{
    AmberCanal,
    ChalkReach
};

struct FZeroZoneGpsSectorMetadata
{
    double LatitudeDeg;
    double LongitudeDeg;
    int DemGridSize;
};

struct FZeroZoneTheaterSector : FZeroZoneGpsSectorMetadata
{
    ETheater Theater;
    bool bGeoreferenced;
};

constexpr FZeroZoneTheaterSector Amber{{0.0, 0.0, 1009}, ETheater::AmberCanal, false};
constexpr FZeroZoneTheaterSector Chalk{{0.0, 0.0, 1009}, ETheater::ChalkReach, false};
static_assert(Amber.DemGridSize == 1009 && Chalk.DemGridSize == 1009);
static_assert(!Amber.bGeoreferenced && !Chalk.bGeoreferenced);
}
