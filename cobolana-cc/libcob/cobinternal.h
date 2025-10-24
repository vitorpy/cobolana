/*
 * Minimal stub of libcob/cobinternal.h for cobolana-cc
 */

#ifndef COB_INTERNAL_H
#define COB_INTERNAL_H

#include "common.h"
#include <stdlib.h>
#include <string.h>

/* Inline macro */
#ifdef __GNUC__
#define COB_INLINE		inline
#define COB_A_INLINE	__attribute__((always_inline))
#else
#define COB_INLINE
#define COB_A_INLINE
#endif

/* Buffer sizes */
#define COB_MINI_BUFF		256
#define COB_MINI_MAX		(COB_MINI_BUFF - 1)
#define COB_SMALL_BUFF		1024
#define COB_SMALL_MAX		(COB_SMALL_BUFF - 1)
#define COB_NORMAL_BUFF		4096
#define COB_NORMAL_MAX		(COB_NORMAL_BUFF - 1)
#define COB_MAX_FIELD_SIZE	268435456
#define COB_MAX_UNBOUNDED_SIZE	999999999
#define COB_MAX_FIELD_SIZE_LINKAGE	999999999
#define COB_MAX_DIGITS		38
#define COB_MAX_WORDLEN		63
#define COB_NATIONAL_SIZE	2
#define MAX_FILE_KEYS		64

/* File path separator */
#ifdef _WIN32
#define SLASH_CHAR		'\\'
#else
#define SLASH_CHAR		'/'
#endif

/* Float constants */
#define COB_FLOAT_DIGITS_MAX	36

/* Stringification macros */
#define CB_STRINGIFY(s)		#s
#define CB_XSTRINGIFY(s)	CB_STRINGIFY(s)

/* Format strings */
#ifdef _WIN32
#define CB_FMT_LLU		"%I64u"
#define CB_FMT_LLD		"%I64d"
#else
#define CB_FMT_LLU		"%llu"
#define CB_FMT_LLD		"%lld"
#endif

/* Type definitions needed by tree.h */
typedef unsigned int cob_flags_t;

/* PIC symbol structure stub */
typedef struct {
	unsigned char	symbol;
	unsigned char	times_repeated;
} cob_pic_symbol;

/* Enums needed by tree.h */
enum cob_file_org {
	COB_ORG_SEQUENTIAL,
	COB_ORG_LINE_SEQUENTIAL,
	COB_ORG_RELATIVE,
	COB_ORG_INDEXED,
	COB_ORG_SORT
};

enum cob_file_access {
	COB_ACCESS_SEQUENTIAL,
	COB_ACCESS_DYNAMIC,
	COB_ACCESS_RANDOM
};

