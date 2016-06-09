#-*- coding:utf-8 -*-
__author__ = 'LCAR979'
__Date__ = '4/10/2016'
__EMAIL__ = 'tcoperator@163.com'

import re
class Mode():
    lPart = 1
    rPart = 0

def terminal_trans():
    fp = open("res\\terminal.txt","r")
    fp_out = open("output\\terminal_transed.txt","w")
    pre = ""
    while True:
        c = fp.read(1)
        if not c:
            break
       # print repr(c)+" pre:"+repr(pre)
        if c == " " and pre != " ":
            fp_out.write("\n")
            pre = " "
        elif (c==" " and pre == " ") or c == ',':
            pass
        else:
            fp_out.write(c)
            pre = c
    fp.close()
    fp_out.close()

def grammer_to_number():
    nonterminal_fp = open("res\\nonterminal.txt",'r')
    nonterminal_list = nonterminal_fp.read().split("\n")
    nonterminal_fp.close()

    terminal_fp = open("output\\terminal_transed.txt",'r')
    terminal_list = terminal_fp.read().split("\n")
    t_n = len(terminal_list)
    terminal_fp.close()

    fp = open("res\\grammer.txt","r")
    fp_out = open("output\\grammer_by_number.txt","w")
    reobj = re.compile(r"([\s\;])")
    content = reobj.split(fp.read())
    for i in content:
        if i == '\n' or i == " " or i =="":
            fp_out.write(i)
            continue
        try:
            t_index = terminal_list.index(i)
            fp_out.write(str(t_index)+' ')
        except:
            try:
                v_index = nonterminal_list.index(i) + 1
                fp_out.write(str(t_n+v_index)+' ')
            except:
                fp_out.write(i)

    print t_n
    fp.close()
    fp_out.close()

def production_trans(grammer_file_name, grammer_file_name_transed):
    fp_out = open(grammer_file_name_transed,'w')
    fp = open(grammer_file_name, "r")
    mode = Mode.lPart
    lPartBuf = ""
    rPartBuf = ""
    while True:
        c = fp.read(1)
        if not c:
            break
        if c == '=':
            fp.read(1)
            mode = Mode.rPart
            continue
        elif c == '\n' or c == '\t' or c==' ':
            pass
        elif c == ';':
            fp_out.write(lPartBuf+"->"+rPartBuf+'\n')
            lPartBuf = ""
            rPartBuf = ""
            mode = Mode.lPart
        elif c == '|':
            fp_out.write(lPartBuf+"->"+rPartBuf+'\n')
            rPartBuf = ""
            mode = Mode.rPart
        elif mode == Mode.lPart:
            lPartBuf += c
        elif mode == Mode.rPart:
            rPartBuf += c

def number_production_trans(grammer_file_name, grammer_file_name_transed):
    fp_out = open(grammer_file_name_transed,'w')
    fp = open(grammer_file_name, "r")
    mode = Mode.lPart
    lPartBuf = ""
    rPartBuf = ""
    pre =""
    while True:
        c = fp.read(1)
        if not c:
            break
        if c == '=':
            fp.read(1)
            mode = Mode.rPart
            continue
        elif c == '\t' or c == '\n':
            pass
        elif c == " " and pre == " ":
            pass
        elif c == ';':
            fp_out.write(lPartBuf+" "+rPartBuf+';\n')
            lPartBuf = ""
            rPartBuf = ""
            mode = Mode.lPart
        elif c == '|':
            fp_out.write(lPartBuf+" "+rPartBuf+';\n')
            rPartBuf = ""
            mode = Mode.rPart
        elif mode == Mode.lPart:
            lPartBuf += c
        elif mode == Mode.rPart:
            rPartBuf += c
        pre = c

def generate_production_word():
    fp = open("res\\grammer.txt",'r')
    fp_func = open("res\\func_name.txt","r")
    func_list = fp_func.readlines()
    fp_func.close()
    fp_func_dec = open("output\\func_dec.txt",'w')
    fp_out = open("output\\call_table.txt",'w')

    line_number = 0
    mode = Mode.lPart
    lPartBuf = ""
    rPartBuf = ""
    while True:
        c = fp.read(1)
        if not c:
            break
        if c == '=':
            fp.read(1)
            mode = Mode.rPart
            continue
        elif c == '\n' or c == '\t':
            pass
        elif c == ';':
            fp_out.write("{" + str(line_number) + ", \"" + lPartBuf+"->"+rPartBuf+"\", "
                                                 + func_list[line_number].strip("\n") + "},\n")
            fp_func_dec.write("extern void " +  func_list[line_number].strip("\n") + "(Item* it);\n")
            lPartBuf = ""
            rPartBuf = ""
            mode = Mode.lPart
            line_number += 1
        elif c == '|':
            fp_out.write("{" + str(line_number) + ", \"" + lPartBuf+"->"+rPartBuf+"\", "
                                                 + func_list[line_number].strip("\n") + "},\n")
            fp_func_dec.write("extern void " +  func_list[line_number].strip("\n") + "(Item* it);\n")
            rPartBuf = ""
            mode = Mode.rPart
            line_number += 1
        elif mode == Mode.lPart:
            lPartBuf += c
        elif mode == Mode.rPart:
            rPartBuf += c

    fp.close()
    fp_out.close()

def grammer_trans():
    production_trans("res\\grammer.txt", "output\\grammer_transed.txt")
    number_production_trans("output\\grammer_by_number.txt","output\\grammer_number_transed.txt")