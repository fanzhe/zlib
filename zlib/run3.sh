#!/bin/sh

#./zlib.exe qbfs data/yeast/yeast.my 1 data/yeast/query/yeast_bfs_q2 1000 2
#./zlib.exe qbfs data/yeast/yeast.my 1 data/yeast/query/yeast_bfs_q3 1000 3
#./zlib.exe qbfs data/yeast/yeast.my 1 data/yeast/query/yeast_bfs_q4 1000 4
#./zlib.exe qbfs data/yeast/yeast.my 1 data/yeast/query/yeast_bfs_q5 1000 5
#./zlib.exe qbfs data/yeast/yeast.my 1 data/yeast/query/yeast_bfs_q6 1000 6
#./zlib.exe qbfs data/yeast/yeast.my 1 data/yeast/query/yeast_bfs_q7 1000 7
#./zlib.exe qbfs data/yeast/yeast.my 1 data/yeast/query/yeast_bfs_q8 1000 8

#./zlib.exe qdfs data/yeast/yeast.my 1 data/yeast/query/yeast_dfs_q2 1000 2
#./zlib.exe qdfs data/yeast/yeast.my 1 data/yeast/query/yeast_dfs_q3 1000 3
#./zlib.exe qdfs data/yeast/yeast.my 1 data/yeast/query/yeast_dfs_q4 1000 4
#./zlib.exe qdfs data/yeast/yeast.my 1 data/yeast/query/yeast_dfs_q5 1000 5
#./zlib.exe qdfs data/yeast/yeast.my 1 data/yeast/query/yeast_dfs_q6 1000 6
#./zlib.exe qdfs data/yeast/yeast.my 1 data/yeast/query/yeast_dfs_q7 1000 7
#./zlib.exe qdfs data/yeast/yeast.my 1 data/yeast/query/yeast_dfs_q8 1000 8

#./zlib.exe qbfs data/human/human.my 1 data/human/query/human_bfs_q2 1000 2
#./zlib.exe qbfs data/human/human.my 1 data/human/query/human_bfs_q3 1000 3
#./zlib.exe qbfs data/human/human.my 1 data/human/query/human_bfs_q4 1000 4
#./zlib.exe qbfs data/human/human.my 1 data/human/query/human_bfs_q5 1000 5
#./zlib.exe qbfs data/human/human.my 1 data/human/query/human_bfs_q6 1000 6
#./zlib.exe qbfs data/human/human.my 1 data/human/query/human_bfs_q7 1000 7
#./zlib.exe qbfs data/human/human.my 1 data/human/query/human_bfs_q8 1000 8

#./zlib.exe qdfs data/human/human.my 1 data/human/query/human_dfs_q2 1000 2
#./zlib.exe qdfs data/human/human.my 1 data/human/query/human_dfs_q3 1000 3
#./zlib.exe qdfs data/human/human.my 1 data/human/query/human_dfs_q4 1000 4
#./zlib.exe qdfs data/human/human.my 1 data/human/query/human_dfs_q5 1000 5
#./zlib.exe qdfs data/human/human.my 1 data/human/query/human_dfs_q6 1000 6
#./zlib.exe qdfs data/human/human.my 1 data/human/query/human_dfs_q7 1000 7
#./zlib.exe qdfs data/human/human.my 1 data/human/query/human_dfs_q8 1000 8

#./zlib.exe r data/yeast/query/yeast_bfs_q2 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_bfs_q2.detailed data/yeast/result/yeast_bfs_q2.avg
#./zlib.exe r data/yeast/query/yeast_bfs_q3 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_bfs_q3.detailed data/yeast/result/yeast_bfs_q3.avg
#./zlib.exe r data/yeast/query/yeast_bfs_q4 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_bfs_q4.detailed data/yeast/result/yeast_bfs_q4.avg
#./zlib.exe r data/yeast/query/yeast_bfs_q5 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_bfs_q5.detailed data/yeast/result/yeast_bfs_q5.avg
#./zlib.exe r data/yeast/query/yeast_bfs_q6 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_bfs_q6.detailed data/yeast/result/yeast_bfs_q6.avg