enum cob_statement {
	STMT_UNKNOWN = 0,
	STMT_ACCEPT,
	STMT_ADD,
	STMT_ALLOCATE,
	STMT_ALTER,
	STMT_CALL,
	STMT_CANCEL,
	STMT_CLOSE,
	STMT_COMMIT,
	STMT_COMPUTE,
	STMT_CONTINUE,
	STMT_DELETE,
	STMT_DELETE_FILE,
	STMT_DISPLAY,
	STMT_DIVIDE,
	STMT_ENTRY,
	STMT_EVALUATE,
	STMT_EXIT,
	STMT_FREE,
	STMT_GENERATE,
	STMT_GO_TO,
	STMT_IF,
	STMT_INITIALIZE,
	STMT_INITIATE,
	STMT_INSPECT,
	STMT_MERGE,
	STMT_MOVE,
	STMT_MULTIPLY,
	STMT_OPEN,
	STMT_PERFORM,
	STMT_READ,
	STMT_READY_TRACE,
	STMT_RECEIVE,
	STMT_RELEASE,
	STMT_RESET_TRACE,
	STMT_RETURN,
	STMT_REWRITE,
	STMT_ROLLBACK,
	STMT_SEARCH,
	STMT_SEND,
	STMT_SET,
	STMT_SORT,
	STMT_START,
	STMT_STOP,
	STMT_STRING,
	STMT_SUBTRACT,
	STMT_SUPPRESS,
	STMT_TERMINATE,
	STMT_TRANSFORM,
	STMT_UNLOCK,
	STMT_UNSTRING,
	STMT_VALIDATE,
	STMT_WRITE,
	STMT_XML_GENERATE,
	STMT_XML_PARSE,
	STMT_JSON_GENERATE,
	STMT_JSON_PARSE,
	STMT_NEXT_SENTENCE,
	STMT_CLOSE_WINDOW,
	STMT_CONTINUE_AFTER,
	STMT_DESTROY,
	STMT_DISABLE,
	STMT_DISPLAY_WINDOW,
	STMT_ENABLE,
	STMT_ENTRY_FOR_GO_TO,
	STMT_EXHIBIT,
	STMT_EXIT_PROGRAM,
	STMT_EXIT_FUNCTION,
	STMT_EXIT_PERFORM_CYCLE,
	STMT_EXIT_PERFORM,
	STMT_EXIT_SECTION,
	STMT_EXIT_PARAGRAPH,
	STMT_GOBACK,
	STMT_INQUIRE,
	STMT_EXAMINE,
	STMT_MODIFY,
	STMT_PURGE,
	STMT_RAISE,
	STMT_SEARCH_ALL,
	STMT_STOP_RUN,
	STMT_STOP_ERROR,
	STMT_STOP_THREAD,
	STMT_AT_END,
	STMT_CHAIN,
	STMT_SEARCH_VARYING,
	STMT_LOCK_THREAD,
	STMT_WAITH_THREAD,
	STMT_RESUME,
	STMT_END,
	STMT_NOTE,
	STMT_UNTIL,
	STMT_VARYING,
	STMT_END_PERFORM,
	STMT_END_SET,
	STMT_ENTER,
	STMT_INVOKE,
	STMT_END_INVOKE,
	STMT_ELSE,
	STMT_WHEN,
	STMT_END_EVALUATE,
	STMT_END_IF,
	STMT_EXECUTE,
	STMT_LOCK_FILE,
	STMT_OTHERWISE,
	STMT_RECOVER,
	STMT_SERVICE,
	STMT_INIT_STORAGE,
	STMT_VALUE_THRU,
	STMT_WHEN_OTHER,
	STMT_PRESENT_WHEN,
	STMT_BEFORE_CALL,
	STMT_BEFORE_UDF,
	STMT_MAX_ENTRY
};

enum cob_module_type {
	COB_MODULE_TYPE_PROGRAM = 0,
	COB_MODULE_TYPE_FUNCTION
};

enum cob_prof_procedure_kind {
	COB_PROF_PROCEDURE_UNKNOWN = 0,
	COB_PROF_PROCEDURE_SECTION,
	COB_PROF_PROCEDURE_PARAGRAPH,
	COB_PROF_PROCEDURE_ENTRY,
	COB_PROF_PROCEDURE_CALL,
	COB_PROF_PROCEDURE_MODULE
};

enum cob_open_mode {
	COB_OPEN_CLOSED = 0,
	COB_OPEN_INPUT,
	COB_OPEN_OUTPUT,
	COB_OPEN_I_O,
	COB_OPEN_EXTEND
};

struct cob_prof_procedure {
	enum cob_prof_procedure_kind kind;
	const char *name;
	const char *text;
	const char *file;
	int line;
	int section;
};

/* Constants needed by tree.h */
#define COB_MAX_SUBSCRIPTS 16

/* Check definitions stub */
#define COB_CHK_SUBSCRIPT 1

/* Screen attribute flags */
#define COB_SCREEN_AUTO                  (1 << 0)
#define COB_SCREEN_BELL                  (1 << 1)
#define COB_SCREEN_BLANK_LINE            (1 << 2)
#define COB_SCREEN_BLANK_SCREEN          (1 << 3)
#define COB_SCREEN_BLINK                 (1 << 4)
#define COB_SCREEN_COLUMN_MINUS          (1 << 5)
#define COB_SCREEN_COLUMN_PLUS           (1 << 6)
#define COB_SCREEN_ERASE_EOL             (1 << 7)
#define COB_SCREEN_ERASE_EOS             (1 << 8)
#define COB_SCREEN_FULL                  (1 << 9)
#define COB_SCREEN_HIGHLIGHT             (1 << 10)
#define COB_SCREEN_INITIAL               (1 << 11)
#define COB_SCREEN_INPUT                 (1 << 12)
#define COB_SCREEN_LINE_MINUS            (1 << 13)
#define COB_SCREEN_LINE_PLUS             (1 << 14)
#define COB_SCREEN_LOWLIGHT              (1 << 15)
#define COB_SCREEN_REQUIRED              (1 << 16)
#define COB_SCREEN_REVERSE               (1 << 17)
#define COB_SCREEN_SECURE                (1 << 18)
#define COB_SCREEN_UNDERLINE             (1 << 19)
#define COB_SCREEN_OVERLINE              (1 << 20)
#define COB_SCREEN_GRID                  (1 << 21)
#define COB_SCREEN_LEFTLINE              (1 << 22)
#define COB_SCREEN_RIGHTLINE             (1 << 23)
#define COB_SCREEN_TAB                   (1 << 24)
#define COB_SCREEN_NO_ECHO               (1 << 25)
#define COB_SCREEN_PROMPT                (1 << 26)
#define COB_SCREEN_NO_UPDATE             (1 << 27)
#define COB_SCREEN_UPDATE                (1 << 28)
#define COB_SCREEN_CONV                  (1 << 29)
#define COB_SCREEN_LOWER                 (1 << 30)
#define COB_SCREEN_UPPER                 (1U << 31)
#define COB_SCREEN_SCROLL_UP             (1 << 0)
#define COB_SCREEN_SCROLL_DOWN           (1 << 1)

