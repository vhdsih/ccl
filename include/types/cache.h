/*
 * @Author: vhdsih
 * @Date: 2022-06-15 00:19:38
 */

#ifndef CCL_CACHE_H
#define CCL_CACHE_H

#include <unordered_map>

namespace ccl {

template <typename Key, typename Value>
class cache_t {
public:
    using release_fn = void (*)(Value *);
    cache_t(release_fn fn);
    ~cache_t();

    Value *get(const Key &key);
    void set(const Key &key, const Value *val);
    void remove(const Key &key);
    void clear(const Key &key);

private:
    release_fn release_fn_;
    std::unordered_map<Key, Value *> data_;
};

}  // namespace erspan

#endif  // ERSPAN_AGENT_COMMON_CACHE_H