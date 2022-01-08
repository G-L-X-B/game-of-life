#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

#include "log.c/src/log.h"

#define lg_set_level(level) log_set_level(level)
#define lg_set_quiet(enable) log_set_quiet(enable)
#define lg_add_fp(fp,level) log_add_fp(fp, level)

#define lg_trace(...) log_trace(__VA_ARGS__)
#define lg_debug(...) log_debug(__VA_ARGS__)
#define lg_info(...)  log_info(__VA_ARGS__)
#define lg_warn(...)  log_warn(__VA_ARGS__)
#define lg_error(...) log_error(__VA_ARGS__)
#define lg_fatal(...) log_fatal(__VA_ARGS__)

#define lg_inv_argument(fun_name) lg_error("In function \"%s\": invalid argument(-s).", fun_name)

#endif  // LOGGER_H
