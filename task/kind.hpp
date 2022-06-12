#pragma once

namespace pool {

enum task_kind {
	tk_parse,

	tk_typecheck,

	tk_compile,

	tk_optimization,

	tk_link,

	tk_count,  // add above
};

}  // namespace pool
