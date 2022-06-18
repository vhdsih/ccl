#ifndef ERSPAN_ANGENT_DAO_WT_H
#define ERSPAN_ANGENT_DAO_WT_H

#include <string>
#include <vector>

#include "wiredtiger.h"

namespace db {

class cursor_op;
class business_info_t;
typedef WT_ITEM wt_item;
typedef WT_CURSOR wt_cursor;
typedef WT_SESSION wt_session;
typedef WT_CONNECTION wt_connection;

const std::string __wt_config(
    "create,cache_size=1GB,log=(enabled,recover=on,path=./logs)");

class wt_t {
public:
    wt_t(const std::string &path, const std::string config = __wt_config);
    ~wt_t();

    bool reset_data_op();

    bool read(uint32_t ip, uint16_t port, time_t since, time_t to,
              std::vector<business_info_t> &result);

    bool write(std::vector<business_info_t> &data);
    bool write(business_info_t &);
    bool remove(time_t before);

private:
    bool open();
    bool close();

private:
    std::string path_;
    std::string config_;
    wt_session *session_;
    wt_connection *conn_;
    cursor_op *data_op_;
};

} // namespace db

#endif
