import matplotlib.pyplot as plt
import numpy as np
import sys
import pandas as pd


def main():
    filename = sys.argv[1]
    dyn_list = []
    dfs_list = []
    with open(filename, "r") as f:
        for line in f.readlines():
            s = list(map(lambda x: float(x), line.split(",")))
            dyn_list.append(s[0])
            dfs_list.append(s[1])

    dyn_list_np = np.array(dyn_list)
    dfs_list_np = np.array(dfs_list)

    df = pd.DataFrame({"dyn": dyn_list_np, "dfs": dfs_list_np})
    df["dyn_roll"] = df["dyn"].rolling(window=15).mean()
    df["dfs_roll"] = df["dfs"].rolling(window=15).mean()


    # create a figure and an axis
    fig, ax = plt.subplots()

    if len(sys.argv) < 4:
        # plot the first set of data
        ax.plot(dyn_list, label="Dynamic structure", color="#d0d5ed")

        # plot the second set of data
        ax.plot(dfs_list, label="DFS query", color="#edd0d0")

    # plot the second set of data
    ax.plot(df["dyn_roll"], label="Dynamic structure (Rolling Avg.)")
    ax.plot(df["dfs_roll"], label="DFS query (Rolling Avg.)")

    ax.set_yscale('log')

    # add a title and labels
    ax.set_title(sys.argv[2])
    ax.set_ylabel("Time (ms)")
    ax.set_xlabel("Edges removed (queries)")

    # add a legend
    ax.legend()

    fig = plt.gcf()

    fig.savefig(filename[:-4] + ".png")
    # show the plot
    plt.show()



if __name__ == "__main__":
    main()
