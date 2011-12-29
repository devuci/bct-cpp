#include <gsl/gsl_math.h>

#include "bct.h"

/*
 * Computes nodal participation coefficient for a binary graph and its
 * corresponding community structure.  For a directed graph, computes "out-
 * neighbor" participation coefficient.
 */
VECTOR_T* bct::participation_coef(const MATRIX_T* A, const VECTOR_T* Ci) {
	if (safe_mode) check_status(A, SQUARE | BINARY, "participation_coef");
	
	// n=length(A);
	int n = length(A);
	
	// Ko=sum(A,2);
	VECTOR_T* Ko = sum(A, 2);
	
	// Ko(~Ko)=inf;
	VECTOR_T* not_Ko = logical_not(Ko);
	logical_index_assign(Ko, not_Ko, GSL_POSINF);
	VECTOR_ID(free)(not_Ko);
	
	// Gc=A*diag(Ci);
	MATRIX_T* diag_Ci = diag(Ci);
	MATRIX_T* Gc = mul(A, diag_Ci);
	MATRIX_ID(free)(diag_Ci);
	
	// Kc2=zeros(n,1);
	VECTOR_T* Kc2 = zeros_vector(n);
	
	// for i=1:max(Ci);
	for (int i = 1; i <= (int)max(Ci); i++) {
		
		// Kc2=Kc2+(sum(Gc==i,2).^2);
		MATRIX_T* Gc_eq_i = compare_elements(Gc, fp_equal, (FP_T)i);
		VECTOR_T* sum_Gc_eq_i = sum(Gc_eq_i, 2);
		MATRIX_ID(free)(Gc_eq_i);
		VECTOR_T* sum_Gc_eq_i_pow_2 = pow_elements(sum_Gc_eq_i, 2.0);
		VECTOR_ID(free)(sum_Gc_eq_i);
		VECTOR_ID(add)(Kc2, sum_Gc_eq_i_pow_2);
		VECTOR_ID(free)(sum_Gc_eq_i_pow_2);
	}
	
	MATRIX_ID(free)(Gc);
	
	// P=ones(n,1)-Kc2./(Ko.^2);
	VECTOR_T* Ko_pow_2 = pow_elements(Ko, 2.0);
	VECTOR_ID(free)(Ko);
	VECTOR_ID(div)(Kc2, Ko_pow_2);
	VECTOR_ID(free)(Ko_pow_2);
	VECTOR_T* P = ones_vector(n);
	VECTOR_ID(sub)(P, Kc2);
	VECTOR_ID(free)(Kc2);
	
	return P;
}
