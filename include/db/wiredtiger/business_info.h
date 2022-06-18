#ifndef ERSPAN_ANGENT_DAO_DATA_H
#define ERSPAN_ANGENT_DAO_DATA_H

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>

namespace db {

class business_info_t {
public:
    business_info_t() {}

    void pack(char buff[], int buff_len) const {
        memset(buff, 0, buff_len);
#ifdef linux
        snprintf(
            buff, buff_len,
            "%u|%u|%ld|%u|%u|%u|%u|%u|%u|%u|%u|%lu|%lu|%lu|%lu|%lu|%lu|%lu|%lu",
            service, port, timestamp, session_created, session_connected,
            session_closing, session_closed, session_timeout, session_no_data,
            session_valid_recv, session_valid_send, Bytes_recv, Bytes_send,
            Bytes_recv_max, Bytes_send_max, Bytes_recv_ave, Bytes_send_ave,
            Bytes_rate_recv, Bytes_rate_send);
#else
        snprintf(
            buff, buff_len,
            "%u|%u|%ld|%u|%u|%u|%u|%u|%u|%u|%u|%llu|%llu|%llu|%llu|%llu|%llu"
            "|%llu|%llu",
            service, port, timestamp, session_created, session_connected,
            session_closing, session_closed, session_timeout, session_no_data,
            session_valid_recv, session_valid_send, Bytes_recv, Bytes_send,
            Bytes_recv_max, Bytes_send_max, Bytes_recv_ave, Bytes_send_ave,
            Bytes_rate_recv, Bytes_rate_send);
#endif
    }

    uint32_t service;
    uint16_t port;
    time_t timestamp;
    uint32_t session_created;
    uint32_t session_connected;
    uint32_t session_closing;
    uint32_t session_closed;
    uint32_t session_timeout;

    /* for success sessions */
    uint32_t session_no_data;
    uint32_t session_valid_recv;
    uint32_t session_valid_send;

    uint64_t Bytes_recv;
    uint64_t Bytes_send;
    uint64_t Bytes_recv_max;
    uint64_t Bytes_send_max;

    uint64_t Bytes_recv_ave;
    uint64_t Bytes_send_ave;
    uint64_t Bytes_rate_recv;
    uint64_t Bytes_rate_send;

private:
};

} // namespace db

#endif
