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
    M_Map(size_t length, int prot, int flags, FD&& fd, off_t offset)
        : fd_{std::move(fd)}, addr_{mmap(nullptr, length, prot, flags, fd_.fd(), offset)}, len_{length}
    {
        if (addr_ == MAP_FAILED)
        {
            throw std::system_error(errno, std::system_category());
        }
    }

    M_Map(const M_Map&) = delete;
    M_Map& operator=(const M_Map&) = delete;

    M_Map(M_Map&& other) noexcept
        : fd_{std::move(other.fd_)}, addr_{std::exchange(other.addr_, nullptr)}, len_{other.len_}
    {
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
    void* addr_;
    size_t len_;
};
}

#endif