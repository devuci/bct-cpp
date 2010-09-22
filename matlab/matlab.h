#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include <limits>
#include "sort.h"
#include <string>

namespace matlab {
	
	// Precision-independent definitions
#ifndef MATLAB_H
#define MATLAB_H
	
	// Functions
	std::string dec2bin(int);
	std::string dec2bin(int, int);
	gsl_matrix* inv(const gsl_matrix*);
	gsl_permutation* randperm(int);
	
	// Operators
	gsl_matrix* div_left(const gsl_matrix*, const gsl_matrix*);
	gsl_matrix* div_right(const gsl_matrix*, const gsl_matrix*);
	
	// Utility
	gsl_rng* get_gsl_rng();
	void seed_rng(const gsl_rng*, unsigned long);
#endif
	
	const FP_TYPE FP_ID(epsilon) = std::numeric_limits<FP_TYPE>::epsilon();
	
	// Functions
	VECTOR_TYPE* abs(const VECTOR_TYPE*);
	MATRIX_TYPE* abs(const MATRIX_TYPE*);
	int all(const VECTOR_TYPE*);
	VECTOR_TYPE* all(const MATRIX_TYPE*, int = 1);
	int any(const VECTOR_TYPE*);
	VECTOR_TYPE* any(const MATRIX_TYPE*, int = 1);
	MATRIX_TYPE* diag(const VECTOR_TYPE*, int = 0);
	VECTOR_TYPE* diag(const MATRIX_TYPE*, int = 0);
	MATRIX_TYPE* FP_ID(eye)(int);
	MATRIX_TYPE* FP_ID(eye)(int, int);
	VECTOR_TYPE* find(const VECTOR_TYPE*, int = std::numeric_limits<int>::max(), const std::string& = "first");
	VECTOR_TYPE* find(const MATRIX_TYPE*, int = std::numeric_limits<int>::max(), const std::string& = "first");
	MATRIX_TYPE* find_ij(const MATRIX_TYPE*, int = std::numeric_limits<int>::max(), const std::string& = "first");
	VECTOR_TYPE* hist(const VECTOR_TYPE*, int = 10);
	VECTOR_TYPE* hist(const VECTOR_TYPE*, const VECTOR_TYPE*);
	int length(const VECTOR_TYPE*);
	int length(const MATRIX_TYPE*);
	FP_TYPE max(FP_TYPE, FP_TYPE);
	FP_TYPE max(const VECTOR_TYPE*);
	VECTOR_TYPE* max(const MATRIX_TYPE*, int = 1);
	FP_TYPE min(FP_TYPE, FP_TYPE);
	FP_TYPE min(const VECTOR_TYPE*);
	VECTOR_TYPE* min(const MATRIX_TYPE*, int = 1);
	int nnz(const VECTOR_TYPE*);
	int nnz(const MATRIX_TYPE*);
	VECTOR_TYPE* nonzeros(const MATRIX_TYPE*);
	VECTOR_TYPE* normpdf(const VECTOR_TYPE*, FP_TYPE, FP_TYPE);
	MATRIX_TYPE* FP_ID(ones)(int);
	MATRIX_TYPE* FP_ID(ones)(int, int);
	VECTOR_TYPE* FP_ID(ones_vector)(int);
	FP_TYPE prod(const VECTOR_TYPE*);
	VECTOR_TYPE* prod(const MATRIX_TYPE*, int = 1);
	MATRIX_TYPE* FP_ID(rand)(int);
	MATRIX_TYPE* FP_ID(rand)(int, int);
	VECTOR_TYPE* FP_ID(rand_vector)(int);
	VECTOR_TYPE* reverse(const VECTOR_TYPE*);
	VECTOR_TYPE* setxor(const VECTOR_TYPE*, const VECTOR_TYPE*);
	VECTOR_TYPE* sort(const VECTOR_TYPE*, const std::string& = "ascend", VECTOR_TYPE** = NULL);
	MATRIX_TYPE* sort(const MATRIX_TYPE*, int = 1, const std::string& = "ascend", MATRIX_TYPE** = NULL);
	VECTOR_TYPE* sortrows(const VECTOR_TYPE*, VECTOR_TYPE** = NULL);
	MATRIX_TYPE* sortrows(const MATRIX_TYPE*, VECTOR_TYPE** = NULL);
	FP_TYPE sum(const VECTOR_TYPE*);
	VECTOR_TYPE* sum(const MATRIX_TYPE*, int = 1);
	MATRIX_TYPE* toeplitz(const VECTOR_TYPE*, const VECTOR_TYPE* = NULL);
	MATRIX_TYPE* tril(const MATRIX_TYPE*, int = 0);
	MATRIX_TYPE* triu(const MATRIX_TYPE*, int = 0);
	VECTOR_TYPE* unique(const VECTOR_TYPE*, const std::string& = "last", VECTOR_TYPE** = NULL, VECTOR_TYPE** = NULL);
	VECTOR_TYPE* unique(const MATRIX_TYPE*, const std::string& = "last", VECTOR_TYPE** = NULL, VECTOR_TYPE** = NULL);
	MATRIX_TYPE* unique_rows(const MATRIX_TYPE*, const std::string& = "last", VECTOR_TYPE** = NULL, VECTOR_TYPE** = NULL);
	MATRIX_TYPE* FP_ID(zeros)(int);
	MATRIX_TYPE* FP_ID(zeros)(int, int);
	VECTOR_TYPE* FP_ID(zeros_vector)(int);
	
