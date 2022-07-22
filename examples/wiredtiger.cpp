/*
 * @Author: vhdsih
 * @Date: 2022-06-19 21:49:19
 */

#include "ccl.h"

class data_t : public ccl::dao_item_t {
public:
    data_t()           = default;
    ~data_t() override = default;
    int a;
    int b;
    int c;
};

int main() {
    ccl::try_mkdir("./WT_HOME/logs");

    auto handle = ccl::db_factory_t<ccl::db_wt_t>::create(
        ccl::db_type_t::wiredtiger, "./WT_HOME", "create");
    handle->create();
    std::shared_ptr<ccl::db_wt_t> db = std::move(handle);

    ccl::dao_wt_t dao(db);
    std::string target = "table:business_info";
    std::string conf   = "key_format=I,value_format=II,columns=(a,b,c)";

    dao.create(target, conf);

    auto insert_fn = [](void *cursor, ccl::dao_item_t &item) -> bool {
        auto *op = static_cast<ccl::cursor_op *>(cursor);
        auto x   = dynamic_cast<data_t &>(item);
        printf("a %p\n", op);
        WT_OP_SET_KEY(op, x.a);
        printf("b\n");
        WT_OP_SET_VALS(op, x.b, x.c);
        printf("c\n");
        return op->insert();
    };

    data_t data;
    data.a = 1;
    data.a = 2;
    data.a = 3;
    dao.exec(target, data, insert_fn);

    return 0;
}