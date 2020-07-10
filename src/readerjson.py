#! /usr/bin/python3
"""
This script reads Common Crawl WAT files, parses and analyzez the JSON data
from WAT files and outputs, and produces a list of number of
linkbacks per domain (at a subdomain granularity).

This job uses Spark to run this distributed ETL job.
"""

import time
import gzip
import argparse
from io import BytesIO
import os

import ujson as json
