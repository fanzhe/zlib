#!/bin/sh
# for ICDE response
# livej 256
#./zlib.exe g data/livej/livej.degree 1 data/livej/livej.random.256 256

./zlib.exe qbfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_bfs_q2 1000 2
./zlib.exe qbfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_bfs_q3 1000 3
./zlib.exe qbfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_bfs_q4 1000 4
./zlib.exe qbfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_bfs_q5 1000 5
./zlib.exe qbfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_bfs_q6 1000 6
./zlib.exe qbfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_bfs_q7 1000 7
./zlib.exe qbfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_bfs_q8 1000 8

./zlib.exe qdfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_dfs_q2 1000 2
./zlib.exe qdfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_dfs_q3 1000 3
./zlib.exe qdfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_dfs_q4 1000 4
./zlib.exe qdfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_dfs_q5 1000 5
./zlib.exe qdfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_dfs_q6 1000 6
./zlib.exe qdfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_dfs_q7 1000 7
./zlib.exe qdfs data/livej/livej.random.256 1 data/livej/query_random_256/livej_dfs_q8 1000 8

./zlib.exe r data/livej/query_random_256/livej_bfs_q2 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_bfs_q2.detailed data/livej/result_random_256/livej_bfs_q2.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_bfs_q3 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_bfs_q3.detailed data/livej/result_random_256/livej_bfs_q3.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_bfs_q4 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_bfs_q4.detailed data/livej/result_random_256/livej_bfs_q4.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_bfs_q5 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_bfs_q5.detailed data/livej/result_random_256/livej_bfs_q5.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_bfs_q6 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_bfs_q6.detailed data/livej/result_random_256/livej_bfs_q6.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_bfs_q7 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_bfs_q7.detailed data/livej/result_random_256/livej_bfs_q7.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_bfs_q8 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_bfs_q8.detailed data/livej/result_random_256/livej_bfs_q8.avg 7 100

./zlib.exe r data/livej/query_random_256/livej_dfs_q2 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_dfs_q2.detailed data/livej/result_random_256/livej_dfs_q2.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_dfs_q3 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_dfs_q3.detailed data/livej/result_random_256/livej_dfs_q3.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_dfs_q4 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_dfs_q4.detailed data/livej/result_random_256/livej_dfs_q4.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_dfs_q5 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_dfs_q5.detailed data/livej/result_random_256/livej_dfs_q5.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_dfs_q6 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_dfs_q6.detailed data/livej/result_random_256/livej_dfs_q6.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_dfs_q7 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_dfs_q7.detailed data/livej/result_random_256/livej_dfs_q7.avg 7 100
./zlib.exe r data/livej/query_random_256/livej_dfs_q8 100 data/livej/livej.random.256 1 data/livej/result_random_256/livej_dfs_q8.detailed data/livej/result_random_256/livej_dfs_q8.avg 7 100
