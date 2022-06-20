/*
 * @Author: vhdsih
 * @Date: 2022-06-19 10:29:17
 */

#ifndef CCL_DB_FACTORY_H
#define CCL_DB_FACTORY_H

#include <memory>

#include "db.h"
#include "db_types.h"
#include "wiredtiger/db_wt.h"

namespace ccl {

template <typename T>
class db_factory_t {
public:
    static std::unique_ptr<T> create(db_type_t type, const std::string &name,
                                     const std::string &conf);

    db_factory_t()  = delete;
    ~db_factory_t() = delete;
};

template <typename T>
inline std::unique_ptr<T> db_factory_t<T>::create(db_type_t type,
                                                  const std::string &name,
                                                  const std::string &conf) {
    switch (type) {
    case db_type_t::wiredtiger:
        return std::unique_ptr<db_wt_t>(new db_wt_t(name, conf));
    default:;
        return nullptr;
    }
}

} // namespace ccl

#endif