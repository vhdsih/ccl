#include "wt.h"

#include <cstdio>

#include "business_info.h"
#include "cursor_op.h"

namespace db {

const std::string business_table("table:business_info");
const std::string business_index("index:business_info:business_time_index");

wt_t::wt_t(const std::string &path, const std::string config) :
    path_(path),
    config_(config),
    session_(nullptr),
    conn_(nullptr),
    data_op_(nullptr) {
    open();
}

wt_t::~wt_t() {
    delete data_op_;
    close();
}

bool wt_t::open() {
    printf(">>> wt start create/open table %s with conf %s",
           business_table.c_str(), config_.c_str());

    int code = 0;
    code     = wiredtiger_open(path_.c_str(), NULL, config_.c_str(), &conn_);
    printf(">>> wt create/open db: %s", wiredtiger_strerror(code));

    code = conn_->open_session(conn_, NULL, NULL, &session_);
    printf(">>> wt create/open session: %s", wiredtiger_strerror(code));

    code = session_->create(session_, business_table.c_str(),
                            "key_format=IHQ,value_format=IIIIIIIIQQQQQQQQ,"
                            "columns=(ip,port,timestamp,"
                            "create,connected,closing,closed,timeout,"
                            "no_data,valid_recv,valid_send,"
                            "B_recv,B_send,B_recv_max,B_send_max,"
                            "B_recv_ave,B_send_ave,B_recv_rate,B_send_rate)");

    printf(">>> wt create/open table: %s", wiredtiger_strerror(code));

    code = session_->create(session_, business_index.c_str(),
                            "columns=(timestamp)");
    printf("wt create/open index: %s\n", wiredtiger_strerror(code));

    reset_data_op();

    return true;
}

bool wt_t::close() {
    printf(">> wt start to close << \n");
    session_->close(session_, NULL);
    conn_->close(conn_, NULL);
    printf(">> wt closed << \n");
    return true;
}
bool wt_t::reset_data_op() {
    if (data_op_) {
        delete data_op_;
    }
    wt_cursor *cursor = NULL;
    int code = session_->open_cursor(session_, business_table.c_str(), NULL,
                                     NULL, &cursor);

    (void)code;
    data_op_ = new cursor_op(cursor);
    return true;
}

bool wt_t::read(uint32_t ip, uint16_t port, time_t since, time_t to,
                std::vector<business_info_t> &result) {
    result.clear();
    data_op_->reset();
    WT_OP_SET_KEY(data_op_, ip, port, since);

    int stat;
    int code = data_op_->search_near(&stat);
    if (code == WT_NOTFOUND) {
        printf("wt not found\n");
        return false;
    }
    printf("wt start\n");
    if (stat >= 0) {
        data_op_->prev();
    }
    while (data_op_->next()) {
        printf("wt next\n");
        business_info_t info;
        WT_OP_GET_KEY(data_op_, &info.service, &info.port, &info.timestamp);
        WT_OP_GET_VALS(
            data_op_, &info.session_created, &info.session_connected,
            &info.session_closing, &info.session_closed, &info.session_timeout,
            &info.session_no_data, &info.session_valid_recv,
            &info.session_valid_send, &info.Bytes_recv, &info.Bytes_send,
            &info.Bytes_recv_max, &info.Bytes_send_max, &info.Bytes_recv_ave,
            &info.Bytes_send_ave, &info.Bytes_rate_recv, &info.Bytes_rate_send);
        if (info.service < ip && info.port < port) {
            printf("continue %u %u %u %u\n", info.service, info.port, ip, port);
            continue;
        }

        if (info.service > ip || info.port > port) {
            printf("break1 %u %u %u %u\n", info.service, info.port, ip, port);
            break;
        }
        if (info.timestamp >= to) {
            printf("break2\n");
            break;
        }
        result.emplace_back(info);
    }
    return true;
}

bool wt_t::write(std::vector<business_info_t> &data) {
    data_op_->reset();
    for (auto info : data) {
        write(info);
    }
    return true;
}

bool wt_t::write(business_info_t &info) {
    data_op_->reset();
    WT_OP_SET_KEY(data_op_, info.service, info.port, info.timestamp);
    WT_OP_SET_VALS(
        data_op_, info.session_created, info.session_connected,
        info.session_closing, info.session_closed, info.session_timeout,
        info.session_no_data, info.session_valid_recv, info.session_valid_send,
        info.Bytes_recv, info.Bytes_send, info.Bytes_recv_max,
        info.Bytes_send_max, info.Bytes_recv_ave, info.Bytes_send_ave,
        info.Bytes_rate_recv, info.Bytes_rate_send);
    data_op_->insert();
    return true;
}

bool wt_t::remove(time_t before) {
    (void)before;
    return false;
}

} // namespace db
