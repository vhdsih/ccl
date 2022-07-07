/*
 * @Author: vhdsih
 * @Date: 2022-06-20 00:00:25
 */

#include "dao_wt.h"

#include "cursor_op.h"
#include "db_wt.h"
#include "log.h"
#include "wiredtiger.h"

namespace ccl {

dao_wt_t::dao_wt_t(std::shared_ptr<db_wt_t> db) : created_(false), db_(db) {
    ccl_glog_info << "dao wt create";
}
dao_wt_t::~dao_wt_t() {
    for (auto &obj2op : ops_) {
        delete obj2op.second;
    }
    ops_.clear();
    ccl_glog_info << "dao wt released";
}

bool dao_wt_t::create(cstr_t &target, cstr_t &conf) {
    if (created_) {
        return true;
    } else {
        created_ = true;
    }
    auto session = db_->get_session();
    int code     = session->create(session, target.c_str(), conf.c_str());

    wt_cursor cursor = nullptr;
    code = session->open_cursor(session, target.c_str(), NULL, NULL, &cursor);
    auto op      = new cursor_op(cursor);
    ops_[target] = op;
    return code == 0;
}

bool dao_wt_t::exec(cstr_t where, dao_item_t &item, fn_i1_t fn) {
    // TODO:
    if (!created_) return false;
    return fn(get_op(where), item);
}

bool dao_wt_t::exec(cstr_t where, dao_item_t &target, dao_items_t &ans,
                    fn_s1_t fn) {
    if (!created_) return false;
    return fn(get_op(where), target, ans);
}
bool dao_wt_t::exec(cstr_t where, dao_item_t &from, dao_item_t &to,
                    dao_items_t &ans, fn_s2_t fn) {
    if (!created_) return false;
    return fn(get_op(where), from, to, ans);
}

} // namespace ccl