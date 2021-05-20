#!/bin/python3

import json
import os
import pandas as pd
import numpy as np

attributes = ("cycles",
              "real_time_nsec",
              "PAPI_TOT_INS",
              "PAPI_TOT_CYC")

dirs = ("seq",
        "omp",
        "cuda",
        "mpi")

subdirs =  ("test250",
            "test500",
            "test750",
            "test1",
            "test1.5",
            "test2",
            "test2.5",
            "test3",
            "test3.5",
            "test4",
            "test4.5")

def fill(df: pd.DataFrame):
    for d in dirs:
        for sd in subdirs:
            for root,_,files in os.walk(os.path.join("output",d,sd)):
                for f in files:
                    ind = f.split("-")
                    with open(root+"/"+f) as js:
                         data = json.load(js)["threads"]["0"]["regions"]["0"]
                    for a in attributes:
                        df.loc[ind[0], a].at[ind[1],ind[2][0]] = data[a]


def main():

    col = subdirs[:]
    scol = [str(i) for i in range(1,8)]
    row = dirs
    srow = attributes

    shape = (len(srow)*len(row),len(col)*len(scol))

    mcol = pd.MultiIndex.from_product([col,scol])
    mrow = pd.MultiIndex.from_product([row,srow])

    df = pd.DataFrame(np.full(shape, np.nan), index=mrow, columns=mcol)
    fill(df)

    gmean = pd.DataFrame(np.full((mrow.size, len(col)), np.nan), index=mrow, columns=col)
    for c in col:
        gmean[c] = df[c].aggregate(lambda ite: ite.prod()**(1.0/len(ite)), axis="columns")

    print(gmean)

    np.savetxt("test2.csv", df.values,delimiter=',')
    #df.to_excel("exe_test.xlsx")

if __name__=="__main__":
    main()
