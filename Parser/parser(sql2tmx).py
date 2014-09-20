# v0.1.2.1b (sql2tmx)

#!/usr/bin/python
#coding=utf8

import os
import re
import sys
import math
import random
import MySQLdb

def init():
    return 0

def export(mymat, sx, sy, filename):
    fp = open(filename, 'w')
    #print filename
    #print "=== start at (%d, %d) ===" % (sx, sy)
    
    fp.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
    
    MAPSIZE = 25
    mapSizeX = MAPSIZE
    mapSizeY = MAPSIZE * 2 + 6
    
    fp.write("<map version=\"1.0\" orientation=\"orthogonal\" width=\"%d\" height=\"%d\" tilewidth=\"61\" tileheight=\"25\">\n" % (mapSizeX, mapSizeY))
    fp.write(" <tileset firstgid=\"1\" source=\"PF Objects.tsx\"/>\n")
    fp.write(" <tileset firstgid=\"55\" source=\"mark.tsx\"/>\n")
    
    layer = 6
    nonemptytile = 0
    for lr in range(layer):
        fp.write(" <layer name=\"PF Layer %d\" width=\"%d\" height=\"%d\">\n" % (lr, mapSizeX, mapSizeY))
        fp.write("  <data>\n")

        tmpmat = []
        for x in range(mapSizeX):
            tmpmat.append([54 for y in range(mapSizeY)])
        print "lr %d => (%d, %d).. (%d, %d)" % (lr, sx, sy, sx+MAPSIZE, sy+MAPSIZE)
        for y in range(sy, sy + MAPSIZE):
            for x in range(sx, sx + MAPSIZE):
                if(lr < mymat[x][y]):
                    nonemptytile += 1
                    #print "------ %d, %d" % (x, y)
                    #print (x - sx, mapSizeY - lr - 1 - (sy+MAPSIZE - 1 - y) * 2)
                    tmpmat[x - sx][mapSizeY - lr - 1 - (sy+MAPSIZE - 1 - y) * 2] = (((x*25+y)*317+lr)*9)%7+1

        for y in range(mapSizeY):
            for x in range(mapSizeX):
                fp.write("   <tile gid=\"%d\"/>\n" % tmpmat[x][y])

        fp.write("  </data>\n")
        fp.write(" </layer>\n")
        #############################
        fp.write(" <layer name=\"Mark Layer %d\" width=\"%d\" height=\"%d\">\n" % (lr, mapSizeX, mapSizeY))
        fp.write("  <data>\n")

        tmpmat = []
        for x in range(mapSizeX):
            tmpmat.append([57 for y in range(mapSizeY)])

        for y in range(mapSizeY):
            for x in range(mapSizeX):
                fp.write("   <tile gid=\"%d\"/>\n" % tmpmat[x][y])

        fp.write("  </data>\n")
        fp.write(" </layer>\n")
    fp.write("</map>\n")
    fp.close()
    if(nonemptytile > 0):
        print filename
        
def main(argv = None):
    db = MySQLdb.connect("@", "@", "@", "mySQLMapDB")
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
                        ID <= 3000; """
    cursor.execute(sql)
    res = cursor.fetchone()
    print res

    rangeX = 1 + (int)(res[1] - res[0]) / 100
    rangeY = 1 + (int)(res[3] - res[2]) / 100

    rangeX = (int)(math.ceil(rangeX / 25.0)) * 25
    rangeY = (int)(math.ceil(rangeY / 25.0)) * 25

    print (rangeX, rangeY)
    
    mymat = []
    for i in range(rangeX):
        mymat.append([0 for j in range(rangeY)])
    
    gapz = (res[5] - res[4] + 1) / 6.0

    sql = """SELECT 
                `POINT_X`, `POINT_Y`, `POINT_Z`
            FROM
                `mySQLMapDB`.`test1`
            WHERE ID <= 3000; """

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
    mapID = 0
    
    for sx in range(0, rangeX, 25):
        for sy in range(0, rangeY, 25):
            mapIDstr = str(mapID % 10)
            mapIDstr += str(mapID / 10 % 10)
            mapIDstr += str(mapID / 100 % 10)
            mapID += 1
            export(mymat, sx, sy, "pfmap" + mapIDstr[::-1] + ".tmx")
            
    return "ok"

if __name__ == "__main__":
    scode = main()
    print(scode)
