#!/usr/bin/env bash

tests="test1.5 test2.5 test3.5 test4.5"
programs="cuda omp seq mpi"

    echo
for p in $programs;do
    for t in $tests;do
        for n in {1..3};do
            mv output/$p/$t/$p-$t-$n.json output/$p/$t/$p-$t-$n
            mv output/$p/$t/$p-$t-$n/* output/$p/$t/$p-$t-$n.json
            rmdir output/$p/$t/$p-$t-$n
        done
    done
done
