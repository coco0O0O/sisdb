#include <os_str.h>

int sis_strcpy(char *out_, size_t olen_, const char *in_)
{
	if (!in_ || !out_)
	{
		return 0;
	}
	size_t len = strlen(in_);
	olen_--; //��ֹ�ַ������һλ�����Ǳ߽�
	len = len > olen_ ? olen_ : len;
	memmove(out_, in_, len);
	// ��ʱ��ע�͵�������olen���ȴ�������󣩣�����outԽ��
	// memset(out_ + len, 0, olen_ - len + 1);
	out_[len] = 0 ;
	return (int)len;
}

int sis_strncpy(char *out_, size_t olen_, const char *in_, size_t ilen_)
{
	if (!in_ || !out_)
	{
		return 0;
	}

	size_t len = strlen(in_);
	len = len > ilen_ ? ilen_ : len;
	olen_--; //��ֹ�ַ������һλ�����Ǳ߽�
	len = len > olen_ ? olen_ : len;
	memmove(out_, in_, len);
	// ��ʱ��ע�͵�������olen���ȴ�������󣩣�����outԽ��
	// memset(out_ + len, 0, olen_ - len + 1);
	out_[len] = 0;
	return (int)len;
}

void sis_trim(char *s)
{
	int i, len;
	len = (int)strlen(s);
	for (i = len - 1; i >= 0; i--)
	{
		// if (s[i] != ' ' && s[i] != 0x0d && s[i] != 0x0a)
		if (s[i] && (unsigned char)s[i] > ' ')
		{
			break;
		}
		else
		{
			s[i] = 0;
		}
	}
	for (i = 0; i < len; i++)
	{
		if (s[i] && (unsigned char)s[i] > ' ')
		// if (s[i] != ' ')
		{
			break;
		}
	}
	if (i != 0)
	{
		memmove(s, s + i, len - i);
		s[len - i] = 0;
	}
}