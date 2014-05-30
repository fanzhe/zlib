#!/bin/sh

#./zlib.exe qbfs data/livej/livej.degree 1 data/livej/query/livej_bfs_q2 1000 2
#./zlib.exe qbfs data/livej/livej.degree 1 data/livej/query/livej_bfs_q3 1000 3
#./zlib.exe qbfs data/livej/livej.degree 1 data/livej/query/livej_bfs_q4 1000 4
#./zlib.exe qbfs data/livej/livej.degree 1 data/livej/query/livej_bfs_q5 1000 5
#./zlib.exe qbfs data/livej/livej.degree 1 data/livej/query/livej_bfs_q6 1000 6
#./zlib.exe qbfs data/livej/livej.degree 1 data/livej/query/livej_bfs_q7 1000 7
#./zlib.exe qbfs data/livej/livej.degree 1 data/livej/query/livej_bfs_q8 1000 8

#./zlib.exe qdfs data/livej/livej.degree 1 data/livej/query/livej_dfs_q2 1000 2
#./zlib.exe qdfs data/livej/livej.degree 1 data/livej/query/livej_dfs_q3 1000 3
#./zlib.exe qdfs data/livej/livej.degree 1 data/livej/query/livej_dfs_q4 1000 4
#./zlib.exe qdfs data/livej/livej.degree 1 data/livej/query/livej_dfs_q5 1000 5
#./zlib.exe qdfs data/livej/livej.degree 1 data/livej/query/livej_dfs_q6 1000 6
#./zlib.exe qdfs data/livej/livej.degree 1 data/livej/query/livej_dfs_q7 1000 7
#./zlib.exe qdfs data/livej/livej.degree 1 data/livej/query/livej_dfs_q8 1000 8


./zlib.exe r data/livej/query/livej_bfs_q2 1000 data/livej/livej.degree 1 data/livej/result/livej_bfs_q2.detailed data/livej/result/livej_bfs_q2.avg
./zlib.exe r data/livej/query/livej_bfs_q3 1000 data/livej/livej.degree 1 data/livej/result/livej_bfs_q3.detailed data/livej/result/livej_bfs_q3.avg
./zlib.exe r data/livej/query/livej_bfs_q4 1000 data/livej/livej.degree 1 data/livej/result/livej_bfs_q4.detailed data/livej/result/livej_bfs_q4.avg
#./zlib.exe r data/livej/query/livej_bfs_q5 1000 data/livej/livej.degree 1 data/livej/result/livej_bfs_q5.detailed data/livej/result/livej_bfs_q5.avg
./zlib.exe r data/livej/query/livej_bfs_q6 1000 data/livej/livej.degree 1 data/livej/result/livej_bfs_q6.detailed data/livej/result/livej_bfs_q6.avg
./zlib.exe r data/livej/query/livej_bfs_q7 1000 data/livej/livej.degree 1 data/livej/result/livej_bfs_q7.detailed data/livej/result/livej_bfs_q7.avg
./zlib.exe r data/livej/query/livej_bfs_q8 1000 data/livej/livej.degree 1 data/livej/result/livej_bfs_q8.detailed data/livej/result/livej_bfs_q8.avg

./zlib.exe r data/livej/query/livej_dfs_q2 1000 data/livej/livej.degree 1 data/livej/result/livej_dfs_q2.detailed data/livej/result/livej_dfs_q2.avg
./zlib.exe r data/livej/query/livej_dfs_q3 1000 data/livej/livej.degree 1 data/livej/result/livej_dfs_q3.detailed data/livej/result/livej_dfs_q3.avg
./zlib.exe r data/livej/query/livej_dfs_q4 1000 data/livej/livej.degree 1 data/livej/result/livej_dfs_q4.detailed data/livej/result/livej_dfs_q4.avg
./zlib.exe r data/livej/query/livej_dfs_q5 1000 data/livej/livej.degree 1 data/livej/result/livej_dfs_q5.detailed data/livej/result/livej_dfs_q5.avg
./zlib.exe r data/livej/query/livej_dfs_q6 1000 data/livej/livej.degree 1 data/livej/result/livej_dfs_q6.detailed data/livej/result/livej_dfs_q6.avg
./zlib.exe r data/livej/query/livej_dfs_q7 1000 data/livej/livej.degree 1 data/livej/result/livej_dfs_q7.detailed data/livej/result/livej_dfs_q7.avg
./zlib.exe r data/livej/query/livej_dfs_q8 1000 data/livej/livej.degree 1 data/livej/result/livej_dfs_q8.detailed data/livej/result/livej_dfs_q8.avg
