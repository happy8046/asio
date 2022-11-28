#ifndef ASIO_CHANNEL_H
#define ASIO_CHANNEL_H

#include <stdint.h>
#include <sys/poll.h>
#include <functional>

namespace asio
{
class EventLoop;

class Channel
{
    enum PollEvent : uint32_t
    {
        kNone,
        kReadEvent = POLLIN | POLLPRI,
        kWriteEvent = POLLOUT,
    };
public:
    Channel(EventLoop* loop);
    ~Channel();

    void set_event(uint32_t event);
    void handle_event();

    int get_index() { return index_; }
    void set_index(int index) { index_ = index; }

    int get_fd() { return fd_; }
    void set_fd(int fd) { fd_ = fd; }

    uint32_t poll_event() { return poll_event_; }

    void set_read_callback(const std::function<void()>& cb);
    void set_write_callback(const std::function<void()>& cb);
    void set_close_callback(const std::function<void()>& cb);

    bool none_event() { return poll_event_ == kNone; }
    void enable_reading() { poll_event_ |= kReadEvent; update(); }
    void disable_all() { poll_event_ = kNone; update(); }

    void remove();
private:
    Channel(const Channel&) = delete;
    Channel operator=(const Channel&) = delete;

    void update();
private:
    uint32_t event_ = 0;
    uint32_t poll_event_ = kNone;
    EventLoop* loop_ = nullptr;
    bool in_loop_ = false;
    int index_ = -1;
    int fd_ = -1;

    std::function<void()> read_callback_;
    std::function<void()> write_callback_;
    std::function<void()> close_callback_;
};

}
#endif