	// Operators
	VECTOR_TYPE* concatenate(const VECTOR_TYPE*, FP_TYPE);
	VECTOR_TYPE* concatenate(FP_TYPE, const VECTOR_TYPE*);
	VECTOR_TYPE* concatenate(const VECTOR_TYPE*, const VECTOR_TYPE*);
	MATRIX_TYPE* concatenate_columns(const VECTOR_TYPE*, const VECTOR_TYPE*);
	MATRIX_TYPE* concatenate_columns(const MATRIX_TYPE*, const VECTOR_TYPE*);
	MATRIX_TYPE* concatenate_columns(const VECTOR_TYPE*, const MATRIX_TYPE*);	
	MATRIX_TYPE* concatenate_columns(const MATRIX_TYPE*, const MATRIX_TYPE*);
	MATRIX_TYPE* concatenate_rows(const VECTOR_TYPE*, const VECTOR_TYPE*);
	MATRIX_TYPE* concatenate_rows(const MATRIX_TYPE*, const VECTOR_TYPE*);
	MATRIX_TYPE* concatenate_rows(const VECTOR_TYPE*, const MATRIX_TYPE*);
	MATRIX_TYPE* concatenate_rows(const MATRIX_TYPE*, const MATRIX_TYPE*);
	VECTOR_TYPE* copy(const VECTOR_TYPE*);
	MATRIX_TYPE* copy(const MATRIX_TYPE*);
	VECTOR_TYPE* logical_and(const VECTOR_TYPE*, const VECTOR_TYPE*);
	MATRIX_TYPE* logical_and(const MATRIX_TYPE*, const MATRIX_TYPE*);
	VECTOR_TYPE* logical_not(const VECTOR_TYPE*);
	MATRIX_TYPE* logical_not(const MATRIX_TYPE*);
	VECTOR_TYPE* logical_or(const VECTOR_TYPE*, const VECTOR_TYPE*);
	MATRIX_TYPE* logical_or(const MATRIX_TYPE*, const MATRIX_TYPE*);
	MATRIX_TYPE* mul(const MATRIX_TYPE*, const MATRIX_TYPE*);
	MATRIX_TYPE* pow(const MATRIX_TYPE*, int);
	VECTOR_TYPE* pow_elements(const VECTOR_TYPE*, FP_TYPE);
	VECTOR_TYPE* pow_elements(const VECTOR_TYPE*, const VECTOR_TYPE*);
	MATRIX_TYPE* pow_elements(const MATRIX_TYPE*, FP_TYPE);
	MATRIX_TYPE* pow_elements(const MATRIX_TYPE*, const MATRIX_TYPE*);
	VECTOR_TYPE* FP_ID(sequence)(int, int);
	VECTOR_TYPE* FP_ID(sequence)(int, int, int);
	
	// Floating-point comparison
	int fp_compare(FP_TYPE, FP_TYPE);
	typedef bool (*FP_ID(fp_cmp_fn))(FP_TYPE, FP_TYPE);
	bool fp_zero(FP_TYPE);
	bool fp_nonzero(FP_TYPE);
	bool fp_equal(FP_TYPE, FP_TYPE);
	bool fp_not_equal(FP_TYPE, FP_TYPE);
	bool fp_less(FP_TYPE, FP_TYPE);
	bool fp_less_or_equal(FP_TYPE, FP_TYPE);
	bool fp_greater(FP_TYPE, FP_TYPE);
	bool fp_greater_or_equal(FP_TYPE, FP_TYPE);
	
