# v0.1.2.0 (grd2sql)

#!/usr/bin/python
#coding=utf8

import os
import re
import sys
import MySQLdb

def init():
    db = MySQLdb.connect("@", "@", "@", "mySQLMapDB")
    cursor = db.cursor()
    cursor.execute("DROP TABLE IF EXISTS `mySQLMapDB`.`test1`")
    sql = """CREATE TABLE `mySQLMapDB`.`test1` (
            `ID` INT NOT NULL AUTO_INCREMENT,
            `POINT_X` DOUBLE NOT NULL,
            `POINT_Y` DOUBLE NOT NULL,
            `POINT_Z` DOUBLE NOT NULL,
            PRIMARY KEY (`ID`),
            UNIQUE INDEX `ID_UNIQUE` (`ID` ASC))
            ENGINE = InnoDB;"""
    
    cursor.execute(sql)
    return db

def anylize(filepath, db):
    if(not os.path.isfile(filepath)):
        return "ERROR"
    fp = open(filepath, 'r')
    for line in fp:
        nums = (map(float,line.split(' ')))
        #print "%f %f %f" % (nums[0], nums[1], nums[2])
        ins_sql = """INSERT INTO  `mySQLMapDB`.`test1` (
                    `POINT_X` ,
                    `POINT_Y` ,
                    `POINT_Z`
                    ) VALUES (
                    '%f',  '%f',  '%f'
                    );""" % (nums[0], nums[1], nums[2])
        cursor = db.cursor()
        cursor.execute(ins_sql)
        db.commit()
        lastID = cursor.lastrowid
        if(lastID % 1000 == 0):
            print "insert at %d" % (lastID)
    fp.close()
    return 0

def Test1(rootDir, db):
    list_dirs = os.walk(rootDir)
    for root, dirs, files in list_dirs:
        for f in files:
            filepath = os.path.join(root, f)
            #print filepath
            anylize(filepath, db)


def main(argv = None):
    db = init()
    print "db connect"
    Test1("Kaohsiung/", db)
    db.close()
    print "db close"
    return "ok"

if __name__ == "__main__":
    scode = main()
    print(scode)