/* Rounding modes */
#define COB_STORE_ROUND                  0
#define COB_STORE_NEAR_AWAY_FROM_ZERO    1
#define COB_STORE_NEAR_EVEN              2
#define COB_STORE_PROHIBITED             3
#define COB_STORE_TRUNCATION             4
#define COB_STORE_AWAY_FROM_ZERO         5
#define COB_STORE_NEAR_TOWARD_ZERO       6
#define COB_STORE_TOWARD_GREATER         7
#define COB_STORE_TOWARD_LESSER          8

/* File size limits */
#define MAX_FD_RECORD                    65535
#define MAX_FD_RECORD_IDX                16777215

/* File special values */
#define COB_SELECT_FILE                  0
#define COB_SELECT_STDIN                 1
#define COB_SELECT_STDOUT                2

/* File line advance */
#define COB_LINE_ADVANCE                 1

/* Lock modes */
#define COB_LOCK_MANUAL                  (1 << 0)
#define COB_LOCK_AUTOMATIC               (1 << 1)
#define COB_LOCK_EXCLUSIVE               (1 << 2)
#define COB_LOCK_MULTIPLE                (1 << 3)
#define COB_LOCK_ROLLBACK                (1 << 4)

/* Share modes */
#define COB_SHARE_ALL_OTHER              0
#define COB_SHARE_NO_OTHER               1
#define COB_SHARE_READ_ONLY              2

/* Sorting */
#define COB_ASCENDING                    0
#define COB_DESCENDING                   1

/* Report types and modes */
#define COB_REPORT_LINE                  0
#define COB_REPORT_HEADING               1
#define COB_REPORT_PAGE_HEADING          2
#define COB_REPORT_DETAIL                3
#define COB_REPORT_PAGE_FOOTING          4
#define COB_REPORT_FOOTING               5
#define COB_REPORT_CONTROL_HEADING       6
#define COB_REPORT_CONTROL_HEADING_FINAL 7
#define COB_REPORT_CONTROL_FOOTING       8
#define COB_REPORT_CONTROL_FOOTING_FINAL 9
#define COB_REPORT_ALL                   10
#define COB_REPORT_PAGE                  11
#define COB_REPORT_NEXT_GROUP_PLUS       12
#define COB_REPORT_NEXT_GROUP_LINE       13
#define COB_REPORT_NEXT_GROUP_PAGE       14
#define COB_REPORT_RESET_FINAL           15
#define COB_REPORT_PRESENT               16
#define COB_REPORT_BEFORE                17
#define COB_REPORT_NEGATE                18
#define COB_REPORT_LINE_NEXT_PAGE        19
#define COB_REPORT_LINE_PLUS             20
#define COB_REPORT_COLUMN_LEFT           21
#define COB_REPORT_COLUMN_RIGHT          22
#define COB_REPORT_COLUMN_CENTER         23
#define COB_REPORT_COLUMN_PLUS           24
#define COB_REPORT_GROUP_INDICATE        25

/* File close modes */
#define COB_CLOSE_NORMAL                 0
#define COB_CLOSE_NO_REWIND              1
#define COB_CLOSE_LOCK                   2
#define COB_CLOSE_UNIT                   3
#define COB_CLOSE_UNIT_REMOVAL           4

/* Additional lock modes */
#define COB_LOCK_OPEN_EXCLUSIVE          (1 << 5)

/* Comparison operators */
#define COB_EQ                           1
#define COB_LT                           2
#define COB_LE                           3
#define COB_GT                           4
#define COB_GE                           5
#define COB_NE                           6
#define COB_FI                           7
#define COB_LA                           8

