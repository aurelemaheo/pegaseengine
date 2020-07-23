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

def parsejson():

    print("Parse JSON scene")
    with open('../tests/scene01.json') as filejson:
      scene = json.load(filejson)

    print("Print JSON scene")
    #print(scene["bodies"])

    bodiesarray = scene["bodies"]

    for body in bodiesarray:
      print("print position in bodiesarray")
      print("x: " + str(body["position"]["x"]) + ", y: " + str(body["position"]["y"]) + ", z: " + str(body["position"]["z"]))
      print("print linear velocity in bodiesarray")
      print("x: " + str(body["linvelocity"]["x"]) + ", y: " + str(body["linvelocity"]["y"]) + ", z: " + str(body["linvelocity"]["z"]))
      print("print angular velocity in bodiesarray")
      print(body["angvelocity"])
      print("print angular axis in bodiesarray")
      print(body["angaxis"])
      print("x: " + str(body["angaxis"]["x"]) + ", y: " + str(body["angaxis"]["y"]) + ", z: " + str(body["angaxis"]["z"]))
    
    return 0

def main():
    parsejson()

if __name__ == "__main__":
    main()