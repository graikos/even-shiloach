import matplotlib.pyplot as plt
import sys


def main():
    filename = sys.argv[1]
    meas = []
    first = True
    cases = []
    with open(filename, "r") as f:
        for line in f.readlines():
            if first:
                cases = [int(x) for x in line.split(",")[:-1]]
                first = False
                continue

            meas.append(float(line))

    # Create a figure and an axis
    fig, ax = plt.subplots()

    # Plot the first set of data
    ax.plot(cases, meas, label="Total time to remove all edges")

    # ax.set_yscale("log")

    # Add a title and labels
    ax.set_title(sys.argv[2])
    ax.set_xlabel("Number of vertices (N)")
    ax.set_ylabel("Time (ms)")

    # Add a legend
    ax.legend()

    # Show the plot
    plt.show()


if __name__ == "__main__":
    main()
