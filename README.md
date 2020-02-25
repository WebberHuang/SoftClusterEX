
SoftClusterEX
==============

    Toolname:    Soft Cluster EX  
    Author:      Webber Huang  
    Contact:     xracz.fx@gmail.com  
    Version:     3.0.1  
    Requires:    Maya 2011 x64 ~ Maya 2017  
    Platform:    Windows 64bit, Mac OS X, Linux 64bit  
    License:     BSD license  

NOTE:
------
  **This project has been DEPRECATED and is no longer being developed.** Sorry I don't have time to support this tool anymore, user will need to compile the plugin by their own to make it run on latest maya, or rewrite the plugin with python.

Description:
----------------------------------------------
  Soft Cluster EX is a python and c++ based tool for converting soft selection to
  cluster or joint, supports common types of deformable geometry(e.g. polygon,nurbs
  ,subdiv and lattice) with a lot of advance features.  


Install:
----------------------------------------------
  1. Place SoftClusterEX folder into your maya script path.
    e.g., C:/Users/YourUsername/Documents/maya/scripts/ 

  2. Drag "install.mel" into maya scene, a new button will appear in your current shelf, 
     click it to launch. (Recommend!)  


Usage:
----------------------------------------------
If you don't like to use "install.mel" to help you install, run the following code in maya's
python tab is also okay.  

    import SoftClusterEX  
    SoftClusterEX.launch()  


Features:
----------------------------------------------
#### General functionality:  

  - supports create cluster or joint from polygon, nurbsSurface, nurbsCurve, subdiv and lattice
  - all types can be toggled on/off from UI
  - supports create cluster or joint from multi objects of different type
  - supports exclude influenced objects and groups while working on Global falloff mode
  - supports objects with duplicate name
  - ultra high performance, even working on massive density geometry
  - mirror cluster or joint in local or world space, also support working on
    multiple types of deformable geometry
  - redefine weight with soft selection
  - supports export and import weight

#### Command line:

  - The core function of this tool comes from the custom command "softSelectionQuery",
    you can use it to implement your own tools.  

  - Synopsis: softSelectionQuery [flags] [String...]
    - sl -selection                            return a list contains transforms and vertices
    - tr -transforms                           return transform names of influenced objects
    - s -shapeNames:                           return shape names of influenced objects
    - vtx -vertices:                           return vertices of influenced objects
    - ap -allPaths:                            return partial paths to objects in DAG
    - l -long:                                 return full path names for Dag objects.
    - c -count:                                return count of influenced objects
    - nt -nodeTypes:                           return type names of influenced objects
    - ant -apiNodeTypes:                       return api type name of influenced objects
    - exo -excludeObjects  String (multi-use): set exclude objects
    - t -types           String (multi-use):   set support types
    - w -weights:                              return weights of influenced components


Todo:
----------------------------------------------
  - None  


Limitation
----------------------------------------------
  - Joint function can't work with subdiv since MFnSkinCluster is incompatible with this type of geometry  
