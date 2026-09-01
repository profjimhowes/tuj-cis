#!/bin/bash
# Compile and run the container visualizer
# Requires: core.jar in this directory (from Processing)
#
# Also requires your existing Container, Stack, Queue,
# ElasticArray, and LinkedList classes in the containers package.

javac -cp core.jar containers/*.java && java -cp .:core.jar containers.ContainerVisualizer
