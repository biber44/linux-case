#define CATCH_CONFIG_MAIN // This provides a main function

#include <Udp.h>
#include <catch2/catch.hpp>
#include <chrono>
#include <thread>

namespace {
constexpr auto ipAddress = "127.0.0.1";
constexpr uint16_t port = 8000;
} // namespace

TEST_CASE("Create a new UPD instance") {

  SECTION("Use invalid IP address") { REQUIRE_THROWS(Udp{"", port}); }

  SECTION("Use valid IP address") { REQUIRE_NOTHROW(Udp{ipAddress, port}); }
}

TEST_CASE("Send UPD message synchronously") {
  Udp udp{ipAddress, port};

  SECTION("Send empty message") { REQUIRE_THROWS(udp.send("")); }

  SECTION("Send valid message") { REQUIRE_NOTHROW(udp.send("Hello Quantum-Systems")); }
}

TEST_CASE("Send UPD message asynchronously") {
  Udp udp{ipAddress, port};

  SECTION("Send empty message") { REQUIRE_THROWS(udp.sendAsync("", std::chrono::seconds{2})); }

  SECTION("Send with too short delay") { REQUIRE_THROWS(udp.sendAsync("", std::chrono::seconds{0})); }

  SECTION("Send with too long delay") { REQUIRE_THROWS(udp.sendAsync("", std::chrono::seconds{256})); }

  SECTION("Send a valid message") { REQUIRE_NOTHROW(udp.sendAsync("Hello Quantum-Systems", std::chrono::seconds{2})); }

  SECTION("Send multiple valid messages") {
    REQUIRE_NOTHROW(udp.sendAsync("Hello Quantum-Systems ", std::chrono::seconds{2}));
    REQUIRE_NOTHROW(udp.sendAsync("Servus Quantum-Systems", std::chrono::seconds{3}));
    REQUIRE_NOTHROW(udp.sendAsync("Servus Quantum-Systems", std::chrono::seconds{1}));
  }
}

TEST_CASE("Send UPD message periodically") {
  Udp udp{ipAddress, port};

  SECTION("Send empty message") { REQUIRE_THROWS(udp.sendPeriodically("", std::chrono::seconds{2})); }

  SECTION("Send with too short interval") { REQUIRE_THROWS(udp.sendPeriodically("", std::chrono::seconds{0})); }

  SECTION("Send with too long interval") { REQUIRE_THROWS(udp.sendPeriodically("", std::chrono::seconds{256})); }

  SECTION("Send a valid message") {
    REQUIRE_NOTHROW(udp.sendPeriodically("Hello Quantum-Systems", std::chrono::seconds{2}));
    std::this_thread::sleep_for(std::chrono::seconds{6});
    udp.stopSendingPeriodically();
  }

  SECTION("Send multiple times") {
    REQUIRE_NOTHROW(udp.sendPeriodically("Hello Quantum-Systems ", std::chrono::seconds{2}));
    REQUIRE_THROWS(udp.sendPeriodically("Servus Quantum-Systems", std::chrono::seconds{3}));
    udp.stopSendingPeriodically();
  }
}
