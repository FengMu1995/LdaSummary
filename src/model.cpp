/*
 * Copyright (C) 2007 by
 * 
 * 	Xuan-Hieu Phan
 *	hieuxuan@ecei.tohoku.ac.jp or pxhieu@gmail.com
 * 	Graduate School of Information Sciences
 * 	Tohoku University
 *
 * GibbsLDA++ is a free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * GibbsLDA++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GibbsLDA++; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

/* 
 * References:
 * + The Java code of Gregor Heinrich (gregor@arbylon.net)
 *   http://www.arbylon.net/projects/LdaGibbsSampler.java
 * + "Parameter estimation for text analysis" by Gregor Heinrich
 *   http://www.arbylon.net/publications/text-est.pdf
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iterator>
//#include "strtokenizer.h"
#include "utils.h"
//#include "dataset.h"
#include "model.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

using namespace std;

model::~model() {
    if (p) {
	delete p;
    }

    
    if (pnewdata) {
	delete pnewdata;
    }

    // if (z) {
	// for (int m = 0; m < M; m++) {
	//     if (z[m]) {
	// 	delete z[m];
	//     }
	// }
    // }
    
    // if (nw) {
	// for (int w = 0; w < V; w++) {
	//     if (nw[w]) {
	// 	delete nw[w];
	//     }
	// }
    // }

    // if (nd) {
	// for (int m = 0; m < M; m++) {
	//     if (nd[m]) {
	// 	delete nd[m];
	//     }
	// }
    // } 
    
    // if (nwsum) {
	// delete nwsum;
    // }   
    
    // if (ndsum) {
	// delete ndsum;
    // }
    
    // if (theta) {
	// for (int m = 0; m < M; m++) {
	//     if (theta[m]) {
	// 	delete theta[m];
	//     }
	// }
    // }
    
    // if (phi) {
	// for (int k = 0; k < K; k++) {
	//     if (phi[k]) {
	// 	delete phi[k];
	//     }
	// }
    // }

    // // only for inference
    // if (newz) {
	// for (int m = 0; m < newM; m++) {
	//     if (newz[m]) {
	// 	delete newz[m];
	//     }
	// }
    // }
    
    if (newnw) {
	for (int w = 0; w < newV; w++) {
	    if (newnw[w]) {
		delete newnw[w];
	    }
	}
    }

    if (newnd) {
	for (int m = 0; m < newM; m++) {
	    if (newnd[m]) {
		delete newnd[m];
	    }
	}
    } 
    
    if (newnwsum) {
	delete newnwsum;
    }   
    
    if (newndsum) {
	delete newndsum;
    }
    
    if (newtheta) {
	for (int m = 0; m < newM; m++) {
	    if (newtheta[m]) {
		delete newtheta[m];
	    }
	}
    }
    
    if (newphi) {
	for (int k = 0; k < K; k++) {
	    if (newphi[k]) {
		delete newphi[k];
	    }
	}
    }
}

void model::set_default_values() {
    twords_suffix = ".twords";
    
    dir = "./";
    pnewdata = NULL;
    
    M = 0;
    K = 100;
    alpha = 50.0 / K;
    beta = 0.1;
    niters = 2000;
    twords = 0;
    sentNum=0;
    
    p = NULL;
    //z = NULL;
    // nw = NULL;
    // nd = NULL;
    // nwsum = NULL;
    // ndsum = NULL;
    // theta = NULL;
    // phi = NULL;
    
    newM = 0;
    newV = 0;
    newz = NULL;
    newnw = NULL;
    newnd = NULL;
    newnwsum = NULL;
    newndsum = NULL;
    newtheta = NULL;
    newphi = NULL;
    docs_words.clear();
}

int model::init(int argc, char ** argv, vector<vector<string>>& filtered_doc_words) {
    if(utils::parse_args(argc, argv,filtered_doc_words, this))
        return 1;
	// do inference
	if (init_inf())
	    return 1;
    return 0;
}

int model::save_inf_model(string model_name) {
   
    
    // if (save_inf_model_newtheta(dir + model_name + theta_suffix)) {
	//     return 1;
    // }
    
    // if (save_inf_model_newphi(dir + model_name + phi_suffix)) {
	//     return 1;
    // }

    if (twords > 0) 
    {
	    if (save_inf_model_twords(dir + model_name + twords_suffix))
	        return 1;
    }
    return 0;
}

int model::save_inf_model_tassign(string filename) {
    int i, j;
    
    FILE * fout = fopen(filename.c_str(), "w");
    if (!fout) {
	printf("Cannot open file %s to save!\n", filename.c_str());
	return 1;
    }

    // wirte docs with topic assignments for words
    for (i = 0; i < pnewdata->M; i++) {    
	for (j = 0; j < pnewdata->docs[i]->length; j++) {
	    fprintf(fout, "%d:%d ", pnewdata->docs[i]->words[j], newz[i][j]);
	}
	fprintf(fout, "\n");
    }

    fclose(fout);
    
    return 0;
}

int model::save_inf_model_newtheta(string filename) {
    int i, j;

    FILE * fout = fopen(filename.c_str(), "w");
    if (!fout) {
	printf("Cannot open file %s to save!\n", filename.c_str());
	return 1;
    }
    
    for (i = 0; i < newM; i++) {
	for (j = 0; j < K; j++) {
	    fprintf(fout, "%f ", newtheta[i][j]);
	}
	fprintf(fout, "\n");
    }
    
    fclose(fout);
    
    return 0;
}

int model::save_inf_model_newphi(string filename) {
    FILE * fout = fopen(filename.c_str(), "w");
    if (!fout) {
	printf("Cannot open file %s to save!\n", filename.c_str());
	return 1;
    }
    
    for (int i = 0; i < K; i++) {
	for (int j = 0; j < newV; j++) {
	    fprintf(fout, "%f ", newphi[i][j]);
	}
	fprintf(fout, "\n");
    }
    
    fclose(fout);    
    
    return 0;
}

int model::save_inf_model_others(string filename) {
    FILE * fout = fopen(filename.c_str(), "w");
    if (!fout) {
	printf("Cannot open file %s to save!\n", filename.c_str());
	return 1;
    }

    fprintf(fout, "alpha=%f\n", alpha);
    fprintf(fout, "beta=%f\n", beta);
    fprintf(fout, "ntopics=%d\n", K);
    fprintf(fout, "ndocs=%d\n", newM);
    fprintf(fout, "nwords=%d\n", newV);
    fprintf(fout, "liter=%d\n", inf_liter);
    
    fclose(fout);    
    
    return 0;
}

int model::save_inf_model_twords(string filename) {
    FILE * fout = fopen(filename.c_str(), "w");
    if (!fout) {
	printf("Cannot open file %s to save!\n", filename.c_str());
	return 1;
    }
    
    if (twords > newV) {
	twords = newV;
    }
    mapid2word::iterator it;
    map<int, int>::iterator _it;
    
    for (int k = 0; k < K; k++) {
	vector<pair<int, double>> words_probs;
	pair<int, double> word_prob;
	for (int w = 0; w < newV; w++) {
	    word_prob.first = w;
	    word_prob.second = newphi[k][w];
	    words_probs.push_back(word_prob);
	}
    
        // quick sort to sort word-topic probability
	utils::quicksort(words_probs, 0, words_probs.size() - 1);
	
	fprintf(fout, "Topic %dth:\n", k);
	for (int i = 0; i < twords; i++) {
	    _it = pnewdata->_id2id.find(words_probs[i].first);
	    if (_it == pnewdata->_id2id.end()) {
		continue;
	    }
	    it = id2word.find(_it->second);
	    if (it != id2word.end()) {
		fprintf(fout, "\t%s   %f\n", (it->second).c_str(), words_probs[i].second);
	    }
	}
    }
    
    fclose(fout);    
    
    return 0;    
}

int model::init_inf() {
    // estimating the model from a previously estimated one
    int m, n, w, k;
    p = new double[K];
    // read new data for inference
    id2word.clear();
    pnewdata = new dataset;
    if (pnewdata->read_newdata(docs_words, &id2word)) 
    {
            printf("Fail to read new data!\n");
            return 1;
    }    
    
    newM = pnewdata->M;
    newV = pnewdata->V;
    
    newnw = new int*[newV];
    for (w = 0; w < newV; w++) {
        newnw[w] = new int[K];
        for (k = 0; k < K; k++) {
    	    newnw[w][k] = 0;
        }
    }
	
    newnd = new int*[newM];
    for (m = 0; m < newM; m++) {
        newnd[m] = new int[K];
        for (k = 0; k < K; k++) {
    	    newnd[m][k] = 0;
        }
    }
	
    newnwsum = new int[K];
    for (k = 0; k < K; k++) {
	newnwsum[k] = 0;
    }
    
    newndsum = new int[newM];
    for (m = 0; m < newM; m++) {
	newndsum[m] = 0;
    }

    srandom(time(0)); // initialize for random number generation
    newz = new int*[newM];
    for (m = 0; m < pnewdata->M; m++) {
	int N = pnewdata->docs[m]->length;
	newz[m] = new int[N];
	// assign values for nw, nd, nwsum, and ndsum	
        for (n = 0; n < N; n++) {
    	    int w = pnewdata->docs[m]->words[n];
    	    int _w = pnewdata->_docs[m]->words[n];
    	    int topic = (int)(((double)random() / RAND_MAX) * K); //随机生成1-k之间的topic id
            newz[m][n] = topic;
    	    // number of instances of word i assigned to topic j
    	    newnw[_w][topic] += 1;
    	    // number of words in document i assigned to topic j
    	    newnd[m][topic] += 1;
    	    // total number of words assigned to topic j
    	    newnwsum[topic] += 1;
        } 
        // total number of words in document i
        newndsum[m] = N;      
    }    
    
    newtheta = new double*[newM];
    for (m = 0; m < newM; m++) {
        newtheta[m] = new double[K];
    }
	
    newphi = new double*[K];
    for (k = 0; k < K; k++) {
        newphi[k] = new double[newV];
    }    
    
    return 0;        
}

void model::inference() {

    printf("Sampling %d iterations for inference!\n", niters);
    
    for (inf_liter = 1; inf_liter <= niters; inf_liter++) 
    {
        //printf("Iteration %d ...\n", inf_liter);
        
        // for all newz_i
        for (int m = 0; m < newM; m++) 
        {
            for (int n = 0; n < pnewdata->docs[m]->length; n++) 
            {
                int topic = inf_sampling(m, n);
                //printf("outer topic=%d\n", topic);
                newz[m][n] = topic;
            }
        }
    }
    
    printf("Gibbs sampling for inference completed!\n");
    printf("Saving the inference outputs!\n");
    compute_newtheta();
    compute_newphi();
    extract_prob_sent();
    inf_liter--;
    save_inf_model(dfile);
}


vector<int> model::Mult(int numb, std::vector<double> prob_array)
{
    const gsl_rng_type * T2;
    gsl_rng * r2;
    srand(time(NULL)); // srand(time(NULL));

    unsigned int Seed2 = 1234567; // rand();
    gsl_rng_env_setup();

    T2 = gsl_rng_default;
    r2 = gsl_rng_alloc (T2);
    gsl_rng_set (r2, Seed2);

    size_t k = prob_array.size();

    double ppp[k]; // Probability array

    for(int ii=0; ii<prob_array.size(); ++ii) {
        ppp[ii] =  prob_array[ii];
    }

    unsigned int mult_op[k];
    gsl_ran_multinomial(r2, k, numb, ppp, mult_op);
    std::vector<int> multi;
    for(int ii=0; ii<k; ++ii ){
        multi.push_back(mult_op[ii]);
    }
    return multi;
}

int model::inf_sampling(int m, int n) {
    // remove z_i from the count variables
    int topic = newz[m][n];
    int w = pnewdata->docs[m]->words[n];
    int _w = pnewdata->_docs[m]->words[n];
    newnw[_w][topic] -= 1;
    newnd[m][topic] -= 1;
    newnwsum[topic] -= 1;
    newndsum[m] -= 1;
    
    double Vbeta = newV * beta;
    double Kalpha = K * alpha;
    // do multinomial sampling via cumulative method
    for (int k = 0; k < K; k++) 
    {
    //p[k] = (nw[w][k] + newnw[_w][k] + beta) / (nwsum[k] + newnwsum[k] + Vbeta) *(newnd[m][k] + alpha) / (newndsum[m] + Kalpha);
	    
        p[k] = (newnw[_w][k] + beta) / (newnwsum[k] + Vbeta) * (newnd[m][k] + alpha) / (newndsum[m] + Kalpha);
    }
    // cumulate multinomial parameters
    double sum_p =0.0;
    for (int k = 0; k < K; k++)
        sum_p += p[k];
    vector<double>new_p;
    for(int k = 0; k < K; k++)
    {
        double tp = p[k]/sum_p;
        new_p.push_back(tp);
    }
    
    vector<int> counts = Mult(1, new_p);
    auto max_it = std::max_element(counts.begin(), counts.end());
    topic= std::distance(counts.begin(), max_it);

	// auto max_it = std::max_element(new_p.begin(), new_p.end());
    // topic= std::distance(new_p.begin(), max_it);    
    // scaled sample because of unnormalized p[]
    // double u = ((double)random() / RAND_MAX) * p[K - 1];
    // for (topic = 0; topic < K; topic++) 
    // {
    //     if (p[topic] > u) 
    //     {
    //         break;
    //     }
    // }
    
    // add newly estimated z_i to count variables
    newnw[_w][topic] += 1;
    newnd[m][topic] += 1;
    newnwsum[topic] += 1;
    newndsum[m] += 1;    
    return topic;
}

void model::compute_newtheta() {
    for (int m = 0; m < newM; m++) {
	for (int k = 0; k < K; k++) {
	    newtheta[m][k] = (newnd[m][k] + alpha) / (newndsum[m] + K * alpha);
	}
    }
}

void model::compute_newphi() {
    map<int, int>::iterator it;
    for (int k = 0; k < K; k++) {
	for (int w = 0; w < newV; w++) 
    {
	    it = pnewdata->_id2id.find(w);
	    if (it != pnewdata->_id2id.end()) {
		//newphi[k][w] = (nw[it->second][k] + newnw[w][k] + beta) / (nwsum[k] + newnwsum[k] + V * beta);
            newphi[k][w] = (newnw[w][k] + beta) / (newnwsum[k] + newV * beta);
	    }
	}
    }
}


bool model::compareByValue(const pair<int, double>&a, const pair<int, double>&b)
{
    return (double)a.second>(double)b.second;
}
    
void model::extract_prob_sent() 
{
    resId.clear();
    vector<pair<int, double>> sent_prob(newM);
    for (int m = 0; m < newM; m++) 
    {
	    double maxprob = 0.0;
        for (int k = 0; k < K; k++)
        {
	        if(newtheta[m][k]-maxprob>0.0)
                maxprob = newtheta[m][k];
	    }
        sent_prob[m].first = m;
        sent_prob[m].second = maxprob; 
    }
    sort(sent_prob.begin(), sent_prob.end(), compareByValue);
    for(int t=0;t<sentNum;++t)
        resId.push_back(sent_prob[t].first);
}
        

