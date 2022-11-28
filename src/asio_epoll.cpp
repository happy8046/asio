#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <logging.h>
#include <channel.h>
#include <asio_epoll.h>

namespace asio
{

Epoll::Epoll() 
  : epoll_fd_(::epoll_create1(EPOLL_CLOEXEC)),
    event_list_(kInitEventListSize)  
{
    LOG_INFO("Epoll create.");
}

Epoll::~Epoll()
{
    LOG_INFO("Epoll delete.");
    ::close(epoll_fd_);
}

void Epoll::epoll_wait(int millisecond, std::vector<Channel*>& active_events)
{
    int io_num = ::epoll_wait(epoll_fd_, &*event_list_.begin(), event_list_.size(), millisecond);
    
    if (io_num > 0)
    {
        for (int i = 0; i < io_num; ++i)
        {
            Channel* channel = static_cast<Channel*>(event_list_[i].data.ptr);

            LOG_INFO("epoll fd:%d", channel->get_fd());
            channel->set_event(event_list_[i].events);
            active_events.emplace_back(channel);
        }
        if (static_cast<size_t>(io_num) == event_list_.size())
        {
            event_list_.resize(event_list_.size() * 2);
        }
    }
    else
    {
        LOG_INFO("no event happen, %s.", strerror(errno));
    }
}

void Epoll::update_channel(Channel* channel)
{
    int index = channel->get_index();
    int fd = channel->get_fd();

    if (index == kNew || index == kDelete)
    {
        if (index = kNew)
        {
            assert(channel_map_.find(fd) == channel_map_.end());
            channel_map_[fd] = channel;
        }
        else
        {
            assert(channel_map_.find(fd) != channel_map_.end());
            assert(channel_map_[fd] == channel);
        }

        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        assert(channel_map_.find(fd) != channel_map_.end());
        assert(channel_map_[fd] == channel);
        assert(index == kAdded);

        if (channel->none_event())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDelete);
        }   
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void Epoll::remove_channel(Channel* channel)
{
    int fd = channel->get_fd();
    int index = channel->get_index();

    assert(channel_map_.find(fd) != channel_map_.end());
    assert(channel_map_[fd] == channel);
    assert(channel->none_event());
    assert(index == kAdded || index == kDelete);
    channel_map_.erase(fd);
    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(kNew);
}

void Epoll::update(int operation, Channel* channel)
{
    epoll_event event;

    memset(&event, 0, sizeof event);
    event.events = channel->poll_event();
    event.data.ptr = channel;

    if (::epoll_ctl(epoll_fd_, operation, channel->get_fd(), &event) < 0)
    {
        LOG_INFO("epoll_ctl fail op=%d, fd=%d, errno=%d %s", operation, channel->get_fd(), errno, strerror(errno));
    }
}

} // asio