# -*- coding: utf-8 -*-
# ---------------------------------------------------------------------------
# grd2shp.py
# Created on: 2014-09-13 14:14:22.00000
#   (generated by ArcGIS/ModelBuilder)
# Description: 
# ---------------------------------------------------------------------------

# Import arcpy module
import arcpy

# Load required toolboxes
arcpy.ImportToolbox("Model Functions")


# Local variables:
dtm22 = "C:\\Users\\Lee\\Documents\\ArcGIS\\dtm22"
RASTER = "C:\\Users\\Lee\\Documents\\ArcGIS\\dtm22\\93203014dem.grd"
v_Name__shp = "C:\\Users\\Lee\\Documents\\ArcGIS\\dtmresult\\%Name%.shp"

# Process: Iterate Datasets
arcpy.IterateDatasets_mb(dtm22, "", "RASTER", "RECURSIVE")

# Process: Raster to Point
arcpy.RasterToPoint_conversion(RASTER, v_Name__shp, "Value")

# Process: Add Geometry Attributes
arcpy.AddGeometryAttributes_management(v_Name__shp, "POINT_X_Y_Z_M", "", "", "")

