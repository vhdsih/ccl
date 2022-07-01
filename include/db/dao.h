/*
 * @Author: vhdsih
 * @Date: 2022-06-18 23:25:07
 */

#ifndef CCL_DB_DAO_H
#define CCL_DB_DAO_H

#include <functional>
#include <string>
#include <vector>

#include "../utils/noncopyable.h"
#include "dao_item.h"
#include "dao_items.h"

namespace ccl {

class db_t;
class dao_t : public noncopyable {
public:
    using cstr_t  = const std::string &;
    using fn_i1_t = std::function<bool(void *, dao_item_t &)>;
    using fn_s1_t = std::function<bool(void *, dao_item_t &, dao_items_t &)>;
    using fn_s2_t =
        std::function<bool(void *, dao_item_t &, dao_item_t &, dao_items_t &)>;

    virtual ~dao_t() = default;

    virtual bool create(cstr_t &target, cstr_t &conf) = 0;

    virtual bool exec(cstr_t, dao_item_t &, fn_i1_t) = 0;

    virtual bool exec(cstr_t, dao_item_t &, dao_items_t &, fn_s1_t) = 0;

    virtual bool exec(cstr_t, dao_item_t &, dao_item_t &, dao_items_t &,
                      fn_s2_t) = 0;
};

}; // namespace ccl

#endif