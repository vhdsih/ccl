#include "db_wt.h"

#include <cstdio>

#include "cursor_op.h"
#include "utils/log.h"
#include "wiredtiger.h"

namespace ccl {

db_wt_t::db_wt_t(const std::string &name, const std::string &conf) :
    db_t(name, conf), opened_(false), session_(nullptr), conn_(nullptr) {}

db_wt_t::~db_wt_t() {
    if (opened_) {
        close();
    }
}

bool db_wt_t::check() { return true; }

bool db_wt_t::close() {
    opened_  = false;
    int code = session_->close(session_, NULL);
    output_log(">> wt close session: %s", code);
    code = conn_->close(conn_, NULL);
    output_log(">> wt close connection: %s", code);
    return true;
}

bool db_wt_t::create() {
    logger(">> wt start create/open table %s with conf %s", db_name_.c_str(),
           db_conf_.c_str());
    int code =
        wiredtiger_open(db_name_.c_str(), NULL, db_conf_.c_str(), &conn_);
    output_log(">> wt create/open connection: %s", code);

    code = conn_->open_session(conn_, NULL, NULL, &session_);
    output_log(">> wt create/open session: %s", code);
    opened_ = true;
    return true;
}

bool db_wt_t::remove() { return true; }

void db_wt_t::output_log(const char *format, int code) const {
    if (code != 0) {
        logger(format, wiredtiger_strerror(code));
    } else {
        logger_error(format, wiredtiger_strerror(code));
    }
}

} // namespace ccl
