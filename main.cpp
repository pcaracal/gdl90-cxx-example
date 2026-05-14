#include <fmt/base.h>

#include <gdl90.h>

// clang-format off
static const std::vector<uint8_t> data = {
  // Heartbeat
  0x7E, 0x00, 0x81, 0x41, 0xDB, 0xD0, 0x08, 0x02, 0xB3, 0x8B, 0x7E,

  // Ownship
  0x7E, 0x0A, 0x00, 0xAB, 0x45, 0x49, 0x1F, 0xEF, 0x15, 0xA8, 0x89,
  0x78, 0x0F, 0x09, 0xA9, 0x07, 0xB0, 0x01, 0x20, 0x01, 0x4E, 0x38,
  0x32, 0x35, 0x56, 0x20, 0x20, 0x20, 0x00, 0x85, 0x5B, 0x7E,

  // AHRS
  0x7E, 0x65, 0x01, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x0A, 0x04,
  0x0E, 0x08, 0x99, 0xDD, 0x7E,

  // Some random data
  0x7E, 0xFF, 0xFF, 0x7E,
};
// clang-format on

int main() {
  auto results = gdl90::parse_gdl90_bytes(data);

  for (size_t i = 0; i < results.size(); ++i) {
    fmt::println("Message {}:", i);
    const auto &res = results[i];

    if (res.is_ok()) {
      // unwrap is not noexcept, throws if !res.is_ok()
      const auto msg = res.unwrap();

      if (msg->is_ownship()) {
        const auto &os = msg->get_ownship();
        fmt::println("  Latitude: {:.6f}", os.latitude.degrees());
        fmt::println("  Longitude: {:.6f}", os.longitude.degrees());
        fmt::println("  Altitude: {:.0f} ft", os.altitude.feet());
        fmt::println("  Groundspeed: {} kt", os.horizontal_velocity.knots());

        const auto &lat = os.latitude;
        fmt::println("Some ways to format latitude:"
                     "\n  {} rad"
                     "\n  {} °"
                     "\n  {} ′"
                     "\n  {} ″"
                     "\n  {} r",
                     lat.radians(), lat.degrees(), lat.minutes(), lat.seconds(),
                     lat.revolutions());
      }
    } else {
      fmt::println("  Error: {}", res.err().c_str());
    }
  }

  try {
    const auto &os = results[0].unwrap()->get_ownship();
  } catch (const rust::Error &e) {
    fmt::println("Catching rust error as exception: {}", e.what());
  }
}
