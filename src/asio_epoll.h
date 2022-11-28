#ifndef ASIO_EPOLL_H
#define ASIO_EPOLL_H

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <sys/epoll.h>

namespace asio
{
class Channel;

class Epoll
{
    enum ChannelStatus
    {
        kNew = -1,
        kAdded,
        kDelete,
    };

public:
    Epoll();
    ~Epoll();

    void epoll_wait(int millisecond, std::vector<Channel*>& active_events);
    void update_channel(Channel* channel);
    void remove_channel(Channel* channel);
private:
    Epoll(const Epoll&) = delete;
    Epoll operator=(const Epoll&) = delete;

    void update(int operation, Channel* channel);

private:
    int epoll_fd_ = -1;
    std::vector<epoll_event> event_list_;
    std::unordered_map<uint32_t, Channel*> channel_map_;

private:
    static const int kInitEventListSize = 16;
};

} // asio
#endif // ASIO_EPOLL_H