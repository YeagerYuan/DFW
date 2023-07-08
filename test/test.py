import os
import time

input_file_list = os.listdir("input")
output_file_list = os.listdir("output")
dump_file_list = os.listdir("dump")


def test():
    incorrect = 0           # incorrect nums
    correct = 0             # correct nums
    incorrect_list = []  # save incorrect output

    for file in input_file_list:
        if file[0] != '.':
            input_file_index = file.split('_')[1]
            dump_file = 'dump/dump_{}'.format(input_file_index)
            output_file = 'output/output_{}'.format(input_file_index)

            with open(dump_file, 'r') as d:
                rd = d.read().split('\n')
                if '\n' in rd:
                    rd.remove('\n')
                sd = set(rd)
            with open(output_file, 'r') as o:
                ro = o.read().split('\n')
                if '\n' in ro:
                    ro.remove('\n')
                so = set(ro)
            if sd == so:
                correct += 1
                print(f"{file}      PASS")
            else:
                incorrect += 1
                incorrect_list.append('dump_' + input_file_index)
    print("\nTRUE:{} | FALSE:{}".format(str(correct), str(incorrect)))
    if incorrect > 0: 
        print("\nError use case:")
        for x in incorrect_list:
            print("\n" + x)


if __name__ == '__main__':
    # os.system('rd .\\build')
    # os.system('md .\\build')
    # os.system('cd .\\build')
    # os.system('rd .\')
    os.system('cmake -G "MinGW Makefiles" .')
    os.system('mingw32-make')
    os.system('test.exe')
    # os.system('cd ../src && gcc test.c play.c display.c input.c stat_judge.c -o test && test')
    time.sleep(1)
    print("*************************\n")
    test()
