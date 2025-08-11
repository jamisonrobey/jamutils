#ifndef FD_H
#define FD_H

#include <fcntl.h>
#include <filesystem>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <utility>

namespace jam_utils
{
class FD
{
  public:
    explicit FD(int fd) : fd_{fd}
    {
        if (fd_ < 0)
        {
            throw std::invalid_argument("FD given negative descriptor");
        }
    }

    explicit FD(const std::filesystem::path& file_path, int flags = O_RDONLY) : fd_{open(file_path.c_str(), flags)}
    {
        if (fd_ < 0)
        {
            throw std::system_error(errno, std::system_category());
        }
    }

    FD(const FD&) = delete;
    FD& operator=(const FD&) = delete;

    FD(FD&& other) noexcept : fd_{std::exchange(other.fd_, -1)} {}
    FD& operator=(FD&& other) noexcept
    {
        if (this != &other)
        {
            if (fd_ >= 0)
            {
                close(fd_);
            }
            fd_ = std::exchange(other.fd_, -1);
        }
        return *this;
    }

    ~FD()
    {
        if (fd_ >= 0)
        {
            close(fd_);
        }
    }

    [[nodiscard]]
    int fd() const noexcept
    {
        return fd_;
    }

  private:
    int fd_{-1};
};
}

#endif