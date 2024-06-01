#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_LEN 2048
/* Generator directives */
#define GD_PRE_BEGIN    "gd::pre"
#define GD_PRE_END      "gd::endpre"
#define GD_H1           "gd::h1"
#define GD_H2           "gd::h2"

void print_prologue(void);
void print_epilogue(void);

/* Compile: cl htmlgen.c /utf-8 */
/* Usage: htmlgen < input.txt > output.html */
int main(void) {
   char *line;

   line = malloc(LINE_LEN);
   if (line == NULL) {
      fprintf(stderr, "failed to malloc for line.");
      exit(EXIT_FAILURE);
   }

   int tag_count;
   bool in_line;
   bool in_pre = false;
   int tag_kind = 0;    /* bit-field */
   
   print_prologue();
   tag_count = 0;
   while (fgets(line, LINE_LEN, stdin) != NULL) {
      /* In case of an empty line */
      if (strncmp(line, "\n", 2) == 0)
         continue;
      
      /* Open <div> */
      if (tag_count == 0) {
         if (strncmp(line, GD_PRE_BEGIN, strlen(GD_PRE_BEGIN)) == 0) {
            puts("      <div class=\"code-block\">");
            puts("         <pre>\n");
            in_pre = true;
            continue;
         }
         else if (strncmp(line, GD_H1, strlen(GD_H1)) == 0) {
            tag_kind |= 1 << 0;
            continue;
         }
         else if (strncmp(line, GD_H2, strlen(GD_H2)) == 0) {
            tag_kind |= 1 << 1;
            continue;
         }
         
         if (!in_pre)
            puts("      <div class=\"plain-text\">");
      }
      
      /* Check if the line is not finished. */
      if (strlen(line) == LINE_LEN - 1
            && line[LINE_LEN - 2] != '\n')
      {
         in_line = true;
      }
      else {
         char *last_char_ptr;

         last_char_ptr = &line[strlen(line) - 1];
         if (*last_char_ptr == '\n')
            *last_char_ptr = '\0';
         in_line = false;
      }

      /* Generate tags */
      if (!in_pre) {
         char tag_name[3];

         if (tag_kind & 1 << 0) strncpy_s(tag_name, 3, "h1", 2);
         else if (tag_kind & 1 << 1) strncpy_s(tag_name, 3, "h2", 2);
         else strncpy_s(tag_name, 3, "p", 1);

         if (!in_line)
            printf("         <%s>", tag_name);
         fputs(line, stdout);
         if (!in_line) {
            printf("</%s>\n", tag_name);
            tag_count++;
         }
      }
      else {
         if (strncmp(line, GD_PRE_END, strlen(GD_PRE_END)) == 0) {
            puts("         </pre>");
            puts("         <p></p>");
            tag_count += 2;
            in_pre = false;
         }
         else {
            if (in_line)
               fputs(line, stdout);
            else
               puts(line);
         }
      }

      /* Close <div> */
      if (tag_count == 2) {
         puts("      </div>");
         tag_count = 0;
         tag_kind = 0;
      }
   }
   if (ferror(stdin)) {
      fprintf(stderr, "error occurred while fgets.");
      exit(EXIT_FAILURE);
   }
   print_epilogue();
}

void print_prologue(void) {
   puts("<!DOCTYPE html>");
   puts("<html lang=\"ko-KR\">");
   puts("   <head>");
   puts("      <meta charset=\"utf-8\" />");
   puts("      <meta name=\"viewport\" content=\"width=device-width\" />");
   puts("      <link href=\"../style.css\" rel=\"stylesheet\" />");
   puts("      <title>TITLE</title>");
   puts("   </head>");
   puts("   <body>");
   puts("      <div id=\"navigator\">");
   puts("         <h2>목 차</h2>");
   puts("         <ul></ul>");
   puts("      </div>");
   puts("      <p id=\"memo\">");
   puts("         <b>메모</b>:&emsp;개인적인 공부용으로 번역하였습니다.");
   puts("         <br>영어 실력 및 각종 이해력 등의 이슈로 번역이 부정확할 수 있으리라 생각합니다.");
   puts("         <br>이에 편의를 위해 원문을 옆에 비치하였사오니 해석이 좀 이?상하다 싶으면 참고하시기 바랍니다. 감사합니다.");
   puts("         <br><br><b>원 글 주소</b>: <a href=\"LINK\">LINK</a>");
   puts("      </p>");
}

void print_epilogue(void) {
   puts("      <div id=\"dummy\"></div>");
   puts("   </body>");
   puts("   <script src=\"../navigator.js\"></script>");
   puts("   <script>constructTableOfContents(\"LINK\");</script>");
   puts("</html>");
}