#ifndef CCL_DB_WIREDTIGER_DB_WT_H
#define CCL_DB_WIREDTIGER_DB_WT_H

#include <string>
#include <vector>

#include "../db.h"

struct __wt_item;
typedef struct __wt_item WT_ITEM;

struct __wt_cursor;
typedef struct __wt_cursor WT_CURSOR;

struct __wt_session;
typedef struct __wt_session WT_SESSION;

struct __wt_connection;
typedef struct __wt_connection WT_CONNECTION;

namespace ccl {

class business_info_t;

using wt_item       = WT_ITEM *;
using wt_cursor     = WT_CURSOR *;
using wt_session    = WT_SESSION *;
using wt_connection = WT_CONNECTION *;

/*
static const char *default_wt_config =
    "create,cache_size=1GB,log=(enabled,recover=on,path=./logs)";
    */

class db_wt_t : public db_t {
public:
    db_wt_t(const std::string &name, const std::string &conf);

    ~db_wt_t() override;

    bool check() override;
    bool close() override;
    bool create() override;
    bool remove() override;

    db_type_t type() const override { return db_type_t::wiredtiger; }

    wt_session get_session() const { return session_; }

private:
    bool opened_;
    wt_session session_;
    wt_connection conn_;
};

} // namespace ccl

#endif
