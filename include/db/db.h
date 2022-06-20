/*
 * @Author: vhdsih
 * @Date: 2022-06-18 23:18:52
 */

#ifndef CCL_DB_H
#define CCL_DB_H

#include <memory>
#include <string>

#include "../utils/noncopyable.h"
#include "dao.h"
#include "db_types.h"

namespace ccl {

class db_t : public noncopyable {
public:
    db_t(const std::string &name, const std::string &conf) :
        db_name_(name), db_conf_(conf) {}

    virtual ~db_t() = default;

    virtual bool check()  = 0;
    virtual bool close()  = 0;
    virtual bool create() = 0;
    virtual bool remove() = 0;

    virtual db_type_t type() const = 0;

    // virtual bool bind_dao(int, std::unique_ptr<dao_t>)       = 0;
    // virtual const std::unique_ptr<dao_t> &get_dao(int) const = 0;

protected:
    std::string db_name_;
    std::string db_conf_;
};

}; // namespace ccl

#endif