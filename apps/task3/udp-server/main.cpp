#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// This app is intended for testing purposes, as a small example of how one might extend a unit test to an integration test.
// Therefore, the parameters are hardcoded and there is no error handling.

int main() {
  int32_t fd{-1};
  sockaddr_in addr, cliaddr;
  char buffer[1024];
  fd = ::socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(8000);

  bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));

  socklen_t len;
  len = sizeof(cliaddr);

  while (true) {
    int n = recvfrom(fd, buffer, 1024, MSG_WAITALL, reinterpret_cast<sockaddr *>(&cliaddr), &len);

    buffer[n] = '\0';
    std::cout << buffer << std::endl << std::flush;
  }
  return 0;
}
