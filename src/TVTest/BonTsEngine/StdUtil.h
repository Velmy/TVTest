#ifndef STD_UTIL_H
#define STD_UTIL_H


namespace StdUtil {

int snprintf(char *s,size_t n,const char *format, ...);
int snprintf(wchar_t *s,size_t n,const wchar_t *format, ...);
inline int strlen(const char *s) { return ::strlen(s); }
inline int strlen(const wchar_t *s) { return ::wcslen(s); }
inline char *strcpy(char *dest,const char *src) { return ::strcpy(dest,src); }
inline wchar_t *strcpy(wchar_t *dest,const wchar_t *src) { return ::wcscpy(dest,src); }
char *strncpy(char *dest,size_t n,const char *src);
wchar_t *strncpy(wchar_t *dest,size_t n,const wchar_t *src);
char *strdup(const char *s);
wchar_t *strdup(const wchar_t *s);

}


#endif
