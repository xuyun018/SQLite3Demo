#include "utf8.h"
//---------------------------------------------------------------------------
unsigned int utf8_encode(unsigned char *buffer, unsigned int buffersize, const wchar_t *characters, unsigned int *characterslength)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned short wch;

	while ((wch = characters[j]) != L'\0')
	{
		if (wch < 0x80)
		{
			if (i + 1 < buffersize)
			{
				buffer[i++] = (unsigned char)wch;
			}
			else
			{
				i++;
			}
		}
		else
		{
			if (wch < 0x800)
			{ 
				if (i + 2 < buffersize)
				{
					buffer[i++] = 0xc0 | ((wch >> 6) & 0x1f);
					buffer[i++] = 0x80 | (wch & 0x3f);
				}
				else
				{
					i += 2;
				}
			}
			else
			{
				if (i + 3 < buffersize)
				{
					buffer[i++] = 0xe0 | ((wch >> 12) & 0x0f);
					buffer[i++] = 0x80 | ((wch >> 6) & 0x3f);
					buffer[i++] = 0x80 | (wch & 0x3f);
				}
				else
				{
					i += 3;
				}
			}
		}

		j++;
	}
	if (characterslength)
	{
		*characterslength = j;
	}
	if (i < buffersize)
	{
		buffer[i++] = 0;
	}
	else
	{
		i++;
	}
	return(i);
}

// 长度包括了结束符
unsigned int utf8_decode(wchar_t *characters, unsigned int *characterslength, unsigned int characterssize, const unsigned char *buffer, unsigned int buffersize)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned short wch;

	do
	{
		if (buffer[i] < 0x80)
		{
			wch = buffer[i++];
		}
		else
		{
			if (buffer[i] < 0xe0)
			{
				wch = buffer[i++] & 0x1f;
				wch <<= 6;
				wch |= buffer[i++] & 0x3f;
			}
			else
			{
				wch = buffer[i++] & 0x0f;
				wch <<= 6;
				wch |= buffer[i++] & 0x3f;
				wch <<= 6;
				wch |= buffer[i++] & 0x3f;
			}
		}
		if (j < characterssize)
		{
			characters[j] = wch;
		}
		j++;
	} while (wch != 0 && (buffersize == 0 || i < buffersize));
	if (characterslength)
	{
		*characterslength = j;
	}
	return(i);
}
//---------------------------------------------------------------------------