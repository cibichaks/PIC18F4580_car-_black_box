#ifndef MAIN
#define MAIN
unsigned char key;
void init_config(void);
void here_is_menu(void);
void here_is_dashboard(void);
static void print_main_menu(unsigned char );
void here_is_view_log(void);
void here_is_Download_log(void);
static void Store_impact(unsigned char*,unsigned char,unsigned short);
static void delete_impact(void);
static void display_time(void);
void fetch_log();
static void modify_for_send(void);
void here_is_set_time(void);
#endif
