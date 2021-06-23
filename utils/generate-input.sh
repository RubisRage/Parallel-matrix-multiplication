#!/bin/bash

tests=(test250 test500 test750 test1 test1.5 test2 test2.5 test3 test3.5 test4 test4.5)
size=(250 500 750 1000 1500 2000 2500 3000 3500 4000 4500)

for i in {0..10};do
    utils/matrix-generatory.py input/${tests[$i]} ${size[$i]}
done
