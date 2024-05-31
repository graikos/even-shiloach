import matplotlib.pyplot as plt
import sys


def main():
    for filename in sys.argv[1:]:
        dyn_list = []
        dfs_list = []
        with open(filename, "r") as f:
            for line in f.readlines():
                s = list(map(lambda x: float(x), line.split(",")))
                dyn_list.append(s[0])
                dfs_list.append(s[1])


        # Create a figure and an axis
        fig, ax = plt.subplots()

        # Plot the first set of data
        ax.plot(dyn_list, label="Dynamic structure")

        # Plot the second set of data
        ax.plot(dfs_list, label="DFS query")

        ax.set_yscale('log')

        # Add a title and labels
        ax.set_title("Two Lines on One Graph")
        ax.set_xlabel("X-axis")
        ax.set_ylabel("Y-axis")

        # Add a legend
        ax.legend()

        # Show the plot
        plt.show()


if __name__ == "__main__":
    main()
