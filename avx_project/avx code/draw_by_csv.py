# -*-encoding:utf-8-*-
import csv
import matplotlib.pyplot as plt
import os
import matplotlib.ticker as ticker
import numpy as np
# fig, ax = plt.subplots(2,4,figsize=(25, 10))
fig = plt.figure(figsize=(40, 20))
# 保存的大图
global ind
ind = 1


def mk_img_dir():
    img_dir = './img/'
    if(not os.path.exists(img_dir)):
        os.mkdir(img_dir)
    files = os.listdir(img_dir)
    
    max_ind = 0
    for file in files:
        max_ind = max(max_ind, int(file[0:3])) #前三位是数字
    
    # new_dir = str(len(files) + 1)
    
    new_dir = str(max_ind + 1)
    new_dir = new_dir.zfill(3)  # 填充为3长度的数
    new_dir = img_dir + new_dir + '/'
    os.mkdir(new_dir)
    return new_dir


def show1(gather_type='gather256', optimi='o1', incsv='GBS256o1.csv', out_dir='./img/001/'):

    if(not os.path.exists(incsv)):
        return
    x = []
    a, b, c, d = [], [], [], []
    with open(incsv, 'r') as f:
        lines = f.readlines()
        lines = lines[1:]
        for line_counter, line in enumerate(lines):
            # print(line_counter)
            # if line_counter != 0:
            #     line = line.split(',')
            # print(line)
            x.append(int(line[0]))
            a.append(float(line[1]))
            b.append(float(line[2]))
            c.append(float(line[3]))
            d.append(float(line[4]))
    f.close()
    # print(a)
    plt.figure(figsize=(25, 10))

    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)

    plt.title(gather_type + ' ' + optimi + ' bandwidth', fontsize=30)
    plt.xlabel("data size", fontsize=25)
    plt.ylabel("GB/s", fontsize=25)

    plt.rcParams.update({'font.size': 25})  # 设置图例字体大小
    plt.plot(x, a, label='gather sequentially', color='blue', marker='o')
    plt.plot(x, b, label='general sequentially', color='fuchsia', marker='x')
    plt.plot(x, c, label='gather randomly', color='orangered', marker='^')
    plt.plot(x, d, label='general randomly', color='lime', marker='s')

    global ind
    ax = fig.add_subplot(2, 4, ind)
    ind = ind + 1

    ax.set_title(gather_type + ' ' + optimi + ' bandwidth')
    # ax.xlabel("data size")
    # ax.ylabel("GB/s")

    ax.plot(x, a, label='gather sequentially',
            color='blue', marker='o', markersize='1', linewidth=1)
    ax.plot(x, b, label='general sequentially',
            color='fuchsia', marker='x', markersize='1', linewidth=1)
    ax.plot(x, c, label='gather randomly',
            color='orangered', marker='^', markersize='1', linewidth=1)
    ax.plot(x, d, label='general randomly',
            color='lime', marker='s', markersize='1', linewidth=1)

    # plt.plot(x,a,label='gather linearly',color='k',marker='o')
    # plt.plot(x,b,label='no linearly',color='r',marker='x')
    # plt.plot(x,c,label='gather randomly',color='b',marker='^')
    # plt.plot(x,d,label='no randomly',color='g',marker='s')
    # plt.savefig('g512_O3.png', bbox_inches='tight')

    plt.legend()
    plt.grid()

    plt.savefig(out_dir + gather_type + '_' + optimi + '.png')
    # plt.show()


def previous_store():
    new_dir = mk_img_dir()

    # show1('gather256', 'without -O', 'GBS256o0.csv', new_dir)
    show1('gather256', 'o0', './data/GBS256o0.csv', new_dir)
    show1('gather256', 'o1', './data/GBS256o1.csv', new_dir)
    show1('gather256', 'o2', './data/GBS256o2.csv', new_dir)
    show1('gather256', 'o3', './data/GBS256o3.csv', new_dir)

    # show1('gather512', 'without -O', 'GBS512o0.csv', new_dir)
    show1('gather512', 'o0', './data/GBS512o0.csv', new_dir)
    show1('gather512', 'o1', './data/GBS512o1.csv', new_dir)
    show1('gather512', 'o2', './data/GBS512o2.csv', new_dir)
    show1('gather512', 'o3', './data/GBS512o3.csv', new_dir)

    lines, labels = fig.axes[-1].get_legend_handles_labels()
    # fig.legend(lines, labels, loc='upper center')
    fig.legend(lines, labels)

    fig.savefig(new_dir + '8.png')


def show2(gather_type='gather256', optimi='o1', incsv='GBS256o1.csv', out_dir='./img/001/', col=16, ylim=[2, 11]):
    # 2021-11-26 Friday

    if(not os.path.exists(incsv)):
        return

    x = []
    y = []
    ave = [0] * col
    for i in range(col):
        y.append([])

    row = 0
    # process .csv file:
    title = ''
    with open(incsv, 'r') as f:
        lines = f.readlines()
        title = lines[0].strip('\n').split(',')[1:]
        lines = lines[1:]
        row = len(lines) #行数
        for line_counter, line in enumerate(lines):
            line = line.strip('\n').split(',')
            x.append(int(line[0]))
            line = line[1:]
            for index, elem in enumerate(line):
                num = float(elem)
                y[index].append(num)
                if num != float ("inf"):
                     ave[index] += num #平均值累积
    ave = [x/row for x in ave] #取平均
    
    # plt.xlim(100,100000)
    # plt.ylim(2,10)

    fig = plt.figure(figsize=(40, 20))


    for i in range(col):

        ax = fig.add_subplot(4, 4, i+1)

        ax.set_title(gather_type + ' ' + optimi +
                     ' ' + title[i] + ' bandwidth')
        ax.set_ylim(ylim)

        # ax.xaxis.set_major_locator(ticker.MultipleLocator(20000))
        # ax.yaxis.set_major_locator(ticker.MultipleLocator(1))

        # ax.xlabel("data size")
        # ax.ylabel("GB/s")

        ax.plot(x, y[i], color='blue', marker='o', markersize='1', linewidth=1)
        ax.axhline(y=ave[i],color="r",ls='dashed',label='average')
        # ax.hlines(y=ave[i],colors="green",linestyles='dotted')
        ax.legend()
        # ax.grid()
        
    fig.savefig(out_dir + gather_type + optimi + '_' + str(col) + '.png')
    # plt.show()




def store2():

    opt = ['o0', 'o1', 'o2', 'o3']
    p = {
        'g': 'gather256',

        'data_dir': './data/GBS256'
    }

    new_dir = mk_img_dir()
    ylim = [4, 12]
    for o in opt:
        show2(p['g'], o, p['data_dir']+o+'.csv', new_dir, 16, ylim)


def main():
    # previous_store()
    store2()


if __name__ == '__main__':

    main()
