#!/bin/bash

#params




rm -f ../results/latency.csv #will be created by ./simulator
rm -f  ../results/logs_f.txt

touch  ../results/logs_f.txt

# for scenario in 0 1 2 ; do
#     for n in 4 ; do
#         for seed in 0 1 2 3 4 5 6 7 8 9 ; do
#             for malicious in 0 ; do
#                 for commission in 10 ; do
#                     for duration in 1000 ; do
#                         ../model/./simulator $n $scenario $seed $malicious $commission $duration | tee -a ../results/logs_f.txt
#                     done
#                 done
#             done
#         done
#     done
# done

# for scenario in 2 ; do
#     for n in 4 ; do
#         for seed in 0 1 2 3 4 5 6 7 8 9 ; do
#             for malicious in 0 ; do
#                 for commission in 5 10 15 20 25 ; do
#                     for duration in 1000 ; do
#                         ../model/./simulator $n $scenario $seed $malicious $commission $duration | tee -a ../results/logs_f.txt
#                     done
#                 done
#             done
#         done
#     done
# done


for scenario in 1 ; do
    for n in 4 10 16 22 28 ; do
        for malicious in 1 ; do
            for commission in 10 ; do
                for duration in 1000 ; do
                    for seed in 0 1 2 3 4 ; do
                        ../model/./simulator $n $scenario $seed $malicious $commission $duration | tee -a ../results/logs_f.txt
                    done
                done
            done
        done
    done
done


echo "f : done"


