#ifndef BCT_H
#define BCT_H

#ifdef SWIG
#define BCT_SWIG
%module bct
%include stl.i
#endif

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>
#include "matlab/matlab_double.h"
#include "matlab/matlab_float.h"
#include "matlab/matlab_long_double.h"
#include <stdexcept>
#include <string>
#include <vector>

#ifdef BCT_SWIG
#include <Python.h>
#endif

namespace bct {
#ifndef BCT_SWIG
	using namespace matlab;
#endif
	
	class bct_exception : public std::runtime_error {
	public:
		bct_exception(const std::string& what_arg) : std::runtime_error(what_arg) { }
	};

	// Density, degree, and assortativity
	double assortativity_dir(const gsl_matrix*);
	double assortativity_und(const gsl_matrix*);
	gsl_vector* degrees_dir(const gsl_matrix*, gsl_vector** = NULL, gsl_vector** = NULL);
	gsl_vector* degrees_und(const gsl_matrix*);
	double density_dir(const gsl_matrix*);
	double density_und(const gsl_matrix*);
	gsl_matrix* jdegree(const gsl_matrix*);
	int jdegree_bl(const gsl_matrix*);
	int jdegree_id(const gsl_matrix*);
	int jdegree_od(const gsl_matrix*);
	gsl_matrix* matching_ind(const gsl_matrix*);
	gsl_matrix* matching_ind_in(const gsl_matrix*);
	gsl_matrix* matching_ind_out(const gsl_matrix*);
	gsl_vector* strengths_dir(const gsl_matrix*, gsl_vector** = NULL, gsl_vector** = NULL);
	gsl_vector* strengths_und(const gsl_matrix*);

	// Clustering
	gsl_vector* clustering_coef_bd(const gsl_matrix*);
	gsl_vector* clustering_coef_bu(const gsl_matrix*);
	gsl_vector* clustering_coef_wd(const gsl_matrix*);
	gsl_vector* clustering_coef_wu(const gsl_matrix*);
	gsl_vector* efficiency_local(const gsl_matrix*);

	// Paths, distances, and cycles
	gsl_vector* breadth(const gsl_matrix*, int, gsl_vector** = NULL);
	gsl_matrix* breadthdist(const gsl_matrix*, gsl_matrix** = NULL);
	gsl_vector* charpath_ecc(const gsl_matrix*, double* = NULL, double* = NULL);
	double charpath_lambda(const gsl_matrix*);
	double connectivity_length(const gsl_matrix*);
	gsl_vector* cycprob_fcyc(const std::vector<gsl_matrix*>&);
	gsl_vector* cycprob_pcyc(const std::vector<gsl_matrix*>&);
	gsl_matrix* distance_bin(const gsl_matrix*);
	gsl_matrix* distance_wei(const gsl_matrix*); 
	gsl_matrix* efficiency_global(const gsl_matrix*);
	std::vector<gsl_matrix*> findpaths(const gsl_matrix*, const gsl_vector*, int, gsl_vector** = NULL, int* = NULL, gsl_matrix** = NULL, gsl_matrix** = NULL);
	std::vector<gsl_matrix*> findwalks(const gsl_matrix*, gsl_vector** = NULL);
	double normalized_path_length(const gsl_matrix*, double = 1.0);
	gsl_matrix* reachdist(const gsl_matrix*, gsl_matrix** = NULL);

	// Centrality
	gsl_vector* betweenness_bin(const gsl_matrix*);
	gsl_vector* betweenness_wei(const gsl_matrix*);
	gsl_matrix* edge_betweenness_bin(const gsl_matrix*, gsl_vector** = NULL);
	gsl_matrix* edge_betweenness_wei(const gsl_matrix*, gsl_vector** = NULL);
	gsl_matrix* erange(const gsl_matrix*, double* = NULL, gsl_matrix** = NULL, double* = NULL);

	// Motifs
	enum motif_mode_enum { MILO, SPORNS };
	extern motif_mode_enum motif_mode;
	motif_mode_enum get_motif_mode();
	void set_motif_mode(motif_mode_enum);
	std::vector<gsl_matrix*> find_motif34(int, int);
	int find_motif34(const gsl_matrix*);
	gsl_vector* motif3funct_bin(const gsl_matrix*, gsl_matrix** = NULL);
	gsl_matrix* motif3funct_wei(const gsl_matrix*, gsl_matrix** = NULL, gsl_matrix** = NULL);
	gsl_vector* motif3funct_wei_v(const gsl_matrix*, gsl_vector** = NULL, gsl_vector** = NULL);
	gsl_matrix* motif3generate(gsl_vector** = NULL, gsl_vector** = NULL);
	gsl_vector* motif3struct_bin(const gsl_matrix*, gsl_matrix** = NULL);
	gsl_matrix* motif3struct_wei(const gsl_matrix*, gsl_matrix** = NULL, gsl_matrix** = NULL);
	gsl_vector* motif3struct_wei_v(const gsl_matrix*, gsl_vector** = NULL, gsl_vector** = NULL);
	gsl_matrix* motif4generate(gsl_vector** = NULL, gsl_vector** = NULL);
	gsl_vector* motif4funct_bin(const gsl_matrix*, gsl_matrix** = NULL);
	gsl_matrix* motif4funct_wei(const gsl_matrix*, gsl_matrix** = NULL, gsl_matrix** = NULL);
	gsl_vector* motif4funct_wei_v(const gsl_matrix*, gsl_vector** = NULL, gsl_vector** = NULL);
	gsl_vector* motif4struct_bin(const gsl_matrix*, gsl_matrix** = NULL);
	gsl_matrix* motif4struct_wei(const gsl_matrix*, gsl_matrix** = NULL, gsl_matrix** = NULL);
	gsl_vector* motif4struct_wei_v(const gsl_matrix*, gsl_vector** = NULL, gsl_vector** = NULL);

