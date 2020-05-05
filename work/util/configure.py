#!/usr/bin/env python

#this code is for editing particular attributes of a given subelement of a XML tree 

import xml.etree.ElementTree as ET     #ElementTree method for parsing and creating XML data
import argparse

parser = argparse.ArgumentParser() #create an argumentparser object

#use add_argument method to specify which command-line options the program is required

parser.add_argument('--include_pv', action='store_true') 
parser.add_argument('--raster_r', type=float, required=True, metavar='value',
                    help="computes cube for the given value")
parser.add_argument('--raster_phi', type=float, required=True, metavar='value', 
                    help="computes cube for the given value")

args = parser.parse_args()

print(args.include_pv)
print(args.raster_r)
print(args.raster_phi)

     
tree = ET.parse('template.xml')#Access XML tree
root= tree.getroot() 
print(root.attrib)
sirasterrizer = root.findall('.//SiRasterizer') #Selects all subelements SiRasterizer in the entire tree
vertex = root.findall('.//Vertex') #Selects all subelements Vertex in the entire tree

#update the attribute value for Vertex subelements depending on input
for idx, Vertex in enumerate(vertex): #enumerate function in for loop for Vertex subelement 
 if args.include_pv== True:                     
   Vertex.attrib['includeInFit']= "true"           
 if args.include_pv== False:
   Vertex.attrib['includeInFit']= "false"

#update the attributes value for SiRasterizer subelements depending on input            
for idx2, SiRasterizer in enumerate(sirasterrizer):#enumerate function in for loop for SiRasterizer subelement
    SiRasterizer.attrib['r']= str(args.raster_r)                
    SiRasterizer.attrib['phi']= str(args.raster_phi)

tree.write('config.xml',xml_declaration=True, encoding='utf-8')#update the xml file
    
   
