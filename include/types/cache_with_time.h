/*
 * @Author: vhdsih
 * @Date: 2022-06-18 22:41:35
 */

// <key, value> cache with timeout,
// given a time interval, you can quickly delete elements
// that have not been visited beyond that interval

#ifndef CCL_CACHE_WITH_TIME_H
#define CCL_CACHE_WITH_TIME_H

#include <functional>
#include <iostream>
#include <list>
#include <unordered_map>

namespace ccl {

/* the item saved in the cache */
template <typename Key, typename Value>
struct cache_item_t {
    cache_item_t(const Key &k, const Value &v) :
        key(k), val(v), last_touch(time(nullptr)) {}
    Key key;
    Value val;
    time_t last_touch;
};

template <typename Key, typename Value>
class cache_with_time_t {
public:
    /* for debug */
    typedef void (*debug_func_t)(const Key &key, const Value &val, time_t t);

    /* key should not be released */
    /* if the value need to be released, use this */
    typedef void (*val_release_fn)(Value &);

    /* default timeout is 5 min */
    cache_with_time_t() : timeout_(300) {}

    cache_with_time_t(uint32_t timeout, val_release_fn fn = nullptr) :
        timeout_(timeout), val_release_fn_(fn) {}

    ~cache_with_time_t();

    /* get the value in the cache by key,
     * write the value into val, return true if successed */
    bool get(const Key &key, Value &val);

    /* set key and value */
    void set(const Key &key, const Value &val);

    /* delete by key */
    void erase(const Key &key);

    /* remove timeout items */
    void timeout_clean(
        std::function<void(Key &key, Value &val)> timeout_fn = nullptr);

    void debug(debug_func_t func) const;

    size_t size() const { return key2it_.size(); }

    /* copy disabled */
    cache_with_time_t(cache_with_time_t &)                  = delete;
    cache_with_time_t &operator=(const cache_with_time_t &) = delete;

private:
    uint32_t timeout_;                         // second
    val_release_fn val_release_fn_;            // release value
    std::list<cache_item_t<Key, Value>> data_; // <key, value> save in here
    std::unordered_map<Key,
                       typename std::list<cache_item_t<Key, Value>>::iterator>
        key2it_; // key to iterator of data_
};

template <typename Key, typename Value>
cache_with_time_t<Key, Value>::~cache_with_time_t() {
    while (!data_.empty()) {
        auto &val = data_.back();
        if (val_release_fn_) {
            val_release_fn_(val.val);
        }
        data_.pop_back();
    }
}

template <typename Key, typename Value>
bool cache_with_time_t<Key, Value>::get(const Key &key, Value &val) {
    auto it = key2it_.find(key);
    if (it == key2it_.end()) {
        return false;
    } else {
        val = it->second->val;
        return true;
    }
}

template <typename Key, typename Value>
void cache_with_time_t<Key, Value>::set(const Key &key, const Value &val) {
    auto it = key2it_.find(key);
    if (it != key2it_.end()) {
        auto item       = *it->second;
        item.last_touch = time(nullptr);
        if (item.val != val) {
            if (val_release_fn_) val_release_fn_(item.val);
            item.val = val;
        }
        data_.erase(it->second);
        data_.emplace_front(item);
    } else {
        data_.emplace_front(cache_item_t<Key, Value>(key, val));
    }
    key2it_[key] = data_.begin();
}

template <typename Key, typename Value>
void cache_with_time_t<Key, Value>::timeout_clean(
    std::function<void(Key &key, Value &v)> timeout_fn) {
    time_t now = time(nullptr);
    while (!data_.empty()) {
        auto &item = data_.back();
        if (now - item.last_touch >= timeout_) {
            if (timeout_fn) {
                timeout_fn(item.key, item.val);
            }
            if (val_release_fn_) {
                val_release_fn_(item.val);
            }
            data_.pop_back();
            key2it_.erase(key2it_.find(item.key));
        } else {
            break;
        }
    }
}

template <typename Key, typename Value>
void cache_with_time_t<Key, Value>::debug(debug_func_t func) const {
    /* operate every <key, value> in the cache */
    for (auto it = data_.begin(); it != data_.end(); ++it) {
        func(it->key, it->val, it->last_touch);
    }
}

template <typename Key, typename Value>
void cache_with_time_t<Key, Value>::erase(const Key &key) {
    /* find the iterator of the data_, should be it->second (type cache_item_t)
     */
    auto it = key2it_.find(key);
    if (it != key2it_.end()) {
        /* try to release the value */
        if (val_release_fn_) {
            val_release_fn_(it->second->val);
        }
        /* then remove from the cache */
        data_.erase(it->second);
        key2it_.erase(it);
    }
}

} // namespace ccl

#endif
