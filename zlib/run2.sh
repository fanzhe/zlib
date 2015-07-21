#!/bin/sh
# for ICDE response
# livej 512
#./zlib.exe g data/livej/livej.degree 1 data/livej/livej.random.512 512

./zlib.exe qbfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_bfs_q2 1000 2
./zlib.exe qbfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_bfs_q3 1000 3
./zlib.exe qbfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_bfs_q4 1000 4
./zlib.exe qbfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_bfs_q5 1000 5
./zlib.exe qbfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_bfs_q6 1000 6
./zlib.exe qbfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_bfs_q7 1000 7
./zlib.exe qbfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_bfs_q8 1000 8

./zlib.exe qdfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_dfs_q2 1000 2
./zlib.exe qdfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_dfs_q3 1000 3
./zlib.exe qdfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_dfs_q4 1000 4
./zlib.exe qdfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_dfs_q5 1000 5
./zlib.exe qdfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_dfs_q6 1000 6
./zlib.exe qdfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_dfs_q7 1000 7
./zlib.exe qdfs data/livej/livej.random.512 1 data/livej/query_random_512/livej_dfs_q8 1000 8

./zlib.exe r data/livej/query_random_512/livej_bfs_q2 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_bfs_q2.detailed data/livej/result_random_512/livej_bfs_q2.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_bfs_q3 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_bfs_q3.detailed data/livej/result_random_512/livej_bfs_q3.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_bfs_q4 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_bfs_q4.detailed data/livej/result_random_512/livej_bfs_q4.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_bfs_q5 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_bfs_q5.detailed data/livej/result_random_512/livej_bfs_q5.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_bfs_q6 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_bfs_q6.detailed data/livej/result_random_512/livej_bfs_q6.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_bfs_q7 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_bfs_q7.detailed data/livej/result_random_512/livej_bfs_q7.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_bfs_q8 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_bfs_q8.detailed data/livej/result_random_512/livej_bfs_q8.avg 7 100

./zlib.exe r data/livej/query_random_512/livej_dfs_q2 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_dfs_q2.detailed data/livej/result_random_512/livej_dfs_q2.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_dfs_q3 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_dfs_q3.detailed data/livej/result_random_512/livej_dfs_q3.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_dfs_q4 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_dfs_q4.detailed data/livej/result_random_512/livej_dfs_q4.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_dfs_q5 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_dfs_q5.detailed data/livej/result_random_512/livej_dfs_q5.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_dfs_q6 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_dfs_q6.detailed data/livej/result_random_512/livej_dfs_q6.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_dfs_q7 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_dfs_q7.detailed data/livej/result_random_512/livej_dfs_q7.avg 7 100
./zlib.exe r data/livej/query_random_512/livej_dfs_q8 100 data/livej/livej.random.512 1 data/livej/result_random_512/livej_dfs_q8.detailed data/livej/result_random_512/livej_dfs_q8.avg 7 100

#./zlib.exe r data/dblp/query/dblp_bfs_q2 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q2.detailed data/dblp/result/dblp_bfs_q2.avg
#./zlib.exe r data/dblp/query/dblp_bfs_q3 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q3.detailed data/dblp/result/dblp_bfs_q3.avg
#./zlib.exe r data/dblp/query/dblp_bfs_q4 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q4.detailed data/dblp/result/dblp_bfs_q4.avg
#./zlib.exe r data/dblp/query/dblp_bfs_q5 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q5.detailed data/dblp/result/dblp_bfs_q5.avg
#./zlib.exe r data/dblp/query/dblp_bfs_q6 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q6.detailed data/dblp/result/dblp_bfs_q6.avg

#./zlib.exe r data/dblp/query/dblp_dfs_q2 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q2.detailed data/dblp/result/dblp_dfs_q2.avg
#./zlib.exe r data/dblp/query/dblp_dfs_q3 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q3.detailed data/dblp/result/dblp_dfs_q3.avg
#./zlib.exe r data/dblp/query/dblp_dfs_q4 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q4.detailed data/dblp/result/dblp_dfs_q4.avg
#./zlib.exe r data/dblp/query/dblp_dfs_q5 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q5.detailed data/dblp/result/dblp_dfs_q5.avg
#./zlib.exe r data/dblp/query/dblp_dfs_q6 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q6.detailed data/dblp/result/dblp_dfs_q6.avg

