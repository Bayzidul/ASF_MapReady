#include <ctype.h>

#include <asf.h>

char *uc (const char *string)
{
  static char out[1024];
  int ii;

  for (ii=0; ii<strlen(string); ii++)
    out[ii]=toupper(string[ii]);
  out[ii]='\0';

  return out;
}

int strcmp_case(const char *s1, const char *s2)
{
    const char *p1 = s1;
    const char *p2 = s2;

    while (toupper(*p1)==toupper(*p2++))
        if (*p1++ == '\0') return 0;
    return (toupper(*p1) - toupper(*--p2));
}

int strncmp_case(const char *s1, const char *s2, size_t n)
{
    const char *p1 = s1;
    const char *p2 = s2;
    int i=0;

    while (toupper(*p1)==toupper(*p2++)) {
        if (*p1++ == '\0') return 0;
        if (++i == n) return 0;
    }
    return (toupper(*p1) - toupper(*--p2));
}

char *appendStr(const char *s1, const char *s2)
{
    char *ret = MALLOC((strlen(s1)+strlen(s2)+1)*sizeof(char));
    strcat(strcpy(ret, s1), s2);
    return ret;
}

// copies not more than len-1 characters from the string 'src'
// (including any terminating null characters), to 'dst'
// If strlen(src)<len, null characters are appended to dst
// to make the total len.
// If strlen(src>=len, src is truncated to len-1 characters,
// and a null terminating character is appended to dst.
// Differs from strncpy in that:
//  - only copies len-1 characters, instead of len.
//  - dst is guaranteed null-terminated
char *strncpy_safe(char *dst, const char *src, size_t len)
{
    strncpy(dst, src, len-1);
    dst[len-1] = '\0';
    return dst;
}

char *trim_spaces(const char *s)
{
  char *p1, *p2;

  p1 = (char *) MALLOC(sizeof(char)*strlen(s));
  strcpy(p1, s);
  strtok(p1, " ");
  p2 = strchr(p1, ' ');
  if (p2) {
    while (strncmp(p2, " ", 1) == 0)
      p2++;
  }
  else {
    p2 = (char *) MALLOC(sizeof(char)*strlen(s));
    strcpy(p2, p1);
  }
  FREE(p1);
  return p2;
}
