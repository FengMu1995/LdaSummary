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

#include <stdio.h>
#include <string>
#include <map>
// #include "strtokenizer.h"
#include "utils.h"
#include "model.h"

using namespace std;

int utils::parse_args(int argc, char ** argv, const vector<vector<string>>& filtered_doc_words, model *pmodel) {
    string dir = "";
    string dfile = "";

    double alpha = -1.0;
    double beta = -1.0;
    int K = 0;
    int niters = 0;
    int twords = 0;
	int sentnum = 0;

    int i = 0; 
    while (i < argc) 
	{
		string arg = argv[i];
		if (arg == "-dir")
			dir = argv[++i];	    
		else if (arg == "-dfile")
			dfile = argv[++i];	    
		else if (arg == "-alpha") 
			alpha = atof(argv[++i]);	    
		else if (arg == "-beta")
			beta = atof(argv[++i]);	    
		else if (arg == "-ntopics")
			K = atoi(argv[++i]);	    
		else if (arg == "-niters")
			niters = atoi(argv[++i]);	    
		else if (arg == "-twords")
			twords = atoi(argv[++i]);
		else if (arg == "-sentnum")
			sentnum = atoi(argv[++i]);
		else 
		{
			// any more?
		}	
		i++;
    }
    
	if (dir == "") 
	{
		printf("Please specify model directory please!\n");
		return 1;
	}
	if (dfile == "") {
		printf("Please specify the new data file for inference!\n");
		return 1;
	}
	if (dir[dir.size() - 1] != '/') {
		dir += "/";
	}
	pmodel->dir = dir;
	pmodel->dfile = dfile;
	if (K > 0) {
		pmodel->K = K;
	}
	if (alpha > 0) {
		pmodel->alpha = alpha;
	}
	if (beta > 0) {
		pmodel->beta = beta;
	}


	if (niters > 0) {
		pmodel->niters = niters;
	} else {
		// default number of Gibbs sampling iterations for doing inference
		pmodel->niters = 20;
	}
	if (twords > 0) {
		pmodel->twords = twords;
	}
	if (sentnum > 0) {
		pmodel->sentNum = sentnum;
	}
	pmodel->docs_words = filtered_doc_words;
    return 0;
}

void utils::sort(vector<double> & probs, vector<int> & words) {
    for (int i = 0; i < probs.size() - 1; i++) {
	for (int j = i + 1; j < probs.size(); j++) {
	    if (probs[i] < probs[j]) {
		double tempprob = probs[i];
		int tempword = words[i];
		probs[i] = probs[j];
		words[i] = words[j];
		probs[j] = tempprob;
		words[j] = tempword;
	    }
	}
    }
}

void utils::quicksort(vector<pair<int, double> > & vect, int left, int right) {
    int l_hold, r_hold;
    pair<int, double> pivot;
    
    l_hold = left;
    r_hold = right;    
    int pivotidx = left;
    pivot = vect[pivotidx];

    while (left < right) {
	while (vect[right].second <= pivot.second && left < right) {
	    right--;
	}
	if (left != right) {
	    vect[left] = vect[right];
	    left++;
	}
	while (vect[left].second >= pivot.second && left < right) {
	    left++;
	}
	if (left != right) {
	    vect[right] = vect[left];
	    right--;
	}
    }

    vect[left] = pivot;
    pivotidx = left;
    left = l_hold;
    right = r_hold;
    
    if (left < pivotidx) {
	quicksort(vect, left, pivotidx - 1);
    }
    if (right > pivotidx) {
	quicksort(vect, pivotidx + 1, right);
    }    
}

