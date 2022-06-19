#pragma once

namespace pool {

enum task_kind {
	tk_parse,

	tk_typecheck,

	tk_lower,

	tk_optimization,

	tk_link,

	tk_count,  // add above
};

#ifdef DEBUGGING

static const char* TASK_KIND_NAMES[] = {
	"PARSE", "TYPECHECK", "LOWER", "OPTIMIZATION", "LINK",
};

#endif

}  // namespace pool
