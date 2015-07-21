#!/bin/sh
# for ICDE response
# DBLP 128
#./zlib.exe g data/dblp/dblp.degree 1 data/dblp/dblp.random.64 64

./zlib.exe qbfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_bfs_q2 1000 2
./zlib.exe qbfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_bfs_q3 1000 3
./zlib.exe qbfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_bfs_q4 1000 4
./zlib.exe qbfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_bfs_q5 1000 5
./zlib.exe qbfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_bfs_q6 1000 6
./zlib.exe qbfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_bfs_q7 1000 7
./zlib.exe qbfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_bfs_q8 1000 8

./zlib.exe qdfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_dfs_q2 1000 2
./zlib.exe qdfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_dfs_q3 1000 3
./zlib.exe qdfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_dfs_q4 1000 4
./zlib.exe qdfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_dfs_q5 1000 5
./zlib.exe qdfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_dfs_q6 1000 6
./zlib.exe qdfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_dfs_q7 1000 7
./zlib.exe qdfs data/dblp/dblp.random.64 1 data/dblp/query_random_64/dblp_dfs_q8 1000 8

./zlib.exe r data/dblp/query_random_64/dblp_bfs_q2 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_bfs_q2.detailed data/dblp/result_random_64/dblp_bfs_q2.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_bfs_q3 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_bfs_q3.detailed data/dblp/result_random_64/dblp_bfs_q3.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_bfs_q4 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_bfs_q4.detailed data/dblp/result_random_64/dblp_bfs_q4.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_bfs_q5 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_bfs_q5.detailed data/dblp/result_random_64/dblp_bfs_q5.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_bfs_q6 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_bfs_q6.detailed data/dblp/result_random_64/dblp_bfs_q6.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_bfs_q7 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_bfs_q7.detailed data/dblp/result_random_64/dblp_bfs_q7.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_bfs_q8 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_bfs_q8.detailed data/dblp/result_random_64/dblp_bfs_q8.avg 7 100

./zlib.exe r data/dblp/query_random_64/dblp_dfs_q2 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_dfs_q2.detailed data/dblp/result_random_64/dblp_dfs_q2.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_dfs_q3 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_dfs_q3.detailed data/dblp/result_random_64/dblp_dfs_q3.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_dfs_q4 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_dfs_q4.detailed data/dblp/result_random_64/dblp_dfs_q4.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_dfs_q5 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_dfs_q5.detailed data/dblp/result_random_64/dblp_dfs_q5.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_dfs_q6 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_dfs_q6.detailed data/dblp/result_random_64/dblp_dfs_q6.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_dfs_q7 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_dfs_q7.detailed data/dblp/result_random_64/dblp_dfs_q7.avg 7 100
./zlib.exe r data/dblp/query_random_64/dblp_dfs_q8 100 data/dblp/dblp.random.64 1 data/dblp/result_random_64/dblp_dfs_q8.detailed data/dblp/result_random_64/dblp_dfs_q8.avg 7 100
