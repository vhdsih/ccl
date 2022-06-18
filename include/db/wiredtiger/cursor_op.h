/**
 * Copyright (C) 2015-2020 IQIYI
 * All rights reserved.
 *
 * AUTHOR(S)
 *   zhangdongchang
 *   E-mail: zhangdongchang@qiyi.com
 *
 */

// 用于wiredtiger游标的操作
//

#ifndef ERSPAN_ANGENT_DAO_CURSOR_OP_H
#define ERSPAN_ANGENT_DAO_CURSOR_OP_H

#include <string>

#include "wiredtiger.h"

// 由于wiredtiger的setter和getter方法都使用了
// 变长参数，而且必须使用原始cursor指针操作
// 为了简化操作，引入4个宏函数，用于wiredtiger
// 的key和value的读取和写入

// 设置key，op为cursor_op的实例
#define WT_OP_SET_KEY(op, ...) \
    op->get_cursor()->set_key(op->get_cursor(), __VA_ARGS__)

// 获取key，op为cursor_op的实例
#define WT_OP_SET_VALS(op, ...) \
    op->get_cursor()->set_value(op->get_cursor(), __VA_ARGS__)

// 设置values，op为cursor_op的实例
#define WT_OP_GET_KEY(op, ...) \
    op->get_cursor()->get_key(op->get_cursor(), __VA_ARGS__)

// 获取values，op为cursor_op的实例
#define WT_OP_GET_VALS(op, ...) \
    op->get_cursor()->get_value(op->get_cursor(), __VA_ARGS__)

namespace db {

typedef WT_CURSOR wt_cursor;

class cursor_op {
public:
    cursor_op(wt_cursor *cursor) : error_code_(0), cursor_(cursor) {}
    cursor_op(const cursor_op &) = delete;
    cursor_op &operator=(const cursor_op &) = delete;
    ~cursor_op();

    wt_cursor *get_cursor() { return cursor_; }

    // 移动cursor指向下一条数据
    bool next();

    // 移动cursor指向前一条数据
    bool prev();

    // 释放cursor占用的所有资源，复位cursor
    // 此后当调用next()，cursor将指向第一条
    // 数据，若调用prev()，将指向最后一条
    bool reset();

    // 精确查找设置了key的cursor的位置
    // 必须首先用WT_OP_SET_KEY设置待查找的key
    bool search();

    // 查找设置了key的最临近的cursor位置
    // 必须首先用WT_OP_SET_KEY设置待查找的key
    bool search_near(int *stat);

    // 插入设置好的key和value
    // 必须首先用WT_OP_SET_KEY设置待插入的key
    // 必须首先用WT_OP_SET_VALS设置待插入的value
    bool insert();

    // 更新
    bool update();

    // 删除设置好的key和value
    // 必须首先用WT_OP_SET_KEY设置待删除的key
    bool remove();

    // 返回stat代表的错误信息
    const char *err_info() const;

    const char *err_info(int code) const;

private:
    bool error_code_;
    wt_cursor *cursor_;
};

}  // namespace db

#endif