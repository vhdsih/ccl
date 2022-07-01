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
#include "db/dao_item.h"
#include "db/dao_items.h"
#include "db/db.h"
#include "db/db_factory.h"
#include "db/db_types.h"
#include "db/wiredtiger/cursor_op.h"
#include "db/wiredtiger/dao_wt.h"
#include "db/wiredtiger/db_wt.h"
#include "pipe/fake_pipe.h"
#include "pipe/inited_pipe.h"
#include "pipe/inited_pipe2.h"
#include "pipe/pipe.h"
#include "pipe/pipe_helper.h"
#include "types/cache.h"
#include "types/cache_with_time.h"
#include "utils/log.h"
#include "utils/noncopyable.h"
#include "utils/ptr.h"

namespace ccl {

void hello();

const char *version();

} // namespace ccl

#endif // CCL_CCL_H
