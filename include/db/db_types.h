/*
 * @Author: vhdsih
 * @Date: 2022-06-19 11:37:48
 */

#ifndef CCL_DB_TYPES_H
#define CCL_DB_TYPES_H

#include <unistd.h>

#include <unordered_map>

namespace ccl {

enum class db_type_t : uint8_t {
    wiredtiger,
};

}

namespace std {

template <>
struct hash<ccl::db_type_t> {
    size_t operator()(const ccl::db_type_t type) const {
        return static_cast<size_t>(type);
    }
};

} // namespace std

#endif