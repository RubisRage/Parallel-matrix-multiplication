#!/usr/bin/env bash

tests="test250 test500 test750 test1 test1.5 test2 test2.5 test3 test3.5 test4 test4.5"
programs="cuda"

for p in $programs;do
    for t in $tests;do
        for n in {1..7};do
            mv output/$p/$t/cuda_test-$t-$n.json output/$p/$t/$p-$t-$n
        done
    done
done
