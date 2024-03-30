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

#ifndef	_MODEL_H
#define	_MODEL_H

//#include "constants.h"
#include "dataset.h"

using namespace std;

// LDA model
class model {
public:
    // fixed options
    string twords_suffix;	// suffix for file containing words-per-topics
    string dir;			    // model directory
    string dfile;		    // data file
    int sentNum;            // sentence number set by user


    vector<vector<string>> docs_words;
    dataset *pnewdata; // pointer to new dataset object
    mapid2word id2word; // word map [int => string]
    
    // --- model parameters and variables ---    
    int M; // dataset size (i.e., number of docs)
    int K; // number of topics
    double alpha, beta; // LDA hyperparameters 
    int niters; // number of Gibbs sampling iterations
    int twords; // print out top words per each topic

    double* p; // temp variable for sampling
    // int ** z; // topic assignments for words, size M x doc.size()
    // int ** nw; // cwt[i][j]: number of instances of word/term i assigned to topic j, size V x K
    // int ** nd; // na[i][j]: number of words in document i assigned to topic j, size M x K
    // int * nwsum; // nwsum[j]: total number of words assigned to topic j, size K
    // int * ndsum; // nasum[i]: total number of words in document i, size M
    // double ** theta; // theta: document-topic distributions, size M x K
    // double ** phi; // phi: topic-word distributions, size K x V
    
    // for inference only
    int inf_liter;
    int newM;
    int newV;
    int ** newz;
    int ** newnw;
    int ** newnd;
    int * newnwsum;
    int * newndsum;
    double ** newtheta;
    double ** newphi;

    
    vector<int>resId;
    // --------------------------------------
    
    model() 
    {
	    set_default_values();
    }
          
    ~model();
    
    // set default values for variables
    void set_default_values();   

    // parse command line to get options
    //int parse_args(int argc, char ** argv);
    
    // initialize the model
    int init(int argc, char ** argv, vector<vector<string>>& filtered_doc_words);

    
    // saving inference outputs
    int save_inf_model(string model_name);
    int save_inf_model_tassign(string filename);
    int save_inf_model_newtheta(string filename);
    int save_inf_model_newphi(string filename);
    int save_inf_model_others(string filename);
    int save_inf_model_twords(string filename);
    

    
    // init for inference
    int init_inf();
    // inference for new (unseen) data based on the estimated LDA model
    void inference();
    vector<int> Mult(int numb, std::vector<double> prob_array);
    int inf_sampling(int m, int n);
    void compute_newtheta();
    void compute_newphi();
    static bool compareByValue(const pair<int, double>&a, const pair<int, double>&b);
    void extract_prob_sent();
};

#endif

