#include <event_loop.h>
#include <logging.h>
#include <channel.h>

namespace asio
{

Channel::Channel(EventLoop* loop)
    : loop_(loop)
{

}

Channel::~Channel()
{

}

void Channel::set_event(uint32_t event)
{
    event_ = event;
}

void Channel::handle_event()
{
    if ((event_ & POLLHUP) && !(event_ & POLLIN))
    {
        if (close_callback_)
        {
            close_callback_();
        }
    }
    if (event_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (read_callback_) 
        {
            read_callback_();
        }
    }
    if (event_ & POLLOUT)
    {
        if (write_callback_) 
        {
            write_callback_();
        }
    }
}

void Channel::set_read_callback(const std::function<void()>& cb)
{
    read_callback_ = cb;
}

void Channel::set_write_callback(const std::function<void()>& cb)
{
    write_callback_ = cb;
}

void Channel::set_close_callback(const std::function<void()>& cb)
{
    close_callback_ = cb;
}

void Channel::remove()
{
    in_loop_ = false;
    loop_->remove_channel(this);
}

void Channel::update()
{
    in_loop_ = true;
    loop_->update_channel(this);
}

}