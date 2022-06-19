/*
 * @Author: vhdsih
 * @Date: 2022-06-19 10:29:17
 */

#ifndef CCL_DB_FACTORY_H
#define CCL_DB_FACTORY_H

#include <memory>

#include "db.h"
#include "db_types.h"

namespace ccl {
class db_factory_t {
public:
    static std::unique_ptr<db_t> create(db_type_t type);

    db_factory_t()  = delete;
    ~db_factory_t() = delete;
};

inline std::unique_ptr<db_t> db_factory_t::create(db_type_t type) {
    switch (type) {
    case db_type_t::wiredtiger:
        // TODO:
        return nullptr;
    default:;
        return nullptr;
    }
}

} // namespace ccl

#endif