#./zlib.exe r data/yeast/query/yeast_dfs_q2 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_dfs_q2.detailed data/yeast/result/yeast_dfs_q2.avg
#./zlib.exe r data/yeast/query/yeast_dfs_q3 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_dfs_q3.detailed data/yeast/result/yeast_dfs_q3.avg
#./zlib.exe r data/yeast/query/yeast_dfs_q4 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_dfs_q4.detailed data/yeast/result/yeast_dfs_q4.avg
#./zlib.exe r data/yeast/query/yeast_dfs_q5 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_dfs_q5.detailed data/yeast/result/yeast_dfs_q5.avg
#./zlib.exe r data/yeast/query/yeast_dfs_q6 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_dfs_q6.detailed data/yeast/result/yeast_dfs_q6.avg

#./zlib.exe r data/human/query/human_bfs_q2 1000 data/human/human.my 1 data/human/result/human_bfs_q2.detailed data/human/result/human_bfs_q2.avg
#./zlib.exe r data/human/query/human_bfs_q3 1000 data/human/human.my 1 data/human/result/human_bfs_q3.detailed data/human/result/human_bfs_q3.avg
#./zlib.exe r data/human/query/human_bfs_q4 1000 data/human/human.my 1 data/human/result/human_bfs_q4.detailed data/human/result/human_bfs_q4.avg
#./zlib.exe r data/human/query/human_bfs_q5 1000 data/human/human.my 1 data/human/result/human_bfs_q5.detailed data/human/result/human_bfs_q5.avg
#./zlib.exe r data/human/query/human_bfs_q6 1000 data/human/human.my 1 data/human/result/human_bfs_q6.detailed data/human/result/human_bfs_q6.avg

#./zlib.exe r data/human/query/human_dfs_q2 1000 data/human/human.my 1 data/human/result/human_dfs_q2.detailed data/human/result/human_dfs_q2.avg
#./zlib.exe r data/human/query/human_dfs_q3 1000 data/human/human.my 1 data/human/result/human_dfs_q3.detailed data/human/result/human_dfs_q3.avg
#./zlib.exe r data/human/query/human_dfs_q4 1000 data/human/human.my 1 data/human/result/human_dfs_q4.detailed data/human/result/human_dfs_q4.avg
./zlib.exe r data/human/query/human_dfs_q5 1000 data/human/human.my 1 data/human/result/human_dfs_q5.detailed data/human/result/human_dfs_q5.avg
./zlib.exe r data/human/query/human_dfs_q6 1000 data/human/human.my 1 data/human/result/human_dfs_q6.detailed data/human/result/human_dfs_q6.avg

./zlib.exe r data/human/query/human_bfs_q7 1000 data/human/human.my 1 data/human/result/human_bfs_q7.detailed data/human/result/human_bfs_q7.avg
./zlib.exe r data/human/query/human_bfs_q8 1000 data/human/human.my 1 data/human/result/human_bfs_q8.detailed data/human/result/human_bfs_q8.avg
./zlib.exe r data/yeast/query/yeast_dfs_q7 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_dfs_q7.detailed data/yeast/result/yeast_dfs_q7.avg
./zlib.exe r data/yeast/query/yeast_dfs_q8 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_dfs_q8.detailed data/yeast/result/yeast_dfs_q8.avg
./zlib.exe r data/yeast/query/yeast_bfs_q7 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_bfs_q7.detailed data/yeast/result/yeast_bfs_q7.avg
./zlib.exe r data/yeast/query/yeast_bfs_q8 1000 data/yeast/yeast.my 1 data/yeast/result/yeast_bfs_q8.detailed data/yeast/result/yeast_bfs_q8.avg
./zlib.exe r data/human/query/human_dfs_q7 1000 data/human/human.my 1 data/human/result/human_dfs_q7.detailed data/human/result/human_dfs_q7.avg
./zlib.exe r data/human/query/human_dfs_q8 1000 data/human/human.my 1 data/human/result/human_dfs_q8.detailed data/human/result/human_dfs_q8.avg