	// Modularity and community structure
	double modularity_dir(const gsl_matrix*, gsl_vector** = NULL);
	double modularity_und(const gsl_matrix*, gsl_vector** = NULL);
	double modularity_und_louvain(const gsl_matrix*, gsl_vector** = NULL, int = 100);
	gsl_vector* module_degree_zscore(const gsl_matrix*, const gsl_vector*);
	gsl_vector* participation_coef(const gsl_matrix*, const gsl_vector*);
	
	// Synthetic connection networks
	gsl_matrix* makeevenCIJ(int, int, int);
	gsl_matrix* makefractalCIJ(int, double, int, int* = NULL);
	gsl_matrix* makelatticeCIJ(int, int);
	gsl_matrix* makerandCIJ_bd(int, int);
	gsl_matrix* makerandCIJ_bu(int, int);
	gsl_matrix* makerandCIJ_wd(int, int, double, double);
	gsl_matrix* makerandCIJ_wd_wp(const gsl_matrix*);
	gsl_matrix* makerandCIJ_wu(int, int, double, double);
	gsl_matrix* makerandCIJ_wu_wp(const gsl_matrix*);
	gsl_matrix* makerandCIJdegreesfixed(const gsl_vector*, const gsl_vector*);
	gsl_matrix* makerandCIJdegreesfixed(const gsl_matrix*);
	gsl_matrix* makeringlatticeCIJ(int, int);
	gsl_matrix* maketoeplitzCIJ(int, int, double);
	
	// Graph randomization
	gsl_matrix* latmio_dir(const gsl_matrix*, int);
	gsl_matrix* latmio_dir_connected(const gsl_matrix*, int);	
	gsl_matrix* latmio_und(const gsl_matrix*, int);
	gsl_matrix* latmio_und_connected(const gsl_matrix*, int);
	gsl_matrix* randmio_dir(const gsl_matrix*, int);
	gsl_matrix* randmio_dir_connected(const gsl_matrix*, int);
	gsl_matrix* randmio_und(const gsl_matrix*, int);
	gsl_matrix* randmio_und_connected(const gsl_matrix*, int);

	// Data sets
	extern const double cat_all[95 * 95];
	extern const double cat_ctx[52 * 52];
	extern const double fve30[30 * 30];
	extern const double fve32[32 * 32];
	extern const double macaque47[47 * 47];
	extern const double macaque71[71 * 71];
	
	// Matrix status checking
	enum status {
		SQUARE = 1, RECTANGULAR = 2,
		UNDIRECTED = 4, DIRECTED = 8,
		BINARY = 16, WEIGHTED = 32,
		POSITIVE = 64, SIGNED = 128,
		NO_LOOPS = 256, LOOPS = 512
	};
	extern bool safe_mode;
	bool get_safe_mode();
	void set_safe_mode(bool);
	bool check_status(const gsl_matrix*, int, const std::string&);
	bool is_square(const gsl_matrix*);
	bool is_rectangular(const gsl_matrix*);
	bool is_undirected(const gsl_matrix*);
	bool is_directed(const gsl_matrix*);
	bool is_binary(const gsl_matrix*);
	bool is_weighted(const gsl_matrix*);
	bool is_positive(const gsl_matrix*);
	bool is_signed(const gsl_matrix*);
	bool has_loops(const gsl_matrix*);
	bool has_no_loops(const gsl_matrix*);
	
	// Matrix conversion
	gsl_matrix* invert_elements(const gsl_matrix*);
	gsl_matrix* remove_loops(const gsl_matrix*);
	gsl_matrix* to_binary(const gsl_matrix*);
	gsl_matrix* to_positive(const gsl_matrix*);
	gsl_matrix* to_undirected_bin(const gsl_matrix*);
	gsl_matrix* to_undirected_wei(const gsl_matrix*);
	
	// Utility
	void gsl_error_handler(const char*, const char*, int, int);
	void gsl_free(gsl_vector*);
	void gsl_free(gsl_matrix*);
	void gsl_free(std::vector<gsl_matrix*>&);
	void init();
	int number_of_edges_dir(const gsl_matrix*);
	int number_of_edges_und(const gsl_matrix*);
	int number_of_nodes(const gsl_matrix*);
	gsl_matrix* threshold_absolute(const gsl_matrix*, double);
	gsl_matrix* threshold_proportional_dir(const gsl_matrix*, double);
	gsl_matrix* threshold_proportional_und(const gsl_matrix*, double);
	double mean(const gsl_vector*, const std::string& = "a");
	gsl_vector* mean(const gsl_matrix*, int = 1, const std::string& = "a");
	double std(const gsl_vector*, int = 0);
	gsl_vector* std(const gsl_matrix*, int = 0, int = 1);
	
	// Debugging
	void printf(const gsl_vector*, const std::string&);
	void printf(const gsl_matrix*, const std::string&);
	void printf(const gsl_permutation*, const std::string&);
	
	// SWIG
#ifdef BCT_SWIG
	PyObject* from_gsl(const gsl_vector*);
	PyObject* from_gsl(const gsl_matrix*);
	PyObject* from_gsl(const std::vector<gsl_matrix*>&);
	gsl_vector* to_gslv(const double*, int);
	gsl_matrix* to_gslm(const double*, int, int);
	gsl_vector* to_gslv(PyObject*);
	gsl_matrix* to_gslm(PyObject*);
	std::vector<gsl_matrix*> to_gsl3dm(PyObject*);
#endif
};

#endif
