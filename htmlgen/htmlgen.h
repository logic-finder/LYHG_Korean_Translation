#ifndef HTMLGEN_H
#define HTMLGEN_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#if defined(WIN32)
#define NEWLINE_MARK "\r\n"
#elif defined(UNIX)
#define NEWLINE_MARK "\n"
#else
#error unsupported OS!
#endif

#define LINE_READ_UNIT 2048
/* generator directives */
#define GD_PRE_BEGIN    "gd::pre"
#define GD_PRE_END      "gd::endpre"
#define GD_H1           "gd::h1"
#define GD_H2           "gd::h2"

typedef struct process_line_params_ {
   bool  at_div_begin;
   bool  in_pre;
   int   tag_count;
   char *tag_name;
} prcs_ln_params;

/**
 * @brief prints an error message to stderr.
 * 
 * @param err_msg an error message to be printed
 * @param ... variable argument list
 */
void raise_err(const char *err_msg, ...);

/**
 * @brief opens a file with a specified mode.
 * 
 * @param filename the name of a file to be opened
 * @param mode the second argument of fopen()
 */
FILE *open_file(const char *filename, const char *mode);

/**
 * @brief prints the upper part of the whole html document.
 * 
 * @param dest a destination file to write
 */
void print_prologue(FILE *dest);

/**
 * @brief prints the actual contents of the html document.
 * 
 * @param src a source file to get lines
 * @param dest a destination file to write
 */
void print_contents(FILE *src, FILE *dest);

/**
 * @brief prints the lower part of the whole html document.
 * 
 * @param dest a destination file to write
 * @param title_name the name of a file
 */
void print_epilogue(FILE *dest, const char *title_name);

/**
 * @brief `malloc`s a new string, acquires a line from stdin, saves it to the string, and returns the string.
 * 
 * @param src a source file to get a line
 * @return a line from a source file
 * 
 * @details The return value of this function must be `free`d.
 */
char *get_line(FILE *src);

/**
 * @brief processes a line. If the line is a generator directive, this function performs relevant actions. If the line is a normal line, then this function prints it.
 * 
 * @param dest a destination file to write
 * @param line a line acquired from get_line()
 * @param params_ptr a pointer to a struct which contains the necessary information to run
 */
void process_line(FILE *dest, const char *line, prcs_ln_params *pptr);

/**
 * @brief prints the opening part of the plain-text div.
 * 
 * @param dest a destination file to write
 */
void open_plaintext_div(FILE *dest);

/**
 * @brief prints the opening part of the code-block div.
 * 
 * @param dest a destination file to write
 */
void open_codeblock_div(FILE *dest);

/**
 * @brief prints <tag>line</tag>.
 * 
 * @param dest a destination file to write
 * @param tag_name a tag name to be used
 * @param line a line to be written
 * @param tag_count_ptr a pointer to an integer variable which keeps count of how many plain lines have been currently written
 */
void put_plain_line(
   FILE *dest,
   const char *tag_name, const char *line, int *tag_count_ptr);

/**
 * @brief prints <div class="partition"></div>.
 *
 * @param dest a destination file to write
 */
void put_partition(FILE *dest);

/**
 * @brief prints a line itself.
 * 
 * @param dest a destination file to write
 * @param line a line to be written
 */
void put_code_line(FILE *dest, const char *line);

/**
 * @brief prints the closing part of the plain-text div.
 * 
 * @param dest a destination file to write
 */
void close_plaintext_div(FILE *dest);

/**
 * @brief prints the closing part of the code-block div.
 * 
 * @param dest a destination file to write
 */
void close_codeblock_div(FILE *dest);

/**
 * @brief notifies that the program has been finished successfully.
 */
void notify_done(void);

#endif