#include <iostream>

#include "bct.h"

bool bct::safe_mode = true;

bool bct::get_safe_mode() { return safe_mode; }
void bct::set_safe_mode(bool safe_mode) { bct::safe_mode = safe_mode; }

/*
 * Returns whether a matrix matches the given status flags.  If the check fails,
 * a message is printed to stderr starting with the given text.
 */
bool bct::check_status(const MATRIX_T* m, int flags, const std::string& text) {
	status prop[] = {
		SQUARE, RECTANGULAR,
		UNDIRECTED, DIRECTED,
		BINARY, WEIGHTED,
		POSITIVE, SIGNED,
		NO_LOOPS, LOOPS
	};
	std::string propstr[] = {
		"square", "rectangular",
		"undirected", "directed",
		"binary", "weighted",
		"positive", "signed",
		"no_loops", "loops"
	};
	bool (*propfn[])(const MATRIX_T*) = {
		is_square, is_rectangular,
		is_undirected, is_directed,
		is_binary, is_weighted,
		is_positive, is_signed,
		has_no_loops, has_loops
	};
	bool ret = true;
	std::vector<std::string> failures;
	for (int i = 0; i < 10; i++) {
		if (flags & (int)prop[i]) {
			if (!propfn[i](m)) {
				ret = false;
				failures.push_back(propstr[i]);
			}
		}
	}
	if (!ret) {
		std::cerr << text << ": Matrix status check failed (not ";
		for (int i = 0; i < (int)failures.size(); i++) {
			std::cerr << failures[i];
			if (i < (int)failures.size() - 1) {
				std::cerr << ", ";
			}
		}
		std::cerr << ")." << std::endl;
	}
	return ret;
}

/*
 * Returns whether the given matrix is square.
 */
bool bct::is_square(const MATRIX_T* m) {
	return m->size1 == m->size2;
}

/*
 * Returns whether the given matrix is rectangular.
 */
bool bct::is_rectangular(const MATRIX_T* m) {
	return m->size1 != m->size2;
}

/*
 * Returns whether the given matrix is undirected.
 */
bool bct::is_undirected(const MATRIX_T* m) {
	return !is_directed(m);
}

/*
 * Returns whether the given matrix is directed.
 */
bool bct::is_directed(const MATRIX_T* m) {
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			if (fp_not_equal(MATRIX_ID(get)(m, i, j), MATRIX_ID(get)(m, j, i))) {
				return true;
			}
		}
	}
	return false;
}

/*
 * Returns whether the given matrix is binary.
 */
bool bct::is_binary(const MATRIX_T* m) {
	return !is_weighted(m);
}

/*
 * Returns whether the given matrix is weighted.
 */
bool bct::is_weighted(const MATRIX_T* m) {
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			FP_T value = MATRIX_ID(get)(m, i, j);
			if (fp_nonzero(value) && fp_not_equal(value, 1.0)) {
				return true;
			}
		}
	}
	return false;
}

/*
 * Returns whether the given matrix is positive.
 */
bool bct::is_positive(const MATRIX_T* m) {
	return !is_signed(m);
}

/*
 * Returns whether the given matrix is signed.
 */
bool bct::is_signed(const MATRIX_T* m) {
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			if (MATRIX_ID(get)(m, i, j) < 0.0) {
				return true;
			}
		}
	}
	return false;
}

/*
 * Returns whether the given matrix has loops.
 */
bool bct::has_loops(const MATRIX_T* m) {
	return !has_no_loops(m);
}

/*
 * Returns whether the given matrix has no loops.
 */
bool bct::has_no_loops(const MATRIX_T* m) {
	VECTOR_ID(const_view) diag_m = MATRIX_ID(const_diagonal)(m);
	return VECTOR_ID(isnull)(&diag_m.vector) == 1;
}
