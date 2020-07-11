#! /usr/bin/python3
"""
This script reads Common Crawl WAT files, parses and analyzez the JSON data
from WAT files and outputs, and produces a list of number of
linkbacks per domain (at a subdomain granularity).

This job uses Spark to run this distributed ETL job.
"""

import time
import os

import json

def parsejson(self):

    print "Parse JSON scene"
    with open('tests/scene01.json') as filejson:
    scene = json.load(filejson)

    print "Print JSON scene"
    print(scene)

#def main(self):

#    iself.parsejson()
