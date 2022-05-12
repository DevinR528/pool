
// vim: set tabstop=4 smarttab

#pragma once

#include <cassert>

#define TODO(msg) \
	assert(!msg); \
	exit(1)

#define OPT_TRY(expr) \
	({ \
		auto res = (expr); \
		if (!res.has_value()) return (res); \
		res.value(); \
	})

#define OPT_TRY_OR(expr, fail) \
	({ \
		auto res = (expr); \
		if (!res.has_value()) return (fail); \
		res.value(); \
	})

#define RES_TRY(expr) \
	({ \
		auto res = (expr); \
		if (!res.has_value()) return res.error(); \
		res.value(); \
	})
