#include <sys/eventfd.h>
#include <unistd.h>
#include <asio_epoll.h>
#include <channel.h>
#include <event_loop.h>
#include <logging.h>

namespace asio
{   
static const int kMaxEpollWaitTime = 5000;

EventLoop::EventLoop()
    : epoll_(new Epoll()),
    wakeup_channel_(new Channel(this))
{
    int wakeup_fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

    LOG_INFO("wakeup fd:%d", wakeup_fd);
    wakeup_channel_->set_fd(wakeup_fd);
    wakeup_channel_->set_read_callback(std::bind(&EventLoop::handle_read, this));
    wakeup_channel_->enable_reading();
}

EventLoop::~EventLoop()
{
    wakeup_channel_->disable_all();
    wakeup_channel_->remove();
    ::close(wakeup_channel_->get_fd());
}

void EventLoop::loop()
{
    is_stop_ = false;

    while(!is_stop_)
    {
        active_channel_.clear();
        epoll_->epoll_wait(kMaxEpollWaitTime, active_channel_);

        for (Channel* channel : active_channel_)
        {
            channel->handle_event();
        }
    }
}

void EventLoop::run_in_loop(const std::function<void()>& cb)
{
    if (cb)
    {
        cb();
    }
}

void EventLoop::update_channel(Channel* channel)
{
    epoll_->update_channel(channel);
}

void EventLoop::remove_channel(Channel* channel)
{
    epoll_->remove_channel(channel);
}

void EventLoop::handle_read()
{
    char buff[64] = {0};
    auto recv_len = ::read(wakeup_channel_->get_fd(), buff, 64);
    LOG_INFO("recv len:%ld data:%s", recv_len, buff);
}

}