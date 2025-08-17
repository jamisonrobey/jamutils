#ifndef M_MAP_H
#define M_MAP_H

#include "FD.h"

#include <span>
#include <sys/mman.h>
#include <system_error>
#include <unistd.h>
#include <utility>

namespace jam_utils
{
class M_Map
{
  public:
    M_Map(std::filesystem::path file, int prot, int flags, off_t offset)
        : fd_{FD{file.c_str()}},
          len_{std::filesystem::file_size(file)},
          addr_{mmap(nullptr, len_, prot, flags, fd_.fd(), offset)}
    {
        if (addr_ == MAP_FAILED)
            throw std::system_error(errno, std::generic_category(), "mmap");
    }

    M_Map& operator=(M_Map&& other) noexcept
    {
        if (this != &other)
        {
            if (addr_)
            {
                munmap(addr_, len_);
            }

            fd_ = std::move(other.fd_);
            addr_ = std::exchange(other.addr_, nullptr);
            len_ = other.len_;
        }
        return *this;
    }

    ~M_Map()
    {
        if (addr_)
        {
            munmap(addr_, len_);
        }
    }

    [[nodiscard]]
    int fd() const noexcept
    {
        return fd_.fd();
    }

    template <typename T = std::byte>
    [[nodiscard]]
    T* at(size_t offset) const noexcept
    {
        return static_cast<T*>(static_cast<std::byte*>(addr_) + offset);
    }

    template <typename T = void>
    [[nodiscard]]
    T* addr() const noexcept
    {
        return static_cast<T*>(addr_);
    }

    template <typename T>
    [[nodiscard]]
    std::span<T> as_span() const noexcept
    {
        return {static_cast<T*>(addr_), len_ / sizeof(T)};
    }

    [[nodiscard]]
    size_t len() const noexcept
    {
        return len_;
    }

  private:
    FD fd_;
    size_t len_;
    void* addr_;
};
} // namespace jam_utils

#endif