#./zlib.exe r data/amazon/query/amazon_bfs_q2 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q2.detailed data/amazon/result/amazon_bfs_q2.avg
#./zlib.exe r data/amazon/query/amazon_bfs_q3 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q3.detailed data/amazon/result/amazon_bfs_q3.avg
#./zlib.exe r data/amazon/query/amazon_bfs_q4 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q4.detailed data/amazon/result/amazon_bfs_q4.avg
#./zlib.exe r data/amazon/query/amazon_bfs_q5 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q5.detailed data/amazon/result/amazon_bfs_q5.avg
#./zlib.exe r data/amazon/query/amazon_bfs_q6 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q6.detailed data/amazon/result/amazon_bfs_q6.avg

#./zlib.exe r data/amazon/query/amazon_dfs_q2 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q2.detailed data/amazon/result/amazon_dfs_q2.avg
#./zlib.exe r data/amazon/query/amazon_dfs_q3 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q3.detailed data/amazon/result/amazon_dfs_q3.avg
#./zlib.exe r data/amazon/query/amazon_dfs_q4 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q4.detailed data/amazon/result/amazon_dfs_q4.avg
#./zlib.exe r data/amazon/query/amazon_dfs_q5 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q5.detailed data/amazon/result/amazon_dfs_q5.avg
#./zlib.exe r data/amazon/query/amazon_dfs_q6 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q6.detailed data/amazon/result/amazon_dfs_q6.avg

#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q2 1000 2
#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q3 1000 3 
#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q4 1000 4
#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q5 1000 5
#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q6 1000 6
#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q7 1000 7

#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q2 1000 2
#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q3 1000 3
#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q4 1000 4
#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q5 1000 5
#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q6 1000 6
#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q7 1000 7

#./zlib.exe r data/youtube/query/youtube_bfs_q2 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q2.detailed data/youtube/result/youtube_bfs_q2.avg
#./zlib.exe r data/youtube/query/youtube_bfs_q3 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q3.detailed data/youtube/result/youtube_bfs_q3.avg
#./zlib.exe r data/youtube/query/youtube_bfs_q4 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q4.detailed data/youtube/result/youtube_bfs_q4.avg
#./zlib.exe r data/youtube/query/youtube_bfs_q5 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q5.detailed data/youtube/result/youtube_bfs_q5.avg
#./zlib.exe r data/youtube/query/youtube_bfs_q6 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q6.detailed data/youtube/result/youtube_bfs_q6.avg

#./zlib.exe r data/youtube/query/youtube_dfs_q2 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q2.detailed data/youtube/result/youtube_dfs_q2.avg
#./zlib.exe r data/youtube/query/youtube_dfs_q3 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q3.detailed data/youtube/result/youtube_dfs_q3.avg
#./zlib.exe r data/youtube/query/youtube_dfs_q4 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q4.detailed data/youtube/result/youtube_dfs_q4.avg
#./zlib.exe r data/youtube/query/youtube_dfs_q5 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q5.detailed data/youtube/result/youtube_dfs_q5.avg
#./zlib.exe r data/youtube/query/youtube_dfs_q6 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q6.detailed data/youtube/result/youtube_dfs_q6.avg

#./zlib.exe r data/dblp/query/dblp_bfs_q7 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q7.detailed data/dblp/result/dblp_bfs_q7.avg
#./zlib.exe r data/dblp/query/dblp_dfs_q7 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q7.detailed data/dblp/result/dblp_dfs_q7.avg
#./zlib.exe r data/amazon/query/amazon_bfs_q7 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q7.detailed data/amazon/result/amazon_bfs_q7.avg
#./zlib.exe r data/amazon/query/amazon_dfs_q7 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q7.detailed data/amazon/result/amazon_dfs_q7.avg
#./zlib.exe r data/youtube/query/youtube_bfs_q7 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q7.detailed data/youtube/result/youtube_bfs_q7.avg
#./zlib.exe r data/youtube/query/youtube_dfs_q7 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q7.detailed data/youtube/result/youtube_dfs_q7.avg

#./zlib.exe qbfs data/dblp/dblp.degree 1 data/dblp/query/dblp_bfs_q8 1000 8
#./zlib.exe qbfs data/dblp/dblp.degree 1 data/dblp/query/dblp_bfs_q9 1000 9
#./zlib.exe qbfs data/dblp/dblp.degree 1 data/dblp/query/dblp_bfs_q10 1000 10
#./zlib.exe qdfs data/dblp/dblp.degree 1 data/dblp/query/dblp_dfs_q8 1000 8
#./zlib.exe qdfs data/dblp/dblp.degree 1 data/dblp/query/dblp_dfs_q9 1000 9
#./zlib.exe qdfs data/dblp/dblp.degree 1 data/dblp/query/dblp_dfs_q10 1000 10

