#ifndef UTF8_H
#define UTF8_H
//---------------------------------------------------------------------------
#include <Windows.h>
//---------------------------------------------------------------------------
unsigned int utf8_encode(unsigned char *buffer, unsigned int buffersize, const wchar_t *characters, unsigned int *characterslength);
// 返回的长度包括了结束符
unsigned int utf8_decode(wchar_t *characters, unsigned int *characterslength, unsigned int characterssize, const unsigned char *buffer, unsigned int buffersize);
//---------------------------------------------------------------------------
#endif
