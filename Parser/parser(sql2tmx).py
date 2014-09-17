# v0.1.2.0 (sql2tmx)

#!/usr/bin/python
#coding=utf8

import os
import re
import sys
import random
import MySQLdb

def init():
    return 0

def export(mymat):
    fp = open("gogo.tmx", 'w')
    fp.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
    WW = len(mymat)
    HH = len(mymat[0])
    mapSizeX = len(mymat)  #X-coord
    mapSizeY = len(mymat[0]) * 2 + 6  #Y-coord
    fp.write("<map version=\"1.0\" orientation=\"orthogonal\" width=\"%d\" height=\"%d\" tilewidth=\"61\" tileheight=\"25\">\n" % (mapSizeX, mapSizeY))
    fp.write(" <tileset firstgid=\"1\" source=\"PF Objects.tsx\"/>\n")
    fp.write(" <tileset firstgid=\"55\" source=\"mark.tsx\"/>\n")
    layer = 6
    for lr in range(layer):
        fp.write(" <layer name=\"PF Layer %d\" width=\"%d\" height=\"%d\">\n" % (lr, mapSizeX, mapSizeY));
        fp.write("  <data>\n")
        
        tmpmat = []
        for x in range(mapSizeX):
            tmpmat.append([54 for y in range(mapSizeY)])
            
        for y in range(HH):
            for x in range(WW):
                if(lr < mymat[x][y]):
                    #print (x, mapSizeY - lr - (HH - 1 - y) * 2)
                    tmpmat[x][mapSizeY - lr - 1 - (HH - 1 - y) * 2] = random.choice([1, 2, 3, 4, 5, 6, 7])
                    
        for y in range(mapSizeY):
            for x in range(mapSizeX):
                fp.write("   <tile gid=\"%d\"/>\n" % tmpmat[x][y]);
                
        fp.write("  </data>\n")
        fp.write(" </layer>\n")
        #############################
        fp.write(" <layer name=\"Mark Layer %d\" width=\"%d\" height=\"%d\">\n" % (lr, mapSizeX, mapSizeY));
        fp.write("  <data>\n")
        
        tmpmat = []
        for x in range(mapSizeX):
            tmpmat.append([57 for y in range(mapSizeY)])
                    
        for y in range(mapSizeY):
            for x in range(mapSizeX):
                fp.write("   <tile gid=\"%d\"/>\n" % tmpmat[x][y]);
                
        fp.write("  </data>\n")
        fp.write(" </layer>\n")
    fp.write("</map>\n")
    fp.close()
    

def main(argv = None):
    init()
    db = MySQLdb.connect("@, "@", "@", "mySQLMapDB")
    cursor = db.cursor()
    sql = """SELECT MIN(POINT_X),
                    MAX(POINT_X),
                    MIN(POINT_Y),
                    MAX(POINT_Y),
                    MIN(POINT_Z),
                    MAX(POINT_Z)
                    FROM
                        mySQLMapDB.test1
                    WHERE
                        ID <= 700; """
    cursor.execute(sql)
    res = cursor.fetchone()
    print res

    rangeX = 1 + (int)(res[1] - res[0]) / 100
    rangeY = 1 + (int)(res[3] - res[2]) / 100
    print (rangeX, rangeY)
    mymat = []
    for i in range(rangeX):
        mymat.append([0 for j in range(rangeY)])
    print mymat[0][0]
    gapz = (res[5] - res[4] + 1) / 6.0
    
    sql = """SELECT 
                `POINT_X`, `POINT_Y`, `POINT_Z`
            FROM
                `mySQLMapDB`.`test1`
            WHERE ID <= 700; """

    minX = res[0]
    minY = res[2]
    minZ = res[4]
    cursor.execute(sql)
    cnt = 0
    for row in cursor.fetchall():
        mapX = (int)(row[0] - minX) / 100
        mapY = (int)(row[1] - minY) / 100
        mapZ = (int)((row[2] - minZ) / gapz)
        mymat[mapX][mapY] = mapZ
    
    db.close()
    export(mymat)
    return "ok"

if __name__ == "__main__":
    scode = main()
    print(scode)
