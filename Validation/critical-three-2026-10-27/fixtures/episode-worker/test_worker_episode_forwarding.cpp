#include "test.h"
#include "EpisodeTestServer.h"

#include <carla/client/TimeoutException.h>
#include <carla/trafficmanager/WorkerException.h>

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

using namespace std::chrono_literals;

TEST(traffic_manager_worker_integration, failure_reaches_real_episode_pending_channel) {
  test::ClientEpisode fixture;
  ASSERT_TRUE(fixture.Bootstrap());
  const auto error = carla::traffic_manager::detail::CaptureWorkerException([]() {
    throw carla::client::TimeoutException("127.0.0.1:2000", carla::time_duration::milliseconds(100));
  });
  ASSERT_TRUE(error);
  std::string message;
  try {
    std::rethrow_exception(error);
  } catch (const carla::client::TimeoutException &exception) {
    message = std::string("Traffic Manager worker stopped: ") + exception.what();
  }
  std::atomic_bool run{false};
  std::atomic_bool received{false};
  std::thread waiter([&]() {
    const auto deadline = std::chrono::steady_clock::now() + 5s;
    while (!received.load() && std::chrono::steady_clock::now() < deadline) {
      try {
        fixture.episode->WaitForState(10ms);
      } catch (const carla::detail::SharedException &exception) {
        EXPECT_EQ(std::string(exception.what()), message);
        received = true;
      }
    }
  });
  EXPECT_TRUE(test::WaitForCondition([&]() {
    if (received.load()) {
      return true;
    }
    auto producer = carla::traffic_manager::detail::CreateWorkerThread(run, [&]() {
      const auto reporting_error = carla::traffic_manager::detail::CaptureWorkerException([&]() {
        fixture.episode->AddPendingException(message);
      });
      EXPECT_FALSE(reporting_error);
    });
    producer->join();
    fixture.server.Publish();
    return false;
  }));
  waiter.join();
}
