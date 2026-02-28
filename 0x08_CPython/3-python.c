#include <Python.h>



void print_python_bytes(PyObject *p);
void print_python_float(PyObject *p);


/**
 * print_python_list - func
 * @p: PyObject
 */
void print_python_list(PyObject *p)
{
	long int size = 0, i = 0;
	PyObject *ob = NULL;
	PyVarObject *var_ob = NULL;
	PyListObject *list = NULL;

	printf("[*] Python list info\n");
	if (!PyList_Check(p))
	{
		printf("  [ERROR] Invalid List Object\n");
		return;
	}
	list = (PyListObject *)p;
	var_ob = &list->ob_base;
	ob = &var_ob->ob_base;
	size = var_ob->ob_size;
	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", list->allocated);
	for (; i < size; i++)
	{
		ob = list->ob_item[i];
		printf("Element %ld: %s\n", i, ob->ob_type->tp_name);
		if (PyBytes_Check(ob))
			print_python_bytes(ob);
		if (PyFloat_Check(ob))
			print_python_float(ob);
	}
}


/**
 * print_python_bytes - func
 * @p: PyObject
 */
void print_python_bytes(PyObject *p)
{
	long int size = 0, aux_size = 0, i = 0;
	PyVarObject *var_ob = NULL;
	PyBytesObject *bytes = NULL;

	printf("[.] bytes object info\n");
	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}
	bytes = (PyBytesObject *)p;
	var_ob = &bytes->ob_base;
	size = var_ob->ob_size;
	aux_size = (size < 10) ? (size + 1) : 10;
	printf("  size: %ld\n", size);
	printf("  trying string: %s", bytes->ob_sval);
	printf("\n");
	printf("  first %ld bytes:", aux_size);
	fflush(NULL);
	for (; i < aux_size; i++)
		printf(" %02x", (unsigned char)bytes->ob_sval[i]);
	printf("\n");
}


/**
 * print_python_float - func
 * @p: PyObject
 */
void print_python_float(PyObject *p)
{
	int i = 0;
	char buff[32];
	PyFloatObject *fob = NULL;

	printf("[.] float object info\n");
	if (!PyFloat_Check(p))
	{
		printf("  [ERROR] Invalid Float Object\n");
		return;
	}
	memset(buff, 0, sizeof(buff));
	fob = (PyFloatObject *)p;
	sprintf(buff, "%.16g", fob->ob_fval);
	while ((buff[i] != '.') && (buff[i]))
		i++;
	if (!buff[i])
	{
		buff[i] = '.';
		buff[i + 1] = '0';
	}
	printf("  value: %s\n", buff);
}
