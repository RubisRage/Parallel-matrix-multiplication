#!/bin/python3

import sys
import json
import os

attributes = ("cycles",
              "real_time_nsec",
              "PAPI_TOT_INS",
              "PAPI_TOT_CYC")

program = ("seq",
           "omp",
           "cuda",
           "mpi")


for p in program:
        for t in range(1,5):
            print("output/"+p+f"-test{t}/"+p+f"-test{t}.json")
            # with open("output/"+p+f"-test{t}/"+p+f"-test{t}.json") as f:
            #     data = json.load(f)["threads"]["0"]["regions"]["0"]
            #     for a in attributes:
            #         print()

# for root, dirs, _ in os.walk("output"):
#     for d in dirs:
#         for _,_,files in os.walk(os.path.join(root,d)):
#             for f in files:
#                 print(os.path.join(root,d,f))

# with open("output/seq-test1/seq-test1.json", "r") as f:
#     data = json.load(f)["threads"]["0"]["regions"]["0"]

#     for item in data.items():
#         print(item[1])

# with open("output/seq-test1/seq-test1.csv", "w") as f:
#     print
#     pass

#print(data)
#
