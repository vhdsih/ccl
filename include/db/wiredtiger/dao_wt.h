/*
 * @Author: vhdsih
 * @Date: 2022-06-19 23:56:42
 */

#ifndef CCL_DB_WIREDTIGER_DB_DAO_H
#define CCL_DB_WIREDTIGER_DB_DAO_H

#include <map>
#include <memory>

#include "../dao.h"
#include "wiredtiger.h"

namespace ccl {

class cursor_op;
class db_wt_t;

class dao_wt_t : public dao_t {
public:
    dao_wt_t(std::shared_ptr<db_wt_t> db);
    ~dao_wt_t();

    bool create(cstr_t &target, cstr_t &conf) override;

    bool exec(cstr_t, dao_item_t &, fn1_t) override;
    bool exec(cstr_t, dao_item_t &, dao_item_t &, fn2_t) override;

private:
    cursor_op *get_op(cstr_t &target) const {
        if (ops_.find(target) == ops_.end()) {
            return nullptr;
        }
        return ops_.at(target);
    }

private:
    std::shared_ptr<db_wt_t> db_;
    std::map<std::string, cursor_op *> ops_;
};

} // namespace ccl

#endif