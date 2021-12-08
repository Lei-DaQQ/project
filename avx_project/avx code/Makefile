cc = gcc
py = python3

myWinFlag = win

ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
	os := $(myWinFlag)
else
	os := $(shell uname)  # same as"uname -s"
endif

filename = gather

source1 = $(filename)256.c common.c
source2 = $(filename)512.c common.c

out_dir = out/
out256 = $(out_dir)$(filename)256o
out512 = $(out_dir)$(filename)512o

count = 0
$(out256): $(source1)
	$(shell if [ ! -e $(out_dir) ];then mkdir -p $(out_dir); fi)
#	count=$(shell ls $(out_dir) -l |grep "^-"|wc -l)
#	$(shell mkdir -p $(out_dir))
#	out_dir=$(out_dir)$(count)/

	$(cc)     -g -mavx2 $(source1) -o $(out256)0
	$(cc) -o1 -g -mavx2 $(source1) -o $(out256)1
	$(cc) -o2 -g -mavx2 $(source1) -o $(out256)2
	$(cc) -o3 -g -mavx2 $(source1) -o $(out256)3
	
	$(cc)     -g -mavx512f -march=native $(source2) -o $(out512)0
	$(cc) -o1 -g -mavx512f -march=native $(source2) -o $(out512)1
	$(cc) -o2 -g -mavx512f -march=native $(source2) -o $(out512)2
	$(cc) -o3 -g -mavx512f -march=native $(source2) -o $(out512)3
#gcc -mavx2 gather256.c common.c -o gather0
#gcc -mavx512f gather512.c common.c -o gather0
GBS = GBS

#win 不加后缀 【.exe】 也可以运行
run:
	./$(out256)0 $(GBS)256o0
	./$(out256)1 $(GBS)256o1
	./$(out256)2 $(GBS)256o2
	./$(out256)3 $(GBS)256o3
	
	./$(out512)0 $(GBS)512o0
	./$(out512)1 $(GBS)512o1
	./$(out512)2 $(GBS)512o2
	./$(out512)3 $(GBS)512o3
r2:
#	./$(out256)0 $(GBS)256o0
#	./$(out256)1 $(GBS)256o1
#	./$(out256)2 $(GBS)256o2
	./$(out256)3 $(GBS)256o3

r5:
#	./$(out512)0 $(GBS)512o0
#	./$(out512)1 $(GBS)512o1
#	./$(out512)2 $(GBS)512o2
	./$(out512)3 $(GBS)512o3


#注释的井号要加在tab前
# run:
# ifeq ($(os), $(myWinFlag))
#	$(out).exe
# else
#	 ./$(out)
# endif

draw:
	python3 draw_by_csv.py




clean:
ifeq ($(os), $(myWinFlag))
	del $(out256)*.exe
	del $(out512)*.exe
else
	find $(out_dir) -name "gather*" ! -name "*.c" -exec rm {} \;
endif

# del:
#	 del $(out).exe

	
# gcc -mavx2 -S -fverbose-asm $(source1)  #看详细的汇编语言结果
