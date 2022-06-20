/*
 * @Author: vhdsih
 * @Date: 2022-06-20 00:00:25
 */

#include "dao_wt.h"

#include "cursor_op.h"
#include "db_wt.h"
#include "log.h"

namespace ccl {

dao_wt_t::dao_wt_t(std::shared_ptr<db_wt_t> db) : db_(db) {
    logger("dao_wt_t::inited");
}
dao_wt_t::~dao_wt_t() { logger("dao_wt_t::end"); }

bool dao_wt_t::create(cstr_t &target, cstr_t &conf) {
    auto session = db_->get_session();
    int code     = session->create(session, target.c_str(), conf.c_str());

    wt_cursor cursor = nullptr;
    code = session->open_cursor(session, target.c_str(), NULL, NULL, &cursor);
    auto op      = new cursor_op(cursor);
    ops_[target] = op;

    return code == 0;
}

bool dao_wt_t::exec(cstr_t where, dao_item_t &item, fn1_t fn) {
    // TODO:
    return fn(get_op(where), item);
}

bool dao_wt_t::exec(cstr_t where, dao_item_t &from, dao_item_t &to, fn2_t fn) {
    // TODO:
    return fn(get_op(where), from, to);
}

} // namespace ccl