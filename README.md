# Introduction
Text Summary of Chinese with lda gibbs method C++ version
The whole process in this project:
1.Firsly, splitting the text into paragraphs and splitting the paragraghs into sentences.
2.Secondly, tokenizing words with jieba tokenizer.
4.Thirdly, specially, inputting each sentence as a doc into lda model and extracting n key sentences by extracting the most-topic-likely n doc in doc-topic distribution.
