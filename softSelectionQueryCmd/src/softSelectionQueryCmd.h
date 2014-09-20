#ifndef _SoftSelectionQueryCmd_H
#define _SoftSelectionQueryCmd_H

/*
#----------------------------------------------------------------------
#    This file is part of "Soft Cluster EX"
#    and covered by a BSD-style license, check
#    LICENSE for detail.
#
#    Author:      Webber Huang
#    Contact:     xracz.fx@gmail.com
#    Homepage:    http://riggingtd.com
#----------------------------------------------------------------------
*/

#include <maya/MPxCommand.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MFn.h>

class SoftSelectionQueryCmd: public MPxCommand
{
public:
                                        SoftSelectionQueryCmd(){};
    virtual                             ~SoftSelectionQueryCmd();
    MStatus                             doIt( const MArgList& argList );

    

    static void*                        creator();
    static MSyntax                      cmdSyntax();

private:
    MStatus                             parseArgs(const MArgList& argList);
    MStringArray                        retrieveStringArrayFromMultiFlag(MArgDatabase& argDB, const MString& flag);
    MStringArray                        setDefaultType(const char* typeNameList[]);
    MObject                             nameToMObject(const MString& name);
    MDagPath                            getShapeDagPath(const MDagPath& dagPath);
    MDagPath                            getParentDagPath(const MDagPath& dagPath);
    void                                getBaseData(MDagPath& dagPath);
    bool                                isStrItemInArray(const MString& item, const MStringArray& strArray);
    bool                                isExcludeObject(const MString& item, const MStringArray& strArray);
    void                                getDataFromSubdiv(MDagPath& dagPath, MObject& component, bool isTransform);
    void                                getDataFromLattice(MDagPath& dagPath, MObject& component, bool isTransform);
    void                                getDataFromNurbsSurface(MDagPath& dagPath, MObject& component, bool isTransform);
    void                                getDataFromNurbsCurve(MDagPath& dagPath, MObject& component, bool isTransform);
    void                                getDataFromPolygon(MDagPath& dagPath, MObject& component, bool isTransform);

private:
    bool returnAllPath;
    bool returnLongname;
    bool returnSelection;
    bool returnTransforms;
    bool returnShapes;
    bool returnVertices;
    bool returnWeight;
    bool returnNodeType;
    bool returnAPINodeType;
    bool returnCount;

    int count;
    MStringArray transformArray;
    MStringArray shapeArray;
    MStringArray nodeTypeArray;
    MStringArray apiNodeTypeArray;
    MStringArray specifyTypeArray;
    MStringArray excludeObjectArray;
    MStringArray vertexArray;
    MDoubleArray weightArray;

};

#endif /*_SoftSelectionQueryCmd_H*/