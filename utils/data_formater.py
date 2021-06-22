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
        "mpi",
        "mpi_br",
        "cuda_dim")

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

def subfill(main_df: pd.DataFrame, mpi_sr: pd.DataFrame, data: dict, ind: list[str]) -> None:
    """
    For a given test instance, data, set the partial values of the
    given data frames as specified by ind with those of the data
    dictionary
    """
    # ind example value: seq-test2.5-7.json -> 0: seq, 1: test2.5, (2,0): 7
    for i, middle_ind in enumerate(("none", "Send", "Recv")):
        for a in attributes:
            if i != 0 and str(i) in data:
                mpi_sr.loc[ind[0], middle_ind, a].at[ind[1],ind[2][0]] = data[str(i)][a]
            elif i == 0:
                main_df.loc[ind[0], a].at[ind[1],ind[2][0]] = data[str(i)][a]


def fill(main_df: pd.DataFrame, mpi_sr: pd.DataFrame) -> None:
    """
    Traverse output directory loading data into the given dataFrames
    """

    # For each technique used (MPI, OMP, CUDA, etc.)
    for d in dirs:
        # For each test
        for sd in subdirs:
            # Get all files within a test directory
            for root,_,files in os.walk(os.path.join("output",d,sd)):
                # For each instance of that test
                for f in files:
                    ind = f.split("-")
                    with open(root+"/"+f) as js:
                         data = json.load(js)["threads"]["0"]["regions"]
                         subfill(main_df, mpi_sr, data, ind)


def main():

    # Multilevel indices common to all dataframes
    col = subdirs[:]
    scol = [str(i) for i in range(1,8)]
    mcol = pd.MultiIndex.from_product([col,scol])

    # Create main data frame indices
    # Multilevel columns for main dataframe
    row = dirs
    srow = attributes
    mrow = pd.MultiIndex.from_product([row,srow])

    # Create MPI send/recive time row indices
    mpi_row = ("mpi", "mpi_br") # br meaning by matrix multiplication by rows
    mpi_srow = ("Send", "Recv")
    mpi_ssrow = attributes
    mpi_mrow = pd.MultiIndex.from_product([mpi_row,mpi_srow,mpi_ssrow])

    # Create main dataframe and MPI send/recive dataframe
    # Main data frame
    shape = (mrow.size , mcol.size) # Shape of the current dataframe
    main_df = pd.DataFrame(np.full(shape, np.nan), index=mrow, columns=mcol)

    # MPI send/recive data frame
    shape = (mpi_mrow.size, mcol.size)
    mpi_sr = pd.DataFrame(np.full(shape, np.nan), index=mpi_mrow, columns=mcol)

    fill(main_df, mpi_sr)

    np.savetxt("results/total_time.csv", main_df.values, delimiter=',')
    np.savetxt("results/mpi_sr_time.csv", mpi_sr.values, delimiter=',')
    main_df.to_excel("results/total_time.xlsx")
    mpi_sr.to_excel("results/mpi_sr_time.xlsx")


if __name__=="__main__":
    main()
