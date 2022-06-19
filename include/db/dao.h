/*
 * @Author: vhdsih
 * @Date: 2022-06-18 23:25:07
 */

#ifndef CCL_DB_DAO_H
#define CCL_DB_DAO_H

#include <string>

#include "noncopyable.h"

namespace ccl {

class dao_t : public noncopyable {
public:
    dao_t(const std::string &obj_name, const std::string &obj_conf) :
        obj_name_(obj_name), obj_conf_(obj_conf) {}

    virtual ~dao_t() = default;

    virtual bool check()  = 0;
    virtual bool create() = 0;
    virtual bool remove() = 0;

    virtual bool erase()  = 0;
    virtual bool insert() = 0;
    virtual bool search() = 0;

private:
    std::string obj_name_;
    std::string obj_conf_;
};

}; // namespace ccl

#endif