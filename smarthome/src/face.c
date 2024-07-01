#include <Python.h>
#define WGET_CMD "wget http://127.0.0.1:8080/?action=snapshot -O /tmp/SearchFace.jpg"
#define SEARCHFACE_FILE "/tmp/SearchFace.jpg"

void face_init(void)
{
    Py_Initialize();

    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("."));
}

void face_final(void)
{
    Py_Finalize();
}

double face_category(void)
{   
    double result = 0.0;
    system(WGET_CMD);
    if (0 != access(SEARCHFACE_FILE, F_OK))
    {
        return result;
    }
    PyObject *pModule = PyImport_ImportModule("face");//face.py
    if (!pModule)
    {
        PyErr_Print();
        printf("Error: failed to load face.py\n");
        goto FAILED_MODULE;
    }

    PyObject *pFunc = PyObject_GetAttrString(pModule, "alibaba_face");//alibaba_face函数
    if (!pFunc)
    {
        PyErr_Print();
        printf("Error: failed to load alibabacloud_face\n");
        goto FAILED_FUNC;
    }

    PyObject *pValue = PyObject_CallObject(pFunc, NULL);//调用ailibaba_face函数
    if (!pValue)
    {
        PyErr_Print();
        printf("Error: function call failed\n");
        goto FAILED_VALUE;
    }

    if (!PyArg_Parse(pValue, "d", &result)) //解析获取调用ailibaba_face函数的返回值，转换成c格式
    {
        PyErr_Print();
        printf("Error: parse failed");
        goto FAILED_RESULT;
    }

    printf("result=%0.2lf\n",result);

    FAILED_RESULT:
    Py_DECREF(pValue);
    FAILED_VALUE:
    Py_DECREF(pFunc);
    FAILED_FUNC:
    Py_DECREF(pModule);
    FAILED_MODULE:

    return result;
}

#if 0
int main(int argc, char *argv[])
{
    double face_result = 0.0;
    face_init();
    face_result = face_category();  
    printf("face_result=%0.2lf\n",face_result);
    face_final();
}
#endif