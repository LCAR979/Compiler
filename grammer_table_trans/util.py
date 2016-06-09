#-*- coding:utf-8 -*-
__author__ = 'LCAR979'
__Date__ = '4/10/2016'
__EMAIL__ = 'tcoperator@163.com'
def OneLineReader(filename):
    fp = open(filename, "r")
    line = fp.readline()
    while line:
        yield line
        line = fp.readline()
    fp.close()
    yield None