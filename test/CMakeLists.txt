cmake_minimum_required(VERSION 2.8)

######################################################################################
### 1) Set your application properties
######################################################################################

# Define your project name
PROJECT(fssp)


######################################################################################
# to have an optimised code

#SET(CMAKE_CXX_FLAGS "-O3 -std=c++11")
SET(CMAKE_CXX_FLAGS "-O3")

######################################################################################
### 2) Include the sources
######################################################################################

INCLUDE_DIRECTORIES(../src)

######################################################################################
### 3) Define your target: just an executable here
#####################################################################################

ADD_EXECUTABLE(t-solution t-solution.cpp)
ADD_EXECUTABLE(t-eval t-eval.cpp)
ADD_EXECUTABLE(t-exportSVG t-exportSVG.cpp)
#ADD_EXECUTABLE(t-init t-init.cpp)


######################################################################################
### 4) Link the librairies for your executable
######################################################################################

#TARGET_LINK_LIBRARIES()



