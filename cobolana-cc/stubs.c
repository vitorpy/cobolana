#include "cobc.h"
#include "tree.h"

/* Stub functions for missing symbols */
struct cb_field *cb_code_field(cb_tree x) { (void)x; return NULL; }
void cb_parse_xfd(struct cb_file *file, struct cb_field *field) { (void)file; (void)field; }
struct cb_field *get_sum_data_field(struct cb_report *r, struct cb_field *f) { 
    (void)r; (void)f; return NULL; 
}
unsigned int chk_field_variable_address(struct cb_field *fld) { (void)fld; return 0; }
int cb_lookup_literal(struct cb_program *prog, cb_tree x, int make_decimal) { 
    (void)prog; (void)x; (void)make_decimal; return 0; 
}

/* Stub variables */
int cb_flag_memory_check = 0;

/* More stubs */
void cb_save_xfd(char *name) { (void)name; }

/* getopt stub */
int cob_getopt_long_long(int argc, char *const *argv, const char *optstring,
                         const void *longopts, int *longindex, int long_only) {
    (void)argc; (void)argv; (void)optstring; (void)longopts; (void)longindex; (void)long_only;
    return -1;
}

/* More missing functions */
struct cb_field *chk_field_variable_size(struct cb_field *fld) { (void)fld; return NULL; }
const char *cb_open_mode_to_string(const enum cob_open_mode mode) { (void)mode; return "UNKNOWN"; }
int gentable(FILE *f, const char *name, const char *type, char mode) { 
    (void)f; (void)name; (void)type; (void)mode; return 0; 
}
