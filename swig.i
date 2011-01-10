%{
	#include <gsl/gsl_matrix.h>
	#include <gsl/gsl_vector.h>
	#include <Python.h>
	#include <vector>
	
	/*
	 * Checks if a PyObject* is an n-dimensional list.
	 */
	bool is_ndim_list(PyObject* object, int n) {
		for (int i = 0; i < n; i++) {
			if (PyList_Check(object) == 1 && PyList_Size(object) > 0) {
				object = PyList_GetItem(object, PyList_Size(object) - 1);
			} else {
				return false;
			}
		}
		if (PyList_Check(object) == 0) {
			return true;
		} else {
			return false;
		}
	}
	
	/*
	 * Checks if a PyObject* can be converted to a gsl_vector*.
	 */
	bool is_gslv(PyObject* object) {
		return is_ndim_list(object, 1);
	}
	
	/*
	 * Checks if a PyObject* can be converted to a gsl_matrix*.
	 */
	bool is_gslm(PyObject* object) {
		return is_ndim_list(object, 2);
	}
	
	/*
	 * Checks if a PyObject* can be converted to a std::vector<gsl_matrix*>.
	 */
	bool is_gsl3dm(PyObject* object) {
		return is_ndim_list(object, 3);
	}
	
	/*
	 * Converts a gsl_vector* to a Python list.
	 */
	PyObject* from_gsl(const gsl_vector* v) {
		PyObject* list = PyList_New(v->size);
		for (int i = 0; i < (int)v->size; i++) {
			PyObject* value = PyFloat_FromDouble(gsl_vector_get(v, i));
			PyList_SetItem(list, i, value);
		}
		return list;
	}
	
	/*
	 * Converts a gsl_matrix* to a Python list of lists.
	 */
	PyObject* from_gsl(const gsl_matrix* m) {
		PyObject* list = PyList_New(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			PyObject* sublist = PyList_New(m->size2);
			PyList_SetItem(list, i, sublist);
			for (int j = 0; j < (int)m->size2; j++) {
				PyObject* value = PyFloat_FromDouble(gsl_matrix_get(m, i, j));
				PyList_SetItem(sublist, j, value);
			}
		}
		return list;
	}
	
	/*
	 * Converts a std::vector<gsl_matrix*> to a Python list of lists of lists.
	 */
	PyObject* from_gsl(const std::vector<gsl_matrix*>& m) {
		PyObject* list = PyList_New(m.size());
		for (int i = 0; i < (int)m.size(); i++) {
			if (m[i] == NULL) {
				PyList_SetItem(list, i, Py_None);
			} else {
				PyObject* sublist = PyList_New(m[i]->size1);
				PyList_SetItem(list, i, sublist);
				for (int j = 0; j < (int)m[i]->size1; j++) {
					PyObject* subsublist = PyList_New(m[i]->size2);
					PyList_SetItem(sublist, j, subsublist);
					for (int k = 0; k < (int)m[i]->size2; k++) {
						PyObject* value = PyFloat_FromDouble(gsl_matrix_get(m[i], j, k));
						PyList_SetItem(subsublist, k, value);
					}
				}
			}
		}
		return list;
	}
	
	/*
	 * Converts a Python list to a gsl_vector*.
	 */
	gsl_vector* to_gslv(PyObject* list) {
		int n = PyList_Size(list);
		gsl_vector* v = gsl_vector_alloc(n);
		for (int i = 0; i < n; i++) {
			double value = PyFloat_AsDouble(PyList_GetItem(list, i));
			gsl_vector_set(v, i, value);
		}
		return v;
	}
	
	/*
	 * Converts a Python list of lists to a gsl_matrix*.
	 */
	gsl_matrix* to_gslm(PyObject* list) {
		int n_rows = PyList_Size(list);
		int n_cols = PyList_Size(PyList_GetItem(list, 0));
		gsl_matrix* m = gsl_matrix_alloc(n_rows, n_cols);
		for (int i = 0; i < n_rows; i++) {
			PyObject* sublist = PyList_GetItem(list, i);
			for (int j = 0; j < n_cols; j++) {
				double value = PyFloat_AsDouble(PyList_GetItem(sublist, j));
				gsl_matrix_set(m, i, j, value);
			}
		}
		return m;
	}
	
	/*
	 * Converts a Python list of lists of lists to a std::vector<gsl_matrix*>.
	 */
	std::vector<gsl_matrix*> to_gsl3dm(PyObject* list) {
		int n_matrices = PyList_Size(list);
		std::vector<gsl_matrix*> m(n_matrices);
		for (int i = 0; i < n_matrices; i++) {
			PyObject* sublist = PyList_GetItem(list, i);
			if (sublist == Py_None) {
				m[i] = NULL;
			} else {
				int n_rows = PyList_Size(sublist);
				int n_cols = PyList_Size(PyList_GetItem(sublist, 0));
				gsl_matrix* subm = gsl_matrix_alloc(n_rows, n_cols);
				m[i] = subm;
				for (int j = 0; j < n_rows; j++) {
					PyObject* subsublist = PyList_GetItem(sublist, j);
					for (int k = 0; k < n_cols; k++) {
						double value = PyFloat_AsDouble(PyList_GetItem(subsublist, k));
						gsl_matrix_set(subm, j, k, value);
					}
				}
			}
		}
		return m;
	}
%}