/* Exception codes */
#define COB_EC_I_O                       0x0100

/* Fold modes */
#define COB_FOLD_NONE                    0
#define COB_FOLD_UPPER                   1
#define COB_FOLD_LOWER                   2

/* Field types */
#define COB_TYPE_GROUP                   0x00
#define COB_TYPE_BOOLEAN                 0x01
#define COB_TYPE_NUMERIC_DISPLAY         0x10
#define COB_TYPE_NUMERIC_BINARY          0x11
#define COB_TYPE_NUMERIC_PACKED          0x12
#define COB_TYPE_NUMERIC_FLOAT           0x13
#define COB_TYPE_NUMERIC_DOUBLE          0x14
#define COB_TYPE_NUMERIC_L_DOUBLE        0x15
#define COB_TYPE_NUMERIC_FP_BIN32        0x16
#define COB_TYPE_NUMERIC_FP_BIN64        0x17
#define COB_TYPE_NUMERIC_FP_BIN128       0x18
#define COB_TYPE_NUMERIC_FP_DEC64        0x19
#define COB_TYPE_NUMERIC_FP_DEC128       0x1A
#define COB_TYPE_NUMERIC_EDITED          0x24
#define COB_TYPE_ALPHANUMERIC            0x21
#define COB_TYPE_ALPHANUMERIC_EDITED     0x22
#define COB_TYPE_NATIONAL                0x40
#define COB_TYPE_NATIONAL_EDITED         0x41
#define COB_TYPE_NUMERIC_FP_DEC128       0x1A
#define COB_TYPE_NUMERIC_EDITED          0x24

/* Stub for validation functions */
static inline int cob_valid_datetime_format(const char *format, char type) { (void)format; (void)type; return 1; }
static inline int cob_valid_date_format(const char *format) { (void)format; return 1; }
static inline int cob_valid_time_format(const char *format, char type) { (void)format; (void)type; return 1; }
#define DEBUG_LOG(...)
static inline cob_s64_t cob_s32_pow(cob_s32_t base, cob_s32_t exp) { cob_s64_t result = 1; for(int i = 0; i < exp; i++) result *= base; return result; }

#endif /* COB_INTERNAL_H */

/* Additional exception codes */
#define COB_EC_DATA_INCOMPATIBLE     0x0200
#define COB_EC_BOUND_REF_MOD         0x0300

/* Stub for encode function */
static inline void cob_encode_program_id(const unsigned char *name, char *buff, int max, int fold) { (void)name; (void)buff; (void)max; (void)fold; }

/* More exception codes */
#define COB_EC_DATA_PTR_NULL            0x0301
#define COB_EC_PROGRAM_ARG_OMITTED      0x0302
#define COB_EC_BOUND_SUBSCRIPT          0x0303
#define COB_EC_BOUND_ODO                0x0304
#define COB_EC_BOUND_PTR                0x0305
#define COB_EC_RANGE                    0x0400
#define COB_EC_PROGRAM_RESOURCES        0x0500
#define COB_EC_SIZE                     0x0600
#define COB_EC_SIZE_OVERFLOW            0x0601
#define COB_EC_SIZE_UNDERFLOW           0x0602
#define COB_EC_SIZE_ZERO_DIVIDE         0x0603

/* More constants */
#define COB_FILE_MAX                    1024
#define COB_STORE_KEEP_ON_OVERFLOW      1
#define COB_STORE_TRUNC_ON_OVERFLOW     2
#define COB_MAX_DEC_STRUCT              8
#define COB_ZEROES_ALPHABETIC           0
#define COB_SPACES_ALPHABETIC_BYTE_LENGTH 1
#define COB_ZEROES_ALPHABETIC_BYTE_LENGTH 1
#define COB_SCREEN_TYPE_GROUP           1
#define COB_SCREEN_TYPE_VALUE           2
#define COB_SCREEN_TYPE_FIELD           3
#define COB_SCREEN_TYPE_ATTRIBUTE       4
#define COB_SCREEN_NO_DISP              (1 << 0)
#define COB_READ_LOCK                   1
#define COB_UNUSED(x)                   (void)(x)
#define COB_S64_C(x)                    ((cob_s64_t)(x))


