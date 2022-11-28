#ifndef ASIO_EVENT_LOOP_H
#define ASIO_EVENT_LOOP_H

#include <memory>
#include <atomic>
#include <vector>
#include <functional>

namespace asio
{
class Epoll;
class Channel;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void run_in_loop(const std::function<void()>& cb);

    void update_channel(Channel* channel);
    void remove_channel(Channel* channel);
private:
    EventLoop(const EventLoop&) = delete;
    EventLoop operator=(const EventLoop&) = delete;

    void handle_read();
private:
    std::unique_ptr<Epoll> epoll_;
    std::atomic<bool> is_stop_{false};
    std::vector<Channel*> active_channel_;
    std::unique_ptr<Channel> wakeup_channel_;
};
}


#endif