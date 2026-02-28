#include <Python.h>



void print_python_bytes(PyObject *p);


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

	if (!PyList_Check(p))
		return;
	list = (PyListObject *)p;
	var_ob = &list->ob_base;
	ob = &var_ob->ob_base;
	size = var_ob->ob_size;
	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", list->allocated);
	for (; i < size; i++)
	{
		ob = list->ob_item[i];
		printf("Element %ld: %s\n", i, ob->ob_type->tp_name);
		if (PyBytes_Check(ob))
			print_python_bytes(ob);
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
