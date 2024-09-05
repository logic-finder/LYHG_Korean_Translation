#include <stdio.h>
#include <string.h>

// Compile
#define UNIX
// (= WIN32) at Developer Command Prompt, run "cl htmlgen.c /utf-8".
// (= UNIX) "make" or "gcc -o htmlgen htmlgen.c"
#include "htmlgen.h"

const char *error_msg = "htmlgen: \033[1;31mprocessing halted!\033[0m\n\033[4mREASON\033[0m";

int main(int argc, const char *argv[]) {
   if (argc != 4) raise_err(
      "htmlgen: usage: htmlgen input.txt OUTPUT_PATH FILE_NAME.html");

   char *dest_path;
   const int argv2_len = strlen(argv[2]);
   const int argv3_len = strlen(argv[3]);
   const int dest_path_len = argv2_len + 1 + argv3_len + 1;
   
   dest_path = malloc(dest_path_len);
   if (dest_path == NULL)
      raise_err("htmlgen: failed to malloc.");
   if (snprintf(dest_path, dest_path_len, "%s/%s", argv[2], argv[3]) < 0)
      raise_err("htmlgen: failed to snprintf.");

   FILE *src, *dest;

   src  = open_file(argv[1], "r");
   dest = open_file(dest_path, "w");
   free(dest_path);

   print_prologue(dest);
   print_contents(src, dest);
   print_epilogue(dest, argv[3]);

   fclose(src);
   fclose(dest);
   atexit(notify_done);
   return 0;
}

void raise_err(const char *err_msg, ...) {
   va_list ap;
   
   va_start(ap, err_msg);
   vfprintf(stderr, err_msg, ap);
   va_end(ap);
   fprintf(stderr, "\n");
   exit(EXIT_FAILURE);
}

FILE *open_file(const char *filename, const char *mode) {
   const int filename_len = strlen(filename);
   char *path;
   const int path_len = 2 + filename_len + 1;

   path = malloc(path_len);
   if (path == NULL)
      raise_err("htmlgen: failed to malloc.");
   if (snprintf(path, path_len, "./%s", filename) < 0)
      raise_err("htmlgen: failed to snprintf.");

   FILE *fp;
   
   fp = fopen(path, mode);
   if (fp == NULL)
      raise_err("htmlgen: couldn't open %s.", path);
   free(path);
   
   return fp;
}

void print_prologue(FILE *dest) {
   // Note: 1650 characters long
   // the maximum length of a string literal = at least 509 (C89) / 4095 (C99) characters long
   if (fputs(
         "<!DOCTYPE html>\n"
         "<html lang=\"ko-KR\">\n"
         "   <head>\n"
         "      <meta charset=\"utf-8\" />\n"
         "      <meta name=\"viewport\" content=\"width=device-width\" />\n"
         "      <link href=\"../src/style.css\" rel=\"stylesheet\" />\n"
         "      <title></title>\n"
         "   </head>\n"
         "   <body>\n"
         "      <div id=\"navigator\">\n"
         "         <div id=\"book-info\">\n"
         "            <p id=\"book-title\">\n"
         "               <span id=\"n1\">Learn You a</span> <span id=\"n2\">Haskell</span> <span id=\"n3\">for Great Good!</span>\n"
         "            </p>\n"
         "            <p id=\"book-subtitle\">A Beginner's Guide</p>\n"
         "            <p id=\"author\">Miran Lipovača&emsp;原著</p>\n"
         "            <p id=\"translator\"><a href=\"https://github.com/logic-finder\" target=\"_blank\">logicseeker</a>&emsp;&emsp;譯</p>\n"
         "         </div>\n"
         "         <hr>\n"
         "         <h2>목 차</h2>\n"
         "         <hr>\n"
         "         <ul id=\"nav-misc\"></ul>\n"
         "         <hr>\n"
         "         <ol id=\"nav-intro\"></ol>\n"
         "         <hr>\n"
         "         <div id=\"nav-main\"></div>\n"
         "         <p id=\"memo\">\n"
         "            <b>메모</b>:&emsp;개인적인 공부용으로 번역하였습니다.\n"
         "            <br>영어 실력 및 각종 이해력 등의 이슈로 번역이 부정확할 수 있으리라 생각합니다.\n"
         "            <br>이에 편의를 위해 원문을 옆에 비치하였사오니 해석이 좀 이?상하다 싶으시면 참고하시기 바랍니다. 감사합니다.\n"
         "         </p>\n"
         "         <img src=\"../src/Patchouli_Holding_Learn_You_a_Haskell.jpg\" title=\"UwU\" alt=\"a cute character holding the book Learn You a Haskell for Great Good!\">\n"
         "         <p id=\"img-source\">Source: https://github.com/cat-milk/Anime-Girls-Holding-Programming-Books/blob/master/Haskell/Patchouli_Holding_Learn_You_a_Haskell.jpg</p>"
         "      </div>\n"
         "      <article>\n",
         dest) == EOF)
      raise_err("htmlgen: failed to fputs.");
}

void print_contents(FILE *src, FILE *dest) {
   prcs_ln_params params;
   int line_count;

   params.at_div_begin = true;
   line_count = 0;
   while (!feof(src)) {
      char *line;

      line = get_line(src);
      if (line == NULL)
         break;
      if (params.at_div_begin)
         handle_div_begin(dest, line, ++line_count, &params);
      else
         handle_div_inside(dest, line, ++line_count, &params);
      free(line);
   }
   if (!params.at_div_begin)
      raise_err("%s reached to eof but div is not closed.", error_msg);
}

