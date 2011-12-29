#include <gsl/gsl_math.h>

#include "bct.h"

MATRIX_T* distance_inv(const MATRIX_T*);

/*
 * Computes global efficiency.
 */
FP_T bct::efficiency_global(const MATRIX_T* G) {
	if (safe_mode) check_status(G, SQUARE, "efficiency_global");
	
	// N=length(G);
	int N = length(G);
	
	// e=distance_inv(G);
	MATRIX_T* e = distance_inv(G);
	
	// E=sum(e(:))./(N^2-N);
	VECTOR_T* e_v = to_vector(e);
	MATRIX_ID(free)(e);
	FP_T sum_e = sum(e_v);
	VECTOR_ID(free)(e_v);
	return sum_e / (FP_T)(N * (N - 1));
}

/*
 * Computes local efficiency.
 */
VECTOR_T* bct::efficiency_local(const MATRIX_T* G) {
	if (safe_mode) check_status(G, SQUARE, "efficiency_local");
	
	// N=length(G);
	int N = length(G);
	
	// E=zeros(N,1);
	VECTOR_T* E = zeros_vector(N);
	
	// for u=1:N
#ifdef _OPENMP
#pragma omp parallel for shared(E)
#endif
	for (int u = 0; u < N; u++) {
		
		// V=find(G(u,:));
		VECTOR_ID(const_view) G_row_u = MATRIX_ID(const_row)(G, u);
		VECTOR_T* V = find(&G_row_u.vector);
		if (V != NULL) {
			
			// k=length(V);
			int k = length(V);
			
			// if k>=2;
			if (k >= 2) {
				
				// e=distance_inv(G(V,V));
				MATRIX_T* G_idx = ordinal_index(G, V, V);
				MATRIX_T* e = distance_inv(G_idx);
				MATRIX_ID(free)(G_idx);
				
				// E(u)=sum(e(:))./(k.^2-k);
				VECTOR_T* e_v = to_vector(e);
				MATRIX_ID(free)(e);
				FP_T sum_e = sum(e_v);
				VECTOR_ID(free)(e_v);
				VECTOR_ID(set)(E, u, sum_e / (FP_T)(k * (k - 1)));
			}
			
			VECTOR_ID(free)(V);
		}
	}
	
	return E;
}

MATRIX_T* distance_inv(const MATRIX_T* g) {
	using namespace bct;
	
	// D=eye(length(g));
	MATRIX_T* D = eye(length(g));
	
	// n=1;
	int n = 1;
	
	// nPATH=g;
	MATRIX_T* nPATH = copy(g);
	
	// L=(nPATH~=0);
	MATRIX_T* L = compare_elements(nPATH, fp_not_equal, 0.0);
	
	// while find(L,1);
	VECTOR_T* find_L = find(L, 1);
	while (find_L != NULL) {
		VECTOR_ID(free)(find_L);
		
		// D=D+n.*L;
		MATRIX_ID(scale)(L, (FP_T)n);
		MATRIX_ID(add)(D, L);
		
		// n=n+1;
		n++;
		
		// nPATH=nPATH*g;
		MATRIX_T* temp = mul(nPATH, g);
		MATRIX_ID(free)(nPATH);
		nPATH = temp;
		
		// L=(nPATH~=0).*(D==0);
		MATRIX_ID(free)(L);
		L = compare_elements(nPATH, fp_not_equal, 0.0);
		MATRIX_T* D_eq_0 = compare_elements(D, fp_equal, 0.0);
		MATRIX_ID(mul_elements)(L, D_eq_0);
		MATRIX_ID(free)(D_eq_0);
		
		find_L = find(L, 1);
	}
	
	MATRIX_ID(free)(nPATH);
	MATRIX_ID(free)(L);
	
	// D(~D)=inf;
	MATRIX_T* not_D = logical_not(D);
	logical_index_assign(D, not_D, GSL_POSINF);
	MATRIX_ID(free)(not_D);
	
	// D=1./D;
	MATRIX_T* temp = pow_elements(D, -1.0);
	MATRIX_ID(free)(D);
	D = temp;
	
	// D=D-eye(length(g));
	MATRIX_T* eye_length_g = eye(length(g));
	MATRIX_ID(sub)(D, eye_length_g);
	MATRIX_ID(free)(eye_length_g);
	
	return D;
}
