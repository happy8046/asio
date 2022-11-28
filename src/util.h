#ifndef ASIO_UTIL_H
#define ASIO_UTIL_H

template<typename To, typename From>
To implicit_cast(From const& f)
{
    return f;
}

#endif