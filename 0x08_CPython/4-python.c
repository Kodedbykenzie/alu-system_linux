#include <Python.h>



void ucs_to_utf8(void *pos, int kind, Py_ssize_t length);


/**
 * print_python_string - func
 * @p: PyObject
 */
void print_python_string(PyObject *p)
{
	char	*ascii = "compact ascii",
			*ucode = "compact unicode object";
	PyASCIIObject *str_ob = NULL;

	setbuf(stdout, NULL);
	printf("[.] string object info\n");
	if (!PyUnicode_Check(p))
	{
		printf("  [ERROR] Invalid String Object\n");
		return;
	}
	str_ob = (PyASCIIObject *)p;
	printf("  type: %s\n", ((str_ob->state.ascii) ? ascii : ucode));
	printf("  length: %lu\n", str_ob->length);
	printf("  value: %s\n", PyUnicode_AsUTF8(p));
}


/**
 * print_python_string - func
 * @p: PyObject
 */
/*
void print_python_string(PyObject *p)
{
	char	*ascii = "compact ascii",
			*ucode = "compact unicode object",
			*pos = NULL;
	PyASCIIObject *ascii_ob = NULL;
	PyCompactUnicodeObject *uni_ob = NULL;

	printf("[.] string object info\n");
	if (!PyUnicode_Check(p))
	{
		printf("  [ERROR] Invalid String Object\n");
		return;
	}
	uni_ob = (PyCompactUnicodeObject *)p;
	ascii_ob = (PyASCIIObject *)&uni_ob->_base;
	if (ascii_ob->state.ascii)
		pos = (char *)&ascii_ob->state + sizeof(ascii_ob->state) + 4;
	else
		pos = (char *)&uni_ob->utf8;
	printf("  type: %s\n", ((ascii_ob->state.ascii) ? ascii : ucode));
	printf("  length: %ld\n", ascii_ob->length);
	if (ascii_ob->state.ascii)
		printf("  value: %s\n", pos);
	else
	{
		printf("  value: ");
		fflush(NULL);
		ucs_to_utf8(pos, ascii_ob->state.kind, ascii_ob->length);
		fflush(NULL);
		printf("\n");
	}
}
*/


/**
 * ucs_to_utf8 - func
 * @pos: pointer to data
 * @kind: kind of ucs
 * @length:	data length
 */
void ucs_to_utf8(void *pos, int kind, Py_ssize_t length)
{
	Py_UCS1 *ucs1 = NULL;
	Py_UCS2 *ucs2 = NULL;
	Py_UCS4 *ucs4 = NULL;
	unsigned char c = 0;
	unsigned int i = 0;

	if (kind == PyUnicode_1BYTE_KIND)
	{
		ucs1 = (Py_UCS1 *)pos;
		for (int j = 0; j < length; j++)
		{
			c = ucs1[j];
			if (c <= 0x7f)
				printf("%c", c);
			else
				printf("%c%c", 0xc0 | (c >> 6), 0x80 | (c & 0x3f));
		}
	}
	if (kind == PyUnicode_2BYTE_KIND)
	{
		ucs2 = (Py_UCS2 *)pos;
		for (Py_ssize_t j = 0; j < length; j++)
		{
			i = ucs2[j];
			if (i <= 0x7f)
				putchar(i);
			else if (i <= 0x7ff)
				printf("%c%c", 0xc0 | (i >> 6), 0x80 | (i & 0x3f));
			else
				printf("%c%c%c",
				0xe0 | (i >> 12), 0x80 | ((i >> 6) & 0x3f), 0x80 | (i & 0x3f));
		}
	}
	else if (kind == PyUnicode_4BYTE_KIND)
	{
		ucs4 = (Py_UCS4 *)pos;
		for (Py_ssize_t j = 0; j < length; j++)
		{
			i = ucs4[j];
			if (i <= 0x7f)
				putchar(i);
			else if (i <= 0x7ff)
				printf("%c%c", 0xc0 | (i >> 6), 0x80 | (i & 0x3f));
			else if (i <= 0xffff)
				printf("%c%c%c",
				0xe0 | (i >> 12), 0x80 | ((i >> 6) & 0x3f), 0x80 | (i & 0x3f));
			else
				printf("%c%c%c%c", 0xf0 | (i >> 18), 0x80 | ((i >> 12) & 0x3f),
				0x80 | ((i >> 6) & 0x3f), 0x80 | (i & 0x3f));
		}
	}
}