#./zlib.exe qbfs data/amazon/amazon.degree 1 data/amazon/query/amazon_bfs_q8 1000 8
#./zlib.exe qbfs data/amazon/amazon.degree 1 data/amazon/query/amazon_bfs_q9 1000 9
#./zlib.exe qbfs data/amazon/amazon.degree 1 data/amazon/query/amazon_bfs_q10 1000 10
#./zlib.exe qdfs data/amazon/amazon.degree 1 data/amazon/query/amazon_dfs_q8 1000 8
#./zlib.exe qdfs data/amazon/amazon.degree 1 data/amazon/query/amazon_dfs_q9 1000 9
#./zlib.exe qdfs data/amazon/amazon.degree 1 data/amazon/query/amazon_dfs_q10 1000 10

#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q8 1000 8
#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q9 1000 9
#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q10 1000 10
#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q8 1000 8
#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q9 1000 9
#./zlib.exe qdfs data/youtube/youtube.degree 1 data/youtube/query/youtube_dfs_q10 1000 10

#./zlib.exe qbfs data/youtube/youtube.degree 1 data/youtube/query/youtube_bfs_q5 1000 5

#./zlib.exe r data/dblp/query/dblp_bfs_q8 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q8.detailed data/dblp/result/dblp_bfs_q8.avg
#./zlib.exe r data/dblp/query/dblp_dfs_q8 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q8.detailed data/dblp/result/dblp_dfs_q8.avg
#./zlib.exe r data/amazon/query/amazon_bfs_q8 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q8.detailed data/amazon/result/amazon_bfs_q8.avg
#./zlib.exe r data/amazon/query/amazon_dfs_q8 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q8.detailed data/amazon/result/amazon_dfs_q8.avg
#./zlib.exe r data/youtube/query/youtube_bfs_q8 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q8.detailed data/youtube/result/youtube_bfs_q8.avg
#./zlib.exe r data/youtube/query/youtube_dfs_q8 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q8.detailed data/youtube/result/youtube_dfs_q8.avg

#./zlib.exe r data/dblp/query/dblp_bfs_q9 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q9.detailed data/dblp/result/dblp_bfs_q9.avg
#./zlib.exe r data/dblp/query/dblp_dfs_q9 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q9.detailed data/dblp/result/dblp_dfs_q9.avg
#./zlib.exe r data/amazon/query/amazon_bfs_q9 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q9.detailed data/amazon/result/amazon_bfs_q9.avg
#./zlib.exe r data/amazon/query/amazon_dfs_q9 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q9.detailed data/amazon/result/amazon_dfs_q9.avg
#./zlib.exe r data/youtube/query/youtube_bfs_q9 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q9.detailed data/youtube/result/youtube_bfs_q9.avg
#./zlib.exe r data/youtube/query/youtube_dfs_q9 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q9.detailed data/youtube/result/youtube_dfs_q9.avg

#./zlib.exe r data/dblp/query/dblp_bfs_q10 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_bfs_q10.detailed data/dblp/result/dblp_bfs_q10.avg
#./zlib.exe r data/dblp/query/dblp_dfs_q10 1000 data/dblp/dblp.degree 1 data/dblp/result/dblp_dfs_q10.detailed data/dblp/result/dblp_dfs_q10.avg
#./zlib.exe r data/amazon/query/amazon_bfs_q10 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_bfs_q10.detailed data/amazon/result/amazon_bfs_q10.avg
#./zlib.exe r data/amazon/query/amazon_dfs_q10 1000 data/amazon/amazon.degree 1 data/amazon/result/amazon_dfs_q10.detailed data/amazon/result/amazon_dfs_q10.avg
#./zlib.exe r data/youtube/query/youtube_bfs_q10 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_bfs_q10.detailed data/youtube/result/youtube_bfs_q10.avg
#./zlib.exe r data/youtube/query/youtube_dfs_q10 1000 data/youtube/youtube.degree 1 data/youtube/result/youtube_dfs_q10.detailed data/youtube/result/youtube_dfs_q10.avg

