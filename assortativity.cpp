#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/* Assortativity coefficient. Essentially, the assortativity a correlation 
 * coefficient for the degrees of linked nodes. A positive assortativity coefficient 
 * indicates that nodes tend to link to other nodes with the same or similar degree.  
 * The function accepts weighted networks, but the connection weights are ignored.
 */
 
double bct::assortativity_dir(const gsl_matrix* m) {
	int K;
	gsl_vector* deg = degrees_dir(m);
	gsl_matrix* gt0_m = compare_elements(m, cmp_greater, 0.0);
	gsl_matrix* ij = find_ij(gt0_m);
	gsl_vector* degi = gsl_vector_alloc(ij->size1);
	gsl_vector* degj = gsl_vector_alloc(ij->size1);		
	gsl_vector_view i = gsl_matrix_column(ij, 0);
	gsl_vector_view j = gsl_matrix_column(ij, 1);
	K = ij->size1;
	for(int k = 0;k < K;k++) {
		gsl_vector_set(degi, k, gsl_vector_get(deg, ((int)gsl_vector_get(&i.vector, k))));
		gsl_vector_set(degj, k, gsl_vector_get(deg, ((int)gsl_vector_get(&j.vector, k))));
	}
	
	double a = assortativity(degi, degj, K);
	gsl_vector_free(deg);
	gsl_matrix_free(gt0_m);
	gsl_matrix_free(ij);	
	gsl_vector_free(degi);
	gsl_vector_free(degj);
	return a;
}

double bct::assortativity_und(const gsl_matrix* m) {
	int K;
	gsl_vector* deg = degrees_und(m);
	gsl_matrix* triu_m = triu(m, 1);
	gsl_matrix* gt0_triu_m = compare_elements(triu_m, cmp_greater, 0.0);
	gsl_matrix* ij = find_ij(gt0_triu_m);
	gsl_vector* degi = gsl_vector_alloc(ij->size1);
	gsl_vector* degj = gsl_vector_alloc(ij->size1);	
	gsl_vector_view i = gsl_matrix_column(ij, 0);
	gsl_vector_view j = gsl_matrix_column(ij, 1);
	K = ij->size1;
	for(int k = 0;k < K;k++) {
		gsl_vector_set(degi, k, gsl_vector_get(deg, ((int)gsl_vector_get(&i.vector, k))));
		gsl_vector_set(degj, k, gsl_vector_get(deg, ((int)gsl_vector_get(&j.vector, k))));
	}

	double a = assortativity(degi, degj, K);
	gsl_vector_free(deg);
	gsl_matrix_free(triu_m);
	gsl_matrix_free(gt0_triu_m);
	gsl_matrix_free(ij);	
	gsl_vector_free(degi);
	gsl_vector_free(degj);
	return a;	
}

double bct::assortativity(const gsl_vector* degi, const gsl_vector* degj, int K) {

	//r = (sum(degi.*degj)/K - (sum(0.5*(degi+degj))/K)^2)/(sum(0.5*(degi.^2+degj.^2))/K - (sum(0.5*(degi+degj))/K)^2);
	gsl_vector* degtemp = gsl_vector_alloc(degi->size);
	gsl_vector_memcpy(degtemp, degi);
	gsl_vector_mul(degtemp, degj);
	double term1 = sum(degtemp)/(double)K;
	
	gsl_vector_memcpy(degtemp, degi);
	gsl_vector_add(degtemp, degj);
	gsl_vector_scale(degtemp, (double)0.5);
	double term2 = sum(degtemp)/(double)K;
	term2 *= term2;
	
	double termA = term1 - term2;
	
	gsl_vector_memcpy(degtemp, degi);
	gsl_vector_mul(degtemp,degi); //degi.^2
	gsl_vector* degtemp2 = gsl_vector_alloc(degj->size);
	gsl_vector_memcpy(degtemp2, degj);
	gsl_vector_mul(degtemp2,degj); //degj.^2	
	gsl_vector_add(degtemp, degtemp2);
	gsl_vector_scale(degtemp, (double)0.5);
	double term3 = sum(degtemp)/(double)K;
	
	gsl_vector_memcpy(degtemp, degi);
	gsl_vector_add(degtemp, degj);
	gsl_vector_scale(degtemp, (double)0.5);
	double term4 = sum(degtemp)/(double)K;
	term4 *= term4;
	
	double termB = term3 - term4;
	
	double r = (double)termA/(double)termB;
	
	return r;
}