/* File I/O modes */
#define COB_READ_NO_LOCK            0
#define COB_READ_IGNORE_LOCK        2
#define COB_READ_WAIT_LOCK          3
#define COB_READ_KEPT_LOCK          4
#define COB_READ_ADVANCING_LOCK     5
#define COB_READ_PREVIOUS           1
#define COB_READ_NEXT               0
#define COB_WRITE_LOCK              1
#define COB_WRITE_NO_LOCK           0
#define COB_WRITE_AFTER             (1 << 0)
#define COB_WRITE_LINES             (1 << 1)
#define COB_WRITE_BEFORE            (1 << 2)
#define COB_WRITE_PAGE              (1 << 3)
#define COB_EC_RANGE_INDEX          0x0700


/* More I/O constants */
#define COB_WRITE_CHANNEL           (1 << 4)

/* Stub memory and validation functions */
static inline void *cob_malloc(size_t size) { return malloc(size); }
static inline void cob_free(void *ptr) { free(ptr); }
static inline int cob_is_valid_uri(const char *uri) { (void)uri; return 1; }
static inline int cob_is_xml_namestartchar(int c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == ':'; }
static inline int cob_is_xml_namechar(int c) { return cob_is_xml_namestartchar(c) || (c >= '0' && c <= '9') || c == '-' || c == '.'; }


/* Environment expansion stub */
static inline char *cob_expand_env_string(char *str) { return str; }


/* Literal size */
#define COB_MAX_LITERAL_LEN         8192


/* Exception all */
#define COB_EC_ALL                  0xFFFF


/* Environment setup stub */
static inline void cob_setup_env(const char *argv0) { (void)argv0; }


/* More size constants */
#define COB_MAX_BINARY          39
#define COB_MAX_NAMELEN         256


/* Additional constants */
#define COB_EC_MAX              256
#define COB_MEDIUM_BUFF         2048
#define COB_MEDIUM_MAX          (COB_MEDIUM_BUFF - 1)
#ifdef _WIN32
#define PATHSEP_CHAR            ';'
#else
#define PATHSEP_CHAR            ':'
#endif


/* Date conversion stub */
static inline int cob_set_date_from_epoch(struct cob_time *t, const void *epoch) { (void)t; (void)epoch; return 0; }


/* More runtime stubs */
static inline struct cob_time cob_get_current_date_and_time(void) { struct cob_time t = {0}; return t; }
static inline char *cob_getenv_direct(const char *name) { return getenv(name); }
static inline int cob_setenv(const char *name, const char *value, int overwrite) { (void)name; (void)value; (void)overwrite; return 0; }
static inline void cob_hard_failure_internal(const char *msg) { fprintf(stderr, "%s\n", msg); exit(1); }
#define GC_C_VERSION_PRF        "GnuCOBOL"
#define GC_C_VERSION            "3.2"


/* More utility stubs */
static inline char *cob_relocate_string(const char *str) { return (char*)str; }
static inline char *cob_getenv_value(const char *name) { return getenv(name); }


/* Compiler options and dump flags */
#define CB_COPT_0           "0"
#define CB_COPT_1           "1"
#define CB_COPT_2           "2"
#define CB_COPT_3           "3"
#define CB_COPT_S           "s"
#define COB_DUMP_NONE       0
#define COB_DUMP_FD         (1 << 0)
#define COB_DUMP_LS         (1 << 1)
#define COB_DUMP_LO         (1 << 2)
#define COB_DUMP_ALL        0xFF


/* Large buffer and signal handling */
#define COB_LARGE_BUFF      16384
#define COB_LARGE_MAX       (COB_LARGE_BUFF - 1)
static inline void cob_reg_sighnd(void (*handler)(int)) { (void)handler; }
static inline void cob_common_init(void *ptr) { (void)ptr; }


/* More dump flags */
#define COB_DUMP_WS         (1 << 3)
#define COB_DUMP_RD         (1 << 4)


/* Screen dump flag */
#define COB_DUMP_SD         (1 << 5)


/* More constants and stubs */
#define COB_DUMP_SC         (1 << 6)
#ifdef _WIN32
#define SLASH_STR           "\\"
#define PATHSEP_STR         ";"
#else
#define SLASH_STR           "/"
#define PATHSEP_STR         ":"
#endif
static inline const char *cob_get_sig_name(int sig) { (void)sig; return "UNKNOWN"; }
static inline void cob_incr_temp_iteration(void) { }
static inline int cob_load_collation(const char *name, void *data, void *type) { (void)name; (void)data; (void)type; return 0; }
static inline char *cob_temp_name(char *buff, const char *ext) { sprintf(buff, "/tmp/cobtmp%s", ext); return buff; }

/* Macro for ranges */
#define CB_XRANGE(min,max)  #min " - " #max

