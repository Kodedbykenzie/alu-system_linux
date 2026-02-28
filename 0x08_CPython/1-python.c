#include <Python.h>



/**
 * print_python_list - func
 * @p: PyObject
 */
void print_python_list(PyObject *p)
{
	long int size = 0, i = 0;
	PyObject *item = NULL;

	if (!PyList_Check(p))
		return;
	printf("[*] Python list info\n");
	size = PyList_Size(p);
	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", ((PyListObject *)p)->allocated);
	for (; i < size; i++)
	{
		item = PyList_GetItem(p, i);
		printf("Element %ld: %s\n", i, Py_TYPE(item)->tp_name);
	}
}
