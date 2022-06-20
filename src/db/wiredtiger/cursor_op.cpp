#include "cursor_op.h"

#include "wiredtiger.h"

namespace ccl {

cursor_op::~cursor_op() {
    if (cursor_) {
        cursor_->close(cursor_);
    }
}

bool cursor_op::next() { return (error_code_ = cursor_->next(cursor_)) == 0; }

bool cursor_op::prev() { return (error_code_ = cursor_->prev(cursor_)) == 0; }

bool cursor_op::reset() { return (error_code_ = cursor_->reset(cursor_)) == 0; }

bool cursor_op::search() {
    return (error_code_ = cursor_->search(cursor_)) == 0;
}

bool cursor_op::search_near(int *stat) {
    return (error_code_ = cursor_->search_near(cursor_, stat)) == 0;
}

bool cursor_op::insert() {
    return (error_code_ = cursor_->insert(cursor_)) == 0;
}

bool cursor_op::update() {
    return (error_code_ = cursor_->update(cursor_)) == 0;
}

bool cursor_op::remove() {
    return (error_code_ = cursor_->remove(cursor_)) == 0;
}

const char *cursor_op::err_info() const {
    return wiredtiger_strerror(error_code_);
}

const char *cursor_op::err_info(int code) const {
    return wiredtiger_strerror(code);
}

} // namespace ccl
