#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char *stream;

void append(char* s, char c) {
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}

void append_str(char *s, char *c) {
  int len_s = strlen(s);
  int len_c = strlen(s);
  size_t i = 0;
  for(; i <len_c; ++i)
  {
    s[len_s + i] = c[i];
  }
  s[len_s + i + 1] = '\0';
}

void parse_link(char *name, char *link)
{
  while(*stream != ']')
  {
    append(name, *stream);
    *stream++;
  }
  *stream++;
  if(*stream == '(')
  {
    stream++;
    while(*stream != ')')
    {
      append(link, *stream);
      *stream++;
    }
  }
  else
  {
    fprintf(stderr, "Link is Invaild");
  }
}

void parse_header()
{
  ++stream;
  char line[256] = "\0";
  int count = 1;
  while(*stream && *stream == '#')
  {
    ++count;
    if(count > 5)
    {
      break;
    }
    ++stream;
  }

  switch(count)
  {
    case 1:
    {
      printf("<h1>");
    } break;
    case 2:
    {
      printf("<h2>");
    } break;
    case 3:
    {
      printf("<h3>");
    } break;
    case 4:
    {
      printf("<h4>");
    } break;
    case 5:
    {
      printf("<h5>");
    } break;
  }
  while(*stream && *stream != '\n')
  {
    if(*stream == '[')
    {
      stream++;
      char name[256] = "\0";
      char link[256] = "\0";
      parse_link(name, link);
      stream++;
      append_str(line, "<a ");
      append_str(line, "href=\"");
      append_str(line, link);
      append_str(line, "\">");
      append_str(line, name);
      append_str(line, "</a>");
    }
    append(line, *stream);
    ++stream;
  }

  switch(count)
  {
    case 1:
    {
      printf("%s</h1>", line);
    } break;
    case 2:
    {
      printf("%s</h2>", line);
    } break;
    case 3:
    {
      printf("%s</h2>", line);
    } break;
    case 4:
    {
      printf("%s</h4>", line);
    } break;
    case 5:
    {
      printf("%s</h5", line);
    } break;
  }
}

void parse_paragraph()
{
  printf("<p>");
  *stream++;
  char line[256] = "\0";
  int isBlank = 0;
  while(isalpha(*stream) || isspace(*stream) || *stream == '[')
  {
    if(isblank(*stream) && isBlank > 0 || *stream == '\n' && isblank > 0)
    {
    ++isBlank;
    break;
    }
    if(*stream == '[')
    {
      *stream++;
      char name[256] = "\0";
      char link[256] = "\0";
      parse_link(name, link);
      ++stream;
      append_str(line, "<a ");
      append_str(line, "href=\"");
      append_str(line, link);
      append_str(line, "\">");
      append_str(line, name);
      append_str(line, "</a>");
    }
  append(line, *stream);
  ++stream;
  }
  printf("%s</p>", line);      
}

int main()
{
  const char *test = "# I am a heading [new](https://stuff.com) \n\n This is some text\nStuff\n\nThis should be [new](https://stuff/com)\n - Things \n - other things";
  stream = test;

  while(*stream)
  {
    switch(*stream)
    {
      case '#':
      {
        parse_header();
      }
      case '\n':
      {
        *stream++;
        if(*stream != '\n')
        {
          break;
        }
        parse_paragraph();
      } break;
      /*
      TODO: Add links
      case '-': 
      */
      default:
      {
        *stream++;
      }
    }
  }
  return 0;
}