#-*- coding:utf-8 -*-
__author__ = 'LCAR979'
__Date__ = '4/9/2016'
__EMAIL__ = 'tcoperator@163.com'
import re
from util import *
from trans import *

def table_process():
    #row, column should all start from 0
    drive_fp = open("output\\drive_table.txt",'w')
    reader = OneLineReader("res\\table.htm")
    if not reader:
        print "Error open html"
    grammer_fp = open("output\\grammer_transed.txt","r")
    grammer_list = grammer_fp.readlines()
    grammer_fp.close()

    terminal_fp = open("res\\nonterminal.txt",'r')
    terminal_list = terminal_fp.readlines()
    terminal_n = len(terminal_list)
    #print "Non terminal Number:", nonterminal_n
    terminal_fp.close()

    col_count = 0
    row_count = 0
    line = reader.next()
    while line:
        #print line
        goto_m = re.match(r"<td nowrap>(\d+)</td>", line)
        if line == "<td nowrap>0</td>\n":
            row_count = 0
        elif line == "</tr>\n":
            row_count += 1
        elif line == "<tr>\n":
            col_count = 0
        elif goto_m != None and col_count == 0:
            col_count = 0
        else:
            empty_m = re.match(r"<td nowrap>&nbsp;</td>", line)
            shift_m = re.match(r"<td nowrap>shift&nbsp;(\d+)</td>", line)
            reduce_m = re.match(r"<td nowrap>reduce([^<]*)</td>", line)
            acc_m = re.match(r"<td nowrap>accept</td>", line)
            if empty_m != None:
                pass
            elif goto_m != None and col_count!= 0:
                drive_fp.write(str(row_count) + "," + str(col_count) + "," \
                              + goto_m.group(1) +'\n')
            elif shift_m != None:
                drive_fp.write(str(row_count) +"," + str(col_count) + \
                    "," + shift_m.group(1)+'\n')
            elif reduce_m != None:
                target_s = reduce_m.group(1).replace("&nbsp;","").replace(" ","")\
                               .replace("\xa6\xc5","").replace("\n","").replace("ε","")+'\n'
                try:
                    index = grammer_list.index(target_s)
                    drive_fp.write(str(row_count) +"," + str(col_count) + \
                        ",-" + str(index)+'\n')
                except:
                    print "Error found " + target_s
            elif acc_m != None:
                drive_fp.write(str(row_count) +"," + str(col_count) + \
                        "," + str(1023) + '\n')
            col_count += 1
        line = reader.next()
    drive_fp.close()

def get_word_list():

    fn = open("res\\nonterminal.txt",'r')
    fw = open("output\\word.txt","w")
    content = fn.readlines()
    for i in range(len(content)):
        fw.write("\"" + content[i].strip("\n") + "\", ")
        if i % 5 == 0:
            fw.write("\n")
    fw.write("\n\n")
    for i in range(len(content)):
        fw.write("V_"+content[i].replace("\n","")+", ")
        if i % 5 == 0:
            fw.write("\n")
    fn.close()
    fw.close()



if __name__ == "__main__":
    terminal_trans()
    get_word_list()
    grammer_to_number()
    grammer_trans()
    table_process()
    generate_production_word()

