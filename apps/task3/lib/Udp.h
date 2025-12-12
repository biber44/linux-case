#pragma once

#include <atomic>
#include <chrono>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <thread>
#include <vector>

/**
 * @brief class for sending udp messages to a designated destination.
 */
class Udp {
public:
  /**
   * @brief Create a IPv4 based UDP socket.
   * @param ipAddr The IP address of the receiver.
   * @param port The port number of the receiver.
   * @throw std::invalid_argument in case of an empty IP address.
   * @throw std::runtime_error in case the socket creation failed.
   */
  Udp(const std::string &ipAddr, uint16_t port);

  ~Udp();

  /**
   * @brief Send UDP message synchronously.
   * @param message The message to be sent.
   * @throw std::runtime_error in case the message could not be sent,
   *                           or message was send partially.
   * @throw std::invalid_argument in case the message is empty.
   * @details Since UDP is connectionless (fire & forget), returning void
   *          feels more intentionally than forcing the caller to evaluate
   *          a return value.
   */
  void send(const std::string &message);

  /**
   * @brief Send UDP message asynchronously.
   * @param message The message to be sent.
   * @param delay Message will be sent after delay expires.
   * @throw std::invalid_argument in case of an invalid delay or empty message.
   * @details The function can be called multiple times even if the first delay
   * has not expired.
   */
  void sendAsync(const std::string &message, const std::chrono::seconds delay);

  /**
   * @brief Send periodically UDP message.
   * @param message The message to be sent.
   * @param interval Message will be sent based on the given interval.
   * @throw std::invalid_argument in case of an invalid interval or empty
   * message.
   * @details Only one message can be sent periodically at a time.
   */
  void sendPeriodically(const std::string &message,
                        const std::chrono::seconds interval);

  /**
   * @brief Stop periodically sending.
   * @details Blocks until the final message has been sent.S
   */
  void stopSendingPeriodically();

private:
  /**
   * @brief Send UDP message synchronously with a delay.
   * @param message The message to be sent.
   * @details The function is meant to be called inside a thread.
   */
  void sendDelayed(const std::string &message,
                   const std::chrono::seconds delay);

  int32_t m_fd{-1};
  sockaddr_in m_addr{};
  std::mutex m_mutex{};
  std::atomic<bool> m_sending{false};
  std::thread m_periodicWorker{};
  std::vector<std::thread> m_asyncWorkers{};
};
