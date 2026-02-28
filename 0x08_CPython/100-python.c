#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * struct my_PyLongObject - Custom proxy for Python 3.4's internal long object
 * @ob_base: The base variable object
 * @ob_digit: The array of digits (limbs)
 *
 * This matches the internal layout required by the Python 3.4 exercise.
 */
typedef struct my_PyLongObject {
    PyVarObject ob_base;
    uint32_t ob_digit[1];
} my_PyLong;

void print_python_int(PyObject *p)
{
    my_PyLong *l;
    Py_ssize_t size, i;
    uint32_t *digits;
    char *buffer;
    int negative = 0, buf_idx = 0;

    if (!p || !PyLong_Check(p))
    {
        printf("Invalid Int Object\n");
        return;
    }

    /* Cast to our proxy structure instead of PyLongObject */
    l = (my_PyLong *)p;
    size = l->ob_base.ob_size;

    if (size == 0)
    {
        printf("0\n");
        return;
    }

    if (size < 0)
    {
        negative = 1;
        size = -size;
    }

    /* Copy digits to a mutable array for the division-by-10 algorithm */
    digits = malloc(size * sizeof(uint32_t));
    if (!digits) return;
    for (i = 0; i < size; i++)
        digits[i] = l->ob_digit[i];

    /* Buffer: size * 10 is roughly enough for 2^30 limbs converted to base 10 */
    buffer = malloc(size * 10 + 2);
    if (!buffer) { free(digits); return; }

    while (size > 0)
    {
        uint64_t remainder = 0;
        for (i = size - 1; i >= 0; i--)
        {
            uint64_t temp = (remainder << PyLong_SHIFT) + digits[i];
            digits[i] = (uint32_t)(temp / 10);
            remainder = temp % 10;
        }
        buffer[buf_idx++] = (char)(remainder + '0');

        while (size > 0 && digits[size - 1] == 0)
            size--;
    }

    if (negative)
        putchar('-');

    for (i = buf_idx - 1; i >= 0; i--)
        putchar(buffer[i]);
    putchar('\n');

    free(digits);
    free(buffer);
}