	// Vector/matrix comparison
	int compare_vectors(const VECTOR_TYPE*, const VECTOR_TYPE*);
	bool vector_less(VECTOR_TYPE*, VECTOR_TYPE*);
	int compare_matrices(const MATRIX_TYPE*, const MATRIX_TYPE*);
	bool matrix_less(MATRIX_TYPE*, MATRIX_TYPE*);
	VECTOR_TYPE* compare_elements(const VECTOR_TYPE*, FP_ID(fp_cmp_fn), FP_TYPE);
	VECTOR_TYPE* compare_elements(const VECTOR_TYPE*, FP_ID(fp_cmp_fn), const VECTOR_TYPE*);
	MATRIX_TYPE* compare_elements(const MATRIX_TYPE*, FP_ID(fp_cmp_fn), FP_TYPE);
	MATRIX_TYPE* compare_elements(const MATRIX_TYPE*, FP_ID(fp_cmp_fn), const MATRIX_TYPE*);
	
	// Vector-by-vector indexing
	VECTOR_TYPE* ordinal_index(const VECTOR_TYPE*, const VECTOR_TYPE*);
	void ordinal_index_assign(VECTOR_TYPE*, const VECTOR_TYPE*, FP_TYPE);
	void ordinal_index_assign(VECTOR_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*);
	VECTOR_TYPE* logical_index(const VECTOR_TYPE*, const VECTOR_TYPE*);
	void logical_index_assign(VECTOR_TYPE*, const VECTOR_TYPE*, FP_TYPE);
	void logical_index_assign(VECTOR_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*);
	
	// Matrix-by-integer indexing
	FP_TYPE ordinal_index(const MATRIX_TYPE*, int);
	void ordinal_index_assign(MATRIX_TYPE*, int, FP_TYPE);
	
	// Matrix-by-vector indexing
	VECTOR_TYPE* ordinal_index(const MATRIX_TYPE*, const VECTOR_TYPE*);
	void ordinal_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, FP_TYPE);
	void ordinal_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*);
	VECTOR_TYPE* logical_index(const MATRIX_TYPE*, const VECTOR_TYPE*);
	void logical_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, FP_TYPE);
	void logical_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*);
	
	// Matrix-by-two-vectors indexing (non-mixed)
	MATRIX_TYPE* ordinal_index(const MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*);
	void ordinal_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*, FP_TYPE);
	void ordinal_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*, const MATRIX_TYPE*);
	MATRIX_TYPE* logical_index(const MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*);
	void logical_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*, FP_TYPE);
	void logical_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*, const MATRIX_TYPE*);
	
	// Matrix-by-two-vectors indexing (mixed)
	MATRIX_TYPE* ord_log_index(const MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*);
	void ord_log_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*, FP_TYPE);
	void ord_log_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*, const MATRIX_TYPE*);
	MATRIX_TYPE* log_ord_index(const MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*);
	void log_ord_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*, FP_TYPE);
	void log_ord_index_assign(MATRIX_TYPE*, const VECTOR_TYPE*, const VECTOR_TYPE*, const MATRIX_TYPE*);
	
	// Matrix-by-matrix indexing
	MATRIX_TYPE* ordinal_index(const MATRIX_TYPE*, const MATRIX_TYPE*);
	void ordinal_index_assign(MATRIX_TYPE*, const MATRIX_TYPE*, FP_TYPE);
	void ordinal_index_assign(MATRIX_TYPE*, const MATRIX_TYPE*, const MATRIX_TYPE*);
	VECTOR_TYPE* logical_index(const MATRIX_TYPE*, const MATRIX_TYPE*);
	void logical_index_assign(MATRIX_TYPE*, const MATRIX_TYPE*, FP_TYPE);
	void logical_index_assign(MATRIX_TYPE*, const MATRIX_TYPE*, const VECTOR_TYPE*);
	
	// Vector/matrix conversion
	void to_array(const VECTOR_TYPE*, FP_TYPE*);
	bool to_bool(const VECTOR_TYPE*);
	bool to_bool(const MATRIX_TYPE*);
	gsl_vector_float* to_vector_float(const VECTOR_TYPE*);
	gsl_vector* to_vector_double(const VECTOR_TYPE*);
	gsl_vector_long_double* to_vector_long_double(const VECTOR_TYPE*);
	VECTOR_TYPE* to_vector(const MATRIX_TYPE*);
	gsl_matrix_float* to_matrix_float(const MATRIX_TYPE*);
	gsl_matrix* to_matrix_double(const MATRIX_TYPE*);
	gsl_matrix_long_double* to_matrix_long_double(const MATRIX_TYPE*);
	MATRIX_TYPE* to_column_matrix(const VECTOR_TYPE*);
	MATRIX_TYPE* to_row_matrix(const VECTOR_TYPE*);
	VECTOR_TYPE* FP_ID(to_vector)(const gsl_permutation*);
	gsl_permutation* to_permutation(const VECTOR_TYPE*);
	
	// Utility
	MATRIX_TYPE* permute_columns(const gsl_permutation*, const MATRIX_TYPE*);
	MATRIX_TYPE* permute_rows(const gsl_permutation*, const MATRIX_TYPE*);
}
