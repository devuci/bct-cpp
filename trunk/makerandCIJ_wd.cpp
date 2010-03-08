#include "bct.h"
#include <ctime>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_permute_vector.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>

gsl_matrix* makerandCIJ_wd(int, const gsl_vector*);

/*
 * Generates a random directed weighted graph with N nodes and K edges.  Weights
 * are chosen uniformly between wmin and wmax.  No edges are placed on the main
 * diagonal.
 */
gsl_matrix* bct::makerandCIJ_wd(int N, int K, double wmin, double wmax) {
	gsl_rng_default_seed = std::time(NULL);
	gsl_rng* rng = gsl_rng_alloc(gsl_rng_default);
	gsl_vector* w = gsl_vector_alloc(K);
	for (int x = 0; x < K; x++) {
		gsl_vector_set(w, x, gsl_rng_uniform(rng) * (wmax - wmin) + wmin);
	}
	gsl_rng_free(rng);
	gsl_matrix* ret = makerandCIJ_wd(N, w);
	gsl_vector_free(w);
	return ret;
}

/*
 * Generates a random directed weighted graph with the same number of nodes,
 * number of edges, and weight distribution as the given graph.  No edges are
 * placed on the main diagonal.  The given matrix should therefore not contain
 * nonzero entries on the main diagonal.
 */
gsl_matrix* bct::makerandCIJ_wd(const gsl_matrix* m) {
	int N = m->size1;
	gsl_vector* w = nonzeros(m);
	gsl_matrix* ret = makerandCIJ_wd(N, w);
	gsl_vector_free(w);
	return ret;
}

gsl_matrix* makerandCIJ_wd(int N, const gsl_vector* w) {
	using namespace bct;
	
	int K = w->size;
	
	// ind = ~eye(N);
	gsl_matrix* eye_N = eye_double(N);
	gsl_matrix* ind = logical_not(eye_N);
	gsl_matrix_free(eye_N);
	
	// i = find(ind);
	gsl_vector* i = find(ind);
	gsl_matrix_free(ind);
	
	// rp = randperm(length(i));
	gsl_permutation* rp = randperm(length(i));
	
	// irp = i(rp);
	gsl_permute_vector(rp, i);
	gsl_permutation_free(rp);
	gsl_vector* irp = i;
	
	// CIJ = zeros(N);
	gsl_matrix* CIJ = zeros_double(N);
	
	// CIJ(irp(1:K)) = w;
	gsl_vector_view irp_subv = gsl_vector_subvector(irp, 0, K);
	ordinal_index_assign(CIJ, &irp_subv.vector, w);
	gsl_vector_free(irp);
	
	return CIJ;
}