History
----------------------------------------------
#### 2015-05-14: v3.0.1 by Webber Huang
  - UPDATE: Compatible with Maya 2016
    
#### 2014-10-22: v3.0.1 by Webber Huang
  - NEW: A new icon to indicate which type of deformer will be 
	created for create button 

  - FIXED: Window fail to resize properly in maya 2015

#### 2014-09-20: v3.0.0 by Webber Huang
  - NEW: Commit softSelectionQuery's source code

#### 2014-09-07: v3.0.0 by Webber Huang
  - NEW: Fully supports working with joint, create, mirror,
    redefine, export, import weight, just like play with cluster  
    
  - NEW: Convert cluster and joint into each other  
  
  - NEW: Append new weight with redefine tool  
  
  - NEW: A falloff curve panel to help user adjust soft selection  
  
  - UPDATE: Better UI  

#### 2014-09-03: v2.7.0 by Webber Huang
  - Reduce the redundant codes, improve mirror performance.  
  
  - Reduce the information contained in weight data file.  
  
  - A new method for setting environment, plugins have been
    integrated into "SoftClusterEX" module, so people don't
    need to tangle with placing the plugin files any more.  
    
  - NEW: A mel script to help people install a shelf button.  
  
  - UPDATE: softSelectionQuery plugin has been update to v1.5,
    new ways to deal with the path of the geometry vertex.  
    
  - FIXED: Failure to launch in maya 2011.  
  
  - FIXED: Malfunction occured in mirror operation.  

#### 2014-08-28: v2.6.0 by Webber Huang
  - Reconstruct source files to make it more reasonable  
  
  - Simplify weight data structure, performance improve  
  
  - NEW: Redefine cluster weight by soft selection  
  
  - NEW: Detach influenced geometries from cluster  
  
  - NEW: Export and Import cluster weight  
  
  - NEW: A flat style icon  

#### 2014-08-17: v2.1.0 by Webber Huang
  - Optimized architecture to meet fully OO design principle  
  
  - Optimized UI  
  
  - A new tab to show author's information  

#### 2014-07-25: v2.0.0 by Webber Huang
  - A complete redisign of architecture, optimized codes and algorithms  

  - Base on Maya API and optimized data structure, this version achieves a significant
    performance improvement when create cluster on massive density geometry  

  - All geometry vertices are binded to cluster, it means that no more necessary to use
    "Edit Membership Tool" to append vertices to cluster, if you want some more
    influence, just paint it  

  - A powerful mirror function has been implemented to enhance this tool, supports
    all kind of common deformable geometry, base on kd-tree algorithm, it works fast
    and reliable, so please feel free to play it with your wide imagination  

  - Notes that there may be a few vertices mis-binded(zero weight value) after perform
    mirror operation, it happens when geometry is not perfect symmetry, i can't solve
    this issue in current, please paint it when you come across this situation, i hope
    it won't be such a troublesome problem to you, this function works perfect mostly  

#### 2014-05-28: v1.1.1 by Webber Huang
  - Nothing's news, but support maya 2015 SP2 and update compile against 2014 SP4  

#### 2013-11-18: v1.1.0 by Webber Huang
  - Add Linux compile version for softSelectionQuery plugin, it works on linux now  

  - Exclude objects by group supported, user can add groups into exclude list,
    objects under these groups will be ignored while working on global mode  

#### 2013-10-27: v1.02 by Webber Huang
  - Compiled softSelectionQuery plugin for OS X, mac user can play with it  

#### 2013-10-09: v1.01 by Webber Huang
  - Issue of plugin loading failed,i re-compile all softSelectionQuery_<Version>-<Bit>.mll
    with vs2008 to maintain maximum compatibility  

#### 2013-09-30: v1.00 by Webber Huang
  - Initial release  