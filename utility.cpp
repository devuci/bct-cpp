#include "bct.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <string>
#include <vector>

/*
 * Catches GSL errors and throws BCT exceptions.
 */
void bct::gsl_error_handler(const char* reason, const char* file, int line, int gsl_errno) {
	throw bct_exception(std::string(reason));
}

/*
 * Overloaded convenience function for freeing GSL vectors and matrices.
 */
void bct::gsl_free(gsl_vector* v) { gsl_vector_free(v); }
void bct::gsl_free(gsl_matrix* m) { gsl_matrix_free(m); }
void bct::gsl_free(std::vector<gsl_matrix*>& m) {
	for (int i = 0; i < (int)m.size(); i++) {
		if (m[i] != NULL) {
			gsl_matrix_free(m[i]);
			m[i] = NULL;
		}
	}
}

/*
 * Initializes the BCT library for external use.
 */
void bct::init() {
	gsl_set_error_handler(gsl_error_handler);
}

// TODO: This belongs in matlab/functions.cpp...move it when SWIG-accessible
double bct::mean(const gsl_vector* v) {
	double sum = 0.0;
	for (int i = 0; i < (int)v->size; i++) {
		sum += gsl_vector_get(v, i);
	}
	return sum / (double)v->size;
}

/*
 * Returns the number of links in a directed graph.
 */
int bct::number_of_links_dir(const gsl_matrix* m) {
	return nnz(m);
}

/*
 * Returns the number of links in an undirected graph.
 */
int bct::number_of_links_und(const gsl_matrix* m) {
	gsl_matrix* triu_m = triu(m);
	int ret = nnz(triu_m);
	gsl_matrix_free(triu_m);
	return ret;
}

/*
 * Returns the number of nodes in a graph.
 */
int bct::number_of_nodes(const gsl_matrix* m) {
	return (int)m->size1;
}
