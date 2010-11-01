#ifndef BCT_SWIG_H
#define BCT_SWIG_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <Python.h>
#include <vector>

namespace bct {
	PyObject* from_gsl(const gsl_vector* v);
	PyObject* from_gsl(const gsl_matrix* m);
	PyObject* from_gsl(const std::vector<gsl_matrix*>& m);
	gsl_vector* to_gslv(const double* array, int size);
	gsl_matrix* to_gslm(const double* array, int size1, int size2);
	gsl_vector* to_gslv(PyObject* list);
	gsl_matrix* to_gslm(PyObject* list);
	std::vector<gsl_matrix*> to_gsl3dm(PyObject* list);
}

#endif