void print_epilogue(FILE *dest, const char *title_name) {
   // Note: 136 characters long
   if (fprintf(dest,
         "      </article>\n"
         "   </body>\n"
         "   <script src=\"../src/navigator.js\"></script>\n"
         "   <script>\n"
         "      tailorDocument(\"%s\");\n"
         "   </script>\n"
         "</html>",
         title_name) < 0)
      raise_err("htmlgen: failed to fprintf.");
}

char *get_line(FILE *src) {
   char *line;
   int line_cur_idx, line_max_len;

   line_max_len = LINE_READ_UNIT;
   line = malloc(line_max_len);
   if (line == NULL)
      raise_err("htmlgen: failed to malloc.");
   line_cur_idx = 0;

   while (fgets(line + line_cur_idx, LINE_READ_UNIT, src) != NULL) {
      const int  line_len    = strlen(line);
      const bool end_of_line = line[line_len - 1] == '\n';
      const bool end_of_file = feof(src) ? true : false;
      const bool line_ended  = end_of_line || end_of_file;

      if (line_ended) {
         line[strcspn(line, NEWLINE_MARK)] = '\0';
         return line; // tried fgets and read something
      }
      else {
         line_max_len += LINE_READ_UNIT - 1;
         line = realloc(line, line_max_len);
         if (line == NULL)
            raise_err("htmlgen: failed to realloc.");
         line_cur_idx += LINE_READ_UNIT - 1; // so as to overwrite the trailing \0.
      }
   }
   if (ferror(src))
      raise_err("htmlgen: error during fgets.");
   
   // tried fgets but met EOF immediately
   free(line);
   return NULL;
}

void handle_div_begin(FILE *dest, const char *line, int line_count,
                      prcs_ln_params *pptr) {
   if (strlen(line) == 0)
      return;
   if (strcmp(line, GD_PRE_BEGIN) == 0) {
      open_codeblock_div(dest);
      pptr->in_pre = true;
   }
   else {
      const int gd_len = strlen(GD);

      open_plaintext_div(dest);
      pptr->tag_count = 0;
      if (strcmp(line, GD_H1) == 0)
         pptr->tag_name = "h1";
      else if (strcmp(line, GD_H2) == 0)
         pptr->tag_name = "h2";
      else if (strncmp(line, GD, gd_len) == 0)
         raise_err("%s unknown generator directive '%s' at line %d.",
            error_msg, line + gd_len, line_count);
      else {
         pptr->tag_name = "p";
         put_plain_line(dest, pptr->tag_name, line, &pptr->tag_count);
         put_partition(dest);
      }
      pptr->in_pre = false;
   }
   pptr->at_div_begin = false;
}

void handle_div_inside(FILE *dest, const char *line, int line_count,
                       prcs_ln_params *pptr) {
   const int gd_len = strlen(GD);
   
   if (pptr->in_pre) {
      if (strcmp(line, GD_PRE_END) == 0) {
         close_codeblock_div(dest);
         pptr->at_div_begin = true;
      }
      else if (strncmp(line, GD, gd_len) == 0)
         raise_err("%s expected either a normal line or an endpre directive "
            "but encountered a generator directive '%s' at line %d.",
            error_msg, line + gd_len, line_count);
      else if (strlen(line) == 0)
         put_code_line(dest, "");
      else
         put_code_line(dest, line);
   }
   else {
      if (strlen(line) == 0)
         return;
      if (strncmp(line, GD, gd_len) == 0)
         raise_err("%s expected a normal line but encountered a generator "
            "directive '%s' at line %d.",
            error_msg, line + gd_len, line_count);
      put_plain_line(dest, pptr->tag_name, line, &pptr->tag_count);
      if (pptr->tag_count == 1)
         put_partition(dest);
      else if (pptr->tag_count == 2) {
         close_plaintext_div(dest);
         pptr->at_div_begin = true;
      }
   }
}

void open_plaintext_div(FILE *dest) {
   if (fputs(
         "         <div class=\"plain-text\">\n",
         dest) == EOF)
      raise_err("htmlgen: failed to fputs.");
}

void open_codeblock_div(FILE *dest) {
   if (fputs(
         "         <div class=\"code-block\">\n"
         "            <pre>",
         dest) == EOF)
      raise_err("htmlgen: failed to fputs.");
}

void put_plain_line(
   FILE *dest,
   const char *tag_name, const char *line, int *tag_count_ptr
) {
   if (fprintf(dest,
         "            <%s>%s</%s>\n",
         tag_name, line, tag_name) < 0)
      raise_err("htmlgen: failed to fprintf.");
   (*tag_count_ptr)++;
}

void put_partition(FILE *dest) {
   if (fputs(
         "            <div class=\"partition\"></div>\n",
         dest) == EOF)
      raise_err("htmlgen: failed to fpus.");
}

void put_code_line(FILE *dest, const char *line) {
   if (fprintf(dest, "\n%s", line) < 0)
      raise_err("htmlgen: failed to fprintf.");
}

void close_plaintext_div(FILE *dest) {
   if (fputs("         </div>\n", dest) == EOF)
      raise_err("htmlgen: failed to fputs.");
}

void close_codeblock_div(FILE *dest) {
   if (fputs(
         "</pre>\n"
         "         </div>\n",
         dest) == EOF)
      raise_err("htmlgen: failed to fputs.");
}

void notify_done(void) {
   puts("htmlgen: done!");
}