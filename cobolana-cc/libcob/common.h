/*
 * Minimal stub of libcob/common.h for cobolana-cc
 * Only includes types and macros needed by cobc
 */

#ifndef COB_COMMON_H
#define COB_COMMON_H

#include <stdint.h>
#include <time.h>

/* Type definitions */
typedef uint8_t  cob_u8_t;
typedef uint16_t cob_u16_t;
typedef uint32_t cob_u32_t;
typedef uint64_t cob_u64_t;

typedef int8_t   cob_s8_t;
typedef char      cob_c8_t;
typedef int16_t  cob_s16_t;
typedef int32_t  cob_s32_t;
typedef int64_t  cob_s64_t;

/* Time structure */
struct cob_time {
	cob_s64_t year;
	cob_s64_t month;
	cob_s64_t day_of_month;
	cob_s64_t day_of_week;
	cob_s64_t day_of_year;
	cob_s64_t hour;
	cob_s64_t minute;
	cob_s64_t second;
	cob_s64_t nanosecond;
	cob_s64_t offset_known;
	cob_s64_t utc_offset;
	cob_s64_t is_daylight_saving_time;
};

/* Compiler attributes */
#ifdef __GNUC__
#define DECLNORET
#define COB_A_NORETURN    __attribute__((noreturn))
#define COB_A_FORMAT12    __attribute__((format(printf, 1, 2)))
#define COB_A_FORMAT23    __attribute__((format(printf, 2, 3)))
#define COB_A_FORMAT34    __attribute__((format(printf, 3, 4)))
#define COB_A_FORMAT45    __attribute__((format(printf, 4, 5)))
#define COB_A_PURE        __attribute__((pure))
#else
#define DECLNORET
#define COB_A_NORETURN
#define COB_A_FORMAT12
#define COB_A_FORMAT23
#define COB_A_FORMAT34
#define COB_A_FORMAT45
#define COB_A_PURE
#endif

/* Utility macros */
#define ONCE_COB while(0)

/* Exception enum - generates COB_EC_ALL, COB_EC_I_O, etc. */
#define	COB_EXCEPTION(code,tag,name,fatal) tag,
enum cob_exception_id {
	COB_EC_ZERO = 0,
#include "exception.def"
	COB_EC_MAX
};
#undef	COB_EXCEPTION

#endif /* COB_COMMON_H */
