# Introduction
Text Summary of Chinese with lda gibbs method C++ version

The whole process in this project:

1.Firsly, splitting the text into paragraphs and splitting the paragraghs into sentences.

2.Secondly, tokenizing words with jieba tokenizer.

3.Finally, specially inputting each sentence as a doc into lda model and extracting n key sentences by extracting the most-topic-likely n doc in doc-topic distribution.


# Requirements
install gsl in your env

# Compile&Bulid
./build-linux_x86.sh

# usage
example:

build/lda -dir your path -alpha 5.0 -beta 0.5 -ntopics 10 -niters 32 -twords 10 -dfile chinese_t2.dat -sentnum 6
-dfile is yout text data in -dir, and when running some written files were also saved in dir.
-alpha/-beta/-ntopics/-niters are the hyperparameters of gibbslda algorithm.Please refer to gibbslda algorithm.
-twords is the saved word number that you would like corresponding to each topic.
-sentnum is the extracted sentence number you expected




# Ackknowledgement

The code in this project is based on Gibblda++ (https://github.com/mrquincle/gibbs-lda)

and cppjieba(https://github.com/yanyiwu/cppjieba)

I would like to thank all predecessorfors sharing the code and significant ideas. 
