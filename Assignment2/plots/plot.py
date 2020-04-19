import matplotlib.pyplot as plt 

# filename = "ISLIP.csv"
# filename = "INQ.csv"
filename = "kouq_k_n_out.csv"

f = open(filename, 'r')
lines = [line for line in f]

def plotter(key_indices, index_x, index_y):
    dict_x = dict()
    dict_y = dict()
    legends = []

    for line in lines[1:]:
        vals = line.split(',')
        key = [vals[i] for i in key_indices]
        # key = vals[1], vals[2], vals[3]
        key = tuple(key)
        # print(key)
        if (key in dict_x):
            dict_x[key].append(float(vals[index_x]))
        else:
            dict_x[key]= [float(vals[index_x])]

        if (key in dict_y):
            dict_y[key].append(float(vals[index_y]))
        else:
            dict_y[key]= [float(vals[index_y])]

    print(dict_x)
    print(dict_y)

    for key in dict_x:
        plt.plot(dict_x[key], dict_y[key])
        plt.xlabel(lines[0].split(',')[index_x])
        plt.ylabel(lines[0].split(',')[index_y])
        plt.title(lines[0].split(',')[index_x] + ' vs ' + lines[0].split(',')[index_y])

        plot_legend = ""
        for i in key:
            plot_legend += str(i) + " "
        # val1, val2, val3 = key
        # plot_legend = "B: " + str(val1) + " K: " + str(val2) + " p: " + str(val3)
        legends.append(plot_legend)
        print(plot_legend)
        # plt.legend(plot_legend)
    plt.legend(legends, loc="best")
    plt.savefig(filename.split('.')[0] + '_' + lines[0].split(',')[index_x] + "_" + lines[0].split(',')[index_y].rstrip('\n') + ".png")
    plt.clf()

plotter([2], 0, 5)
plotter([2], 0, 6)
plotter([2], 0, 7)
plotter([2], 0, 8)