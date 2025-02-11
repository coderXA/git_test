#if 0
#endif
#include <Python.h>
#include "garbage.h"

void garbage_init(void)
{
    Py_Initialize();

    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("."));
}

void garbage_final(void)
{
    Py_Finalize();
}

char *garbage_category(char *category)
{
    PyObject *pModule = PyImport_ImportModule("garbage");
    if (!pModule)
    {
        PyErr_Print();
        printf("Error: failed to load garbage.py\n");
        goto FAILED_MODULE;
    }

    PyObject *pFunc = PyObject_GetAttrString(pModule, "alibaba_garbage");
    if (!pFunc)
    {
        PyErr_Print();
        printf("Error: failed to load alibabacloud_garbage\n");
        goto FAILED_FUNC;
    }

    PyObject *pValue = PyObject_CallObject(pFunc, NULL);
    if (!pValue)
    {
        PyErr_Print();
        printf("Error: function call failed\n");
        goto FAILED_VALUE;
    }

    char *result = NULL;
    if (!PyArg_Parse(pValue, "s", &result))
    {
        PyErr_Print();
        printf("Error: parse failed");
        goto FAILED_RESULT;
    }

    category = (char *)malloc(sizeof(char) * (strlen(result) + 1) );
    memset(category, 0, (strlen(result) + 1));
    strncpy(category, result, (strlen(result) + 1));

    FAILED_RESULT:
    Py_DECREF(pValue);
    FAILED_VALUE:
    Py_DECREF(pFunc);
    FAILED_FUNC:
    Py_DECREF(pModule);
    FAILED_MODULE:
    return category;
}