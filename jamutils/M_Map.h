#pragma once

#include <sys/mman.h>
#include <unistd.h>
#include <system_error>

namespace jam_utils {
class M_Map {
public:
  M_Map(const size_t length, const int prot, const int flags, const int fd,
        const off_t offset)
    : len_(length) {
    addr_ = mmap(nullptr, length, prot, flags, fd, offset);
    if (addr_ == MAP_FAILED) {
      throw std::system_error(errno, std::system_category(), "");
    }
  }

  M_Map(const M_Map&) = delete;

  M_Map& operator=(const M_Map&) = delete;

  M_Map(M_Map&& other) noexcept : addr_(other.addr_), len_(other.len_) {
    other.addr_ = MAP_FAILED;
    other.len_ = 0;
  }

  M_Map&
  operator=(M_Map&& other) noexcept {
    if (this != &other) {
      if (addr_) {
        munmap(addr_, len_);
      }
      addr_ = other.addr_;
      len_ = other.len_;

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

  [[nodiscard]] std::byte*
  addr() const noexcept {
    return static_cast<std::byte*>(addr_);
  }

  [[nodiscard]] size_t
  len() const noexcept { return len_; }

private:
  void* addr_{nullptr};
  size_t len_{};
};
}