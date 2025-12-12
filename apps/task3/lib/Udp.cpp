#include "Udp.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>

Udp::Udp(const std::string &ipAddr, const uint16_t port) {
  // No need to check the port, but we need to know 0–1023 are privileged
  // (require root to bind).
  if (ipAddr.empty()) {
    throw std::invalid_argument("IP address is empty");
  }

  m_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (m_fd == -1) {
    throw std::runtime_error(std::string{"Failed to create the socket: "} +
                             std::strerror(errno));
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ipAddr.c_str(), &m_addr.sin_addr) == -1) {
    ::close(m_fd);
    throw std::runtime_error(
        std::string{"Failed to convert IP address to binary form: "} +
        std::strerror(errno));
  }
}

Udp::~Udp() {
  for (auto &worker : m_asyncWorkers) {
    worker.join();
  }

  stopSendingPeriodically();

  if (m_fd != -1) {
    ::close(m_fd);
  }
}

void Udp::send(const std::string &message) {
  if (message.empty()) {
    throw std::invalid_argument("Message is empty");
  }

  std::scoped_lock lock{m_mutex};
  const auto bytesSent =
      sendto(m_fd, message.c_str(), message.length(), 0,
             reinterpret_cast<const sockaddr *>(&m_addr), sizeof(m_addr));

  if (bytesSent == -1) {
    throw std::runtime_error(std::string{"Failed to send message: "} + message +
                             ". Reason: " + std::strerror(errno));
  }

  if (bytesSent != message.length())
    throw std::runtime_error(
        "Incomplete sent of message: " + std::to_string(bytesSent) + " of " +
        std::to_string(message.length()) + " bytes.");
}

void Udp::sendAsync(const std::string &message,
                    const std::chrono::seconds delay) {
  if (message.empty()) {
    throw std::invalid_argument("Message is empty");
  }

  if (delay < std::chrono::seconds(1) || delay > std::chrono::seconds(255)) {
    throw std::invalid_argument(
        "Invalid delay. Please choose between 1 and 255 seconds.");
  }

  m_asyncWorkers.emplace_back(
      [this, message, delay] { sendDelayed(message, delay); });
}

void Udp::sendPeriodically(const std::string &message,
                           const std::chrono::seconds interval) {
  if (message.empty()) {
    throw std::invalid_argument("Message is empty");
  }

  if (interval < std::chrono::seconds(1) ||
      interval > std::chrono::seconds(255)) {
    throw std::invalid_argument(
        "Invalid interval. Please choose between 1 and 255 seconds.");
  }

  if (m_sending) {
    throw std::runtime_error(std::string{"Already sending periodic messages"});
  }

  m_sending = true;
  m_periodicWorker = std::thread([this, message, delay = interval] {
    while (m_sending) {
      sendDelayed(message, delay);
    }
  });
}

void Udp::stopSendingPeriodically() {
  m_sending = false;
  if (m_periodicWorker.joinable()) {
    m_periodicWorker.join();
  }
}

void Udp::sendDelayed(const std::string &message,
                      const std::chrono::seconds delay) {
  std::this_thread::sleep_for(delay);
  try {
    send(message);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
