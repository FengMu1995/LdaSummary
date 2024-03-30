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
#include <stdlib.h>
// #include "constants.h"
// #include "strtokenizer.h"
#include "dataset.h"

using namespace std;

int dataset::write_wordmap(string wordmapfile, mapword2id * pword2id) {
    FILE * fout = fopen(wordmapfile.c_str(), "w");
    if (!fout) {
	printf("Cannot open file %s to write!\n", wordmapfile.c_str());
	return 1;
    }    
    
    mapword2id::iterator it;
    fprintf(fout, "%d\n", pword2id->size());
    for (it = pword2id->begin(); it != pword2id->end(); it++) {
	fprintf(fout, "%s %d\n", (it->first).c_str(), it->second);
    }
    
    fclose(fout);
    
    return 0;
}


int dataset::read_newdata(const vector<vector<string>>& new_docs, mapid2word* pid2word)
{
	_id2id.clear();
	mapword2id word2id;
    map<int, int> id2_id;
    
	mapword2id::iterator it;
    map<int, int>::iterator _it;

	M = new_docs.size();
	if(M<=0)
	{
		printf("No document available!\n");
		return 1;
	}
	if (docs)
		deallocate();
    else
		docs = new document*[M];
    _docs = new document*[M];

	V = 0;
	int tmpid = 0;
	for (int i = 0; i < M; i++) 
	{
		vector<int> doc;
		vector<int> _doc;
		for (int j = 0; j < new_docs[i].size(); j++) 
		{
			int _id;
			it = word2id.find(new_docs[i][j]);
			if (it == word2id.end())
			{
				word2id.insert(pair<string, int>(new_docs[i][j], tmpid));
				pid2word->insert(pair<int, string>(tmpid, new_docs[i][j]));
				id2_id.insert(pair<int, int>(tmpid, tmpid));
				_id2id.insert(pair<int, int>(tmpid, tmpid));
				doc.push_back(tmpid);
				_doc.push_back(tmpid);
				++tmpid;
			}
			else
			{
				doc.push_back(it->second);
				_doc.push_back(it->second);
			}	

		}
		// allocate memory for new doc
		document * pdoc = new document(doc);
		document * _pdoc = new document(_doc);
		// add new doc
		add_doc(pdoc, i);
		_add_doc(_pdoc, i);
    }
	V = id2_id.size();
	return 0;
}


