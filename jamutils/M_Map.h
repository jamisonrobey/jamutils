#pragma once

#include <sys/mman.h>
#include <unistd.h>
#include <system_error>

#include "FD.h"

namespace jam_utils {
class M_Map {
public:
  M_Map(const size_t length,
        const int prot,
        const int flags,
        FD&& fd,
        const off_t offset) :
    fd_{std::move(fd)},
    addr_{mmap(nullptr, length, prot, flags, fd_.fd(), offset)},
    len_{length} {
    if (addr_ == MAP_FAILED)
      throw std::system_error(errno, std::system_category(), "mmap");
  }

  M_Map(const M_Map&) = delete;

  M_Map& operator=(const M_Map&) = delete;

  M_Map(M_Map&& other) noexcept : fd_{std::move(other.fd_)}, addr_{other.addr_},
                                  len_{other.len_} {
    other.addr_ = nullptr;
    other.len_ = 0;
  }

  M_Map& operator=(M_Map&& other) noexcept {
    if (this != &other) {
      if (addr_) {
        munmap(addr_, len_);
      }
      addr_ = other.addr_;
      len_ = other.len_;
      fd_ = std::move(other.fd_);

      other.addr_ = nullptr;
      other.len_ = 0;
    }
    return *this;
  }

  ~M_Map() {
    if (addr_) {
      munmap(addr_, len_);
    }
  }

  [[nodiscard]]
  int fd() const noexcept {
    return fd_.fd();
  }

  [[nodiscard]]
  void* addr() const noexcept {
    return addr_;
  }

  template <typename T>
  [[nodiscard]]
  std::span<T> as_span() const noexcept {
    return {static_cast<T*>(addr_), len_ / sizeof(T)};
  }


  [[nodiscard]]
  size_t len() const noexcept {
    return len_;
  }

private:
  FD fd_;
  void* addr_{nullptr};
  size_t len_{};
};
}