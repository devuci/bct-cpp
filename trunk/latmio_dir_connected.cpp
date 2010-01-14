#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_math.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*
 * Returns a 'latticized' graph R, with equivalent degree sequence to the original 
 * weighted directed graph G, and with preserved connectedness 
 * (hence the input graph must be connected).
 *
 * Each edge is rewired (on average) ITER times. The strength distributions 
 * are not preserved for weighted graphs.
 *
 * Rewiring algorithm: Maslov and Sneppen (2002) Science 296:910
 * Latticizing algorithm: Sporns and Zwi (2004); Neuroinformatics 2:145
 */

gsl_matrix* bct::latmio_dir_connected(const gsl_matrix* m, const int iters) {
	//It seems D could also be loaded from a file. How to consider that?
	gsl_matrix* R = copy(m);
	int n = m->size1;
	gsl_matrix* D = zeros(n);
	gsl_vector* v1 = sequence(1, 1, n-1); 
	gsl_vector* v2 = sequence(n-1, -1, 1); 
	gsl_matrix* u1 = concatenate_columns(v1, v2); 
	gsl_vector* v3 = min(u1); 
	gsl_vector* u = concatenate(0.0, v3); 
	gsl_vector_free(v1);
	gsl_vector_free(v2);
	gsl_vector_free(v3);
	gsl_matrix_free(u1);
	int upper_lim = (int)ceil((n-1)/2);
	for(int v = 0;v <= upper_lim;v++) {
		gsl_vector* index1 = sequence((v+1), 1, (n-1)); 
		gsl_vector* index2 = sequence(0, 1, v); 
		gsl_vector* u_seg1 = ordinal_index(u, index1); 
		gsl_vector* u_seg2 = ordinal_index(u, index2); 
		gsl_vector* seg_splice = concatenate(u_seg1, u_seg2); 
		gsl_vector_view bottom_row = gsl_matrix_row(D, (n-1-v));
		gsl_vector_memcpy(&bottom_row.vector, seg_splice);
		gsl_vector_view top_row = gsl_matrix_row(D, v);
		gsl_vector* rev_seg_splice = reverse(seg_splice); 
		gsl_vector_memcpy(&top_row.vector, rev_seg_splice);
		gsl_vector_free(index1);
		gsl_vector_free(index2);
		gsl_vector_free(u_seg1);
		gsl_vector_free(u_seg2);
		gsl_vector_free(seg_splice);
		gsl_vector_free(rev_seg_splice);
		
	}
	gsl_vector_free(u);
	
	//[i j]=find(tril(R));
	gsl_matrix* tril_R = tril(R);
	gsl_matrix* R_ij = find_ij(tril_R); 
	gsl_matrix_free(tril_R);
	gsl_vector_view i = gsl_matrix_column(R_ij, 0); 
	gsl_vector_view j = gsl_matrix_column(R_ij, 1); 
	int K = R_ij->size1;
	int tot_iters = iters;
	tot_iters *= K;
	int rewire = 0;
	srand(time(0));
	for(int iter = 1;iter <= tot_iters;iter++) {
		while(1) {
			rewire = 1;
			int a,b,c,d;
			int e1, e2;
			while(1) {
				e1 = ceil((K-1) * (((double)rand())/((double)RAND_MAX)));
				e2 = ceil((K-1) * (((double)rand())/((double)RAND_MAX)));
				while(e2 == e1) {
					e2 = ceil((K-1) * (((double)rand())/((double)RAND_MAX)));
				}
				a = gsl_vector_get(&i.vector, e1);
				b = gsl_vector_get(&j.vector, e1);
				c = gsl_vector_get(&i.vector, e2);
				d = gsl_vector_get(&j.vector, e2);
				//if all(a~=[c d]) && all(b~=[c d]);
				if(a != c && a != d && b!=c && b!=d) {  //all four vertices must be different
					break;
				}
			}
        
	        //if ~(R(a,d) || R(c,b))
	        //rewiring condition
	        if(!(gsl_matrix_get(R, a, d) || gsl_matrix_get(R, c, b))) {
	        	//if (D(a,b)+D(c,d))>=(D(a,d)+D(c,b))
	        	double val1 = gsl_matrix_get(D, a, b);
	        	double val2 = gsl_matrix_get(D, c, d);
	        	double val3 = gsl_matrix_get(D, a, d);
	        	double val4 = gsl_matrix_get(D, c, b);
	        	if((val1 + val2) >= (val3 + val4)) { //lattice condition
	        		//if ~(any([R(a,c) R(d,b) R(d,c)]) && any([R(c,a) R(b,d) R(b,a)]))
	        		gsl_vector* condn1 = gsl_vector_alloc(3);
	        		gsl_vector_set(condn1, 0, gsl_matrix_get(R, a, c));
	        		gsl_vector_set(condn1, 1, gsl_matrix_get(R, d, b));
	        		gsl_vector_set(condn1, 2, gsl_matrix_get(R, d, c));
	        		gsl_vector* condn2 = gsl_vector_alloc(3);
	        		gsl_vector_set(condn2, 0, gsl_matrix_get(R, c, a));
	        		gsl_vector_set(condn2, 1, gsl_matrix_get(R, b, d));
	        		gsl_vector_set(condn2, 2, gsl_matrix_get(R, b, a));
	        		int any_condn1 = any(condn1);
	        		int any_condn2 = any(condn2);
	        		if(!(any_condn1 && any_condn2)) { //connectedness condition
	        			//P=R([a c],:);
	        			gsl_vector* rows = gsl_vector_alloc(2);
	        			gsl_vector_set(rows, 0, a);
	        			gsl_vector_set(rows, 1, c);
	        			gsl_vector* columns = sequence(0, n-1);
	        			gsl_matrix* P = ordinal_index(m, rows, columns); 
	        			//P(1,b)=0; P(1,d)=1;
	        			gsl_matrix_set(P, 0, b, 0.0);
	        			gsl_matrix_set(P, 0, d, 1.0);
	        			//P(2,d)=0; P(2,b)=1;
	        			gsl_matrix_set(P, 1, d, 0.0);
	        			gsl_matrix_set(P, 1, b, 1.0);
	        			gsl_matrix* PN = copy(P); 
	        			//PN(1,a)=1; PN(2,c)=1;
	        			gsl_matrix_set(PN, 0, a, 1.0);
	        			gsl_matrix_set(PN, 1, c, 1.0);
	        			
	        			while(1) {
	        				//P(1,:)=any(R(P(1,:)~=0,:),1);
	        				gsl_vector_view row = gsl_matrix_row(P, 0); 
	        				gsl_vector* row_ind = compare_elements(&row.vector, cmp_not_equal, 0.0);  
	        				gsl_vector_free(columns);
	        				columns = sequence(0, n-1); 
	        				gsl_matrix* R_indxd = log_ord_index(R, row_ind, columns); 
	        				gsl_vector* any_row;
	        				if(R_indxd == NULL) { //Refer comments above the defintion of log_ord_index
	        					gsl_matrix* any_row_mat = zeros(1, columns->size);
	        					any_row = to_vector(any_row_mat);
	        					gsl_matrix_free(any_row_mat);
	        				}
	        				else {
		        				any_row = any(R_indxd, 1); 
		        			}
	        				gsl_vector_memcpy(&row.vector, any_row);
	        				gsl_vector_free(row_ind);
	        				if(R_indxd != NULL) {
		        				gsl_matrix_free(R_indxd);
		        			}
	        				gsl_vector_free(any_row);
	        				//P(2,:)=any(R(P(2,:)~=0,:),1);	        				
	        				row = gsl_matrix_row(P, 1);
	        				row_ind = compare_elements(&row.vector, cmp_not_equal, 0.0); 
	        				R_indxd = log_ord_index(R, row_ind, columns); 
	        				if(R_indxd == NULL) { //Refer comments above the defintion of log_ord_index
	        					gsl_matrix* any_row_mat = zeros(1, columns->size);
	        					any_row = to_vector(any_row_mat);
	        					gsl_matrix_free(any_row_mat);
	        				}
	        				else {
		        				any_row = any(R_indxd, 1); 
		        			}
	        				gsl_vector_memcpy(&row.vector, any_row);
	        				//P=P.*(~PN);
	        				gsl_matrix* not_PN = logical_not(PN); 
	        				gsl_matrix_mul_elements(P, not_PN);
	        				//PN=PN+P;
	        				gsl_matrix_add(PN, P);
	        				//if ~all(any(P,2))
	        				gsl_vector* any_P = any(P, 2); 
	        				int val = all(any_P);
	        				if(!val) {
	        					rewire = 0;
	        					break;
	        				}
		    				else {
								//elseif any(PN(1,[b c])) && any(PN(2,[d a]))
								gsl_vector_free(condn1);
								gsl_vector_free(condn2);
								condn1 = gsl_vector_alloc(2);
								condn2 = gsl_vector_alloc(2);
								gsl_vector_set(condn1, 0, gsl_matrix_get(PN, 0, b));
								gsl_vector_set(condn1, 1, gsl_matrix_get(PN, 0, c));
								gsl_vector_set(condn2, 0, gsl_matrix_get(PN, 1, d));
								gsl_vector_set(condn2, 1, gsl_matrix_get(PN, 1, a));
								any_condn1 = any(condn1);
								any_condn2 = any(condn2);
		    					if(any_condn1 && any_condn2 ) {
		    						break;
		    					}
		    				}
		    				gsl_matrix_add(PN, P);
		    				gsl_vector_free(row_ind);
		    				if(R_indxd != NULL) {
			    				gsl_matrix_free(R_indxd);
			    			}
		    				gsl_vector_free(any_row);
		    				gsl_matrix_free(not_PN);
	        			}
	        			gsl_vector_free(rows);
						gsl_vector_free(columns);
						gsl_matrix_free(P);
						gsl_matrix_free(PN);
	        		}
	        		
		    		if(rewire) {
		    			//R(a,d)=R(a,b); R(a,b)=0;
		    			gsl_matrix_set(R, a, d, gsl_matrix_get(R, a, b));
		    			gsl_matrix_set(R, a, b, 0.0);
	                	//R(c,b)=R(c,d); R(c,d)=0;
	                	gsl_matrix_set(R, c, b, gsl_matrix_get(R, c, d));
		    			gsl_matrix_set(R, c, d, 0.0);
		    			gsl_vector_set(&j.vector, e1, d);
		    			gsl_vector_set(&j.vector, e2, b);
		    			break;
		    		}
		    		gsl_vector_free(condn1);
		    		gsl_vector_free(condn2);
	        	}
			}
		}	 
	}
	gsl_matrix_free(R_ij);
	gsl_matrix_free(D);
	return R;
}      	
	        					
	        				
	        	
	            
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
		
	
				










