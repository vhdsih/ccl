/*
 * @Author: vhdsih
 * @Date: 2022-06-18 22:41:35
 */

#ifndef CCL_CCL_H
#define CCL_CCL_H

#include "aio/aio.h"
#include "aio/aio_epoll.h"
#include "aio/aio_factory.h"
#include "aio/aio_poll.h"
#include "aio/aio_select.h"
#include "aio/aio_types.h"
#include "db/dao.h"
#include "db/db.h"
#include "db/db_factory.h"
#include "db/db_types.h"
#include "pipe/better_pipe.h"
#include "pipe/pipe.h"
#include "types/buffer.h"
#include "types/cache.h"
#include "types/cache_with_time.h"
#include "utils/noncopyable.h"

namespace ccl {

static const char *CCL_VERSION = "0.0.1";

void hello();

const char *version();

} // namespace ccl

#endif // CCL_CCL_H
