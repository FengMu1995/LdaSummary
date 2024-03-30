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
#include <iostream>
#include <sys/time.h>
#include <algorithm>
#include <regex>
#include <codecvt>

#include "cppjieba/Jieba.hpp"
#include "model.h"

using namespace std;


#define PUNCTUATION_ENG 	",.!?;\""
#define PUNCTUATION_CHN 	"，。！？；：、"
#define REGEXP_SYMBOL_ENG  (R"([,.!?;\"])")
#define REGEXP_SYMBOL_CHN  (L"([。！？；])") 

typedef enum{
	LANGUAGE_ENG,
	LANGUAGE_CHN,
	LANGUAGE_JAPAN,
	
}E_LANGUAGE_TYPE;

const char* const DICT_PATH = "cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "cppjieba/dict/stop_words.utf8";

void show_help();
double __get_us(struct timeval t) { return (t.tv_sec * 1000000 + t.tv_usec); }

string crop_blank_char(string input) 
{ 
	//去掉行首尾的空白符号
	string str = input; 
	if (!input.empty())
	{
		char blank_char[] = " 	\n\r";//其中包括空格键、tab键 、换行符、回车符
		int pos1 = input.find_first_not_of(blank_char);
		int pos2	= input.find_last_not_of(blank_char);
		//printf("input.size %ld, pos1 %d, pos2 %d\n", input.size(), pos1, pos2);		
		if((pos1 == 0) && (pos2 == input.size()-1))
		{
			
		}
		else if((pos1 == string::npos) || (pos2 == string::npos))
		{
			str = "";
		}
		else
		{	
			str.assign(input.begin() + pos1, input.begin() + pos2 + 1);			
		}
	}
	return str;  
}


wstring String2WString(string s)
{    
	wstring ws;
	if(!s.empty())
	{
		wstring_convert<codecvt_utf8<wchar_t>> converter;  
		ws = converter.from_bytes(s); 		
	}
    return ws;	
}

string WString2String(wstring ws)
{
	string s;
	if(!ws.empty())
	{
		wstring_convert<codecvt_utf8<wchar_t>> converter;  
		s = converter.to_bytes(ws);	
	}	
    return s;
}

int isPunctuation(string str, E_LANGUAGE_TYPE lang_type)
{
	string punct_set   = (lang_type == LANGUAGE_ENG) ? PUNCTUATION_ENG : PUNCTUATION_CHN;
	int one_punct_size = (lang_type == LANGUAGE_ENG) ? strlen(".") : strlen("。");
	//printf("a punctuation size %d Bytes\n", one_punct_size);		
	if((str.size() == one_punct_size) && (str.find_first_of(punct_set) != string::npos))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}





int main(int argc, char ** argv) {
    string filename = "your_own_path";///media/wudi/BAK/text_sum/LDA/LdaSummary/models/casestudy/chinese_t2.dat
	
	//tokenize the text into sentences
    char line[1000000];
    vector<string> sentences;
    FILE * fin = fopen(filename.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", filename.c_str());
	    return 1;
    }
    else
    {
        while(fgets(line, sizeof(line), fin)) 
        {
            string paragraph = line;
            paragraph = crop_blank_char(paragraph);
            wstring ws = String2WString(paragraph);
            wregex regexp_split_symbol(REGEXP_SYMBOL_CHN); 
            wsregex_token_iterator iter(ws.begin(), ws.end(), regexp_split_symbol, {-1,0});  
            wsregex_token_iterator end;  
            while (iter != end) 
	        {  
		        string s = WString2String(*iter++);
		        s = crop_blank_char(s);			
		        if(!s.empty())
		        {	
			       if(!isPunctuation(s, LANGUAGE_CHN))
						sentences.push_back(s);
		        }
            }  
        }
        fclose(fin);
    } 

	//tokenize the sentences into words
	struct timeval start_time, stop_time;
    gettimeofday(&start_time, NULL);
	vector<vector<string>> doc_words(sentences.size());
	vector<vector<string>> doc_filtered_words(sentences.size());
	cppjieba::Jieba jieba(DICT_PATH,
			HMM_PATH,
			USER_DICT_PATH,
			IDF_PATH,
			STOP_WORD_PATH);
	for(int i =0;i<sentences.size();++i)
		//jieba.CutAll(sentences[i], doc_words[i]);
		jieba.Cut(sentences[i], doc_words[i], true);
	gettimeofday(&stop_time, NULL);
    printf("total use %f ms\n", (__get_us(stop_time) - __get_us(start_time)) / 1000);
	//remove the stop words
	std::string filePath = STOP_WORD_PATH;
	ifstream ifs(filePath.c_str());
	string stopword;
	unordered_set<string>stop_word_set;
	while(getline(ifs, stopword))
		stop_word_set.insert(stopword);
	
	for(int i = 0;i<doc_words.size();++i)
	{
		for(int j = 0;j<doc_words[i].size();++j)
		{
			if(stop_word_set.find(doc_words[i][j])==stop_word_set.end())
				doc_filtered_words[i].push_back(doc_words[i][j]);
		}
	}
	
	
	
	model lda;
    if (lda.init(argc, argv, doc_filtered_words)) 
	{
		show_help();
		return 1;
    }
	
	// do inference
    
	lda.inference();
	
	int len = lda.resId.size();
	for(int i=0;i<len;++i)
	{
		auto min_it = std::min_element(lda.resId.begin(), lda.resId.end());
		cout<<sentences[*min_it]<<endl;
  		lda.resId.erase(min_it);
	}
   
	
    return 0;
}

void show_help() {
    printf("Command line usage:\n");
    printf("\tlda -est -alpha <double> -beta <double> -ntopics <int> -niters <int> -savestep <int> -twords <int> -dfile <string>\n");
    printf("\tlda -estc -dir <string> -model <string> -niters <int> -savestep <int> -twords <int>\n");
    printf("\tlda -inf -dir <string> -dfile <string> -alpha <float> -beta <float> -ntopics <int> -niters <int> -twords <int> -sentnum <int>\n");
    // printf("\tlda -inf -dir <string> -model <string> -niters <int> -twords <int> -dfile <string> -withrawdata\n");
}

