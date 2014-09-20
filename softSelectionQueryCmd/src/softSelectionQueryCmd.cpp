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

#include "softSelectionQueryCmd.h"
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFn.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MRichSelection.h>
#include <maya/MSelectionList.h>
#include <maya/MStatus.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MItSelectionList.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItCurveCV.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MItSubdVertex.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnDoubleIndexedComponent.h>
#include <maya/MFnTripleIndexedComponent.h>
#include <maya/MFnUint64SingleIndexedComponent.h>
#include <math.h>

#define cAllPathFlag                      "-ap"
#define cAllPathFlagLong                  "-allPaths"

#define cLongnameFlag                     "-l"
#define cLongnameFlagLong                 "-long"

#define cSelectionFlag                    "-sl"
#define cSelectionFlagLong                "-selection"

#define cTransformsFlag                   "-tr"
#define cTransformsFlagLong               "-transforms"

#define cShapesFlag                       "-s"
#define cShapesFlagLong                   "-shapes"

#define cVerticesFlag                     "-vtx"
#define cVerticesFlagLong                 "-vertices"

#define cWeightFlag                       "-w"
#define cWeightFlagLong                   "-weights"

#define cCountFlag                        "-c"
#define cCountFlagLong                    "-count"
#define cTypeFlag                         "-t"
#define cTypeFlagLong                     "-types"
#define cNodeTypeFlag                     "-nt"
#define cNodeTypeFlagLong                 "-nodeTypes"
#define cAPINodeTypeFlag                  "-ant"
#define cAPINodeTypeFlagLong              "-apiNodeTypes"
#define cExcludeObjectFlag                "-exo"
#define cExcludeObjectFlagLong            "-excludeObjects"

static const char* SUPPORT_TYPE[] = {"mesh", "nurbsSurface", "nurbsCurve", "lattice", "subdiv"};

SoftSelectionQueryCmd::~SoftSelectionQueryCmd(){}

void* SoftSelectionQueryCmd::creator()
{
    return new SoftSelectionQueryCmd;
}

MSyntax SoftSelectionQueryCmd::cmdSyntax()
{
    MSyntax syntax;
    syntax.addFlag(cAllPathFlag, cAllPathFlagLong);
    syntax.addFlag(cLongnameFlag, cLongnameFlagLong);
    syntax.addFlag(cSelectionFlag, cSelectionFlagLong);
    syntax.addFlag(cTransformsFlag, cTransformsFlagLong);
    syntax.addFlag(cShapesFlag, cShapesFlagLong);
    syntax.addFlag(cVerticesFlag, cVerticesFlagLong);
    syntax.addFlag(cWeightFlag, cWeightFlagLong);
    syntax.addFlag(cCountFlag, cCountFlagLong);
    syntax.addFlag(cTypeFlag, cTypeFlagLong, MSyntax::kString);
    syntax.addFlag(cNodeTypeFlag, cNodeTypeFlagLong);
    syntax.addFlag(cAPINodeTypeFlag, cAPINodeTypeFlagLong);
    syntax.addFlag(cExcludeObjectFlag, cExcludeObjectFlagLong, MSyntax::kString);

    syntax.makeFlagMultiUse(cExcludeObjectFlag);
    syntax.makeFlagMultiUse(cTypeFlag);

    syntax.setObjectType(MSyntax::kSelectionList);
    syntax.useSelectionAsDefault(true);

    return syntax;
}

MStatus SoftSelectionQueryCmd::parseArgs(const MArgList& argList)
{
    // parse syntax
    MStatus status;
    MArgDatabase argDB(syntax(), argList, &status);
    
    returnAllPath = argDB.isFlagSet(cAllPathFlag);
    returnLongname = argDB.isFlagSet(cLongnameFlag);
    returnSelection = argDB.isFlagSet(cSelectionFlag);
    returnTransforms = argDB.isFlagSet(cTransformsFlag);
    returnShapes = argDB.isFlagSet(cShapesFlag);
    returnVertices = argDB.isFlagSet(cVerticesFlag);
    returnWeight = argDB.isFlagSet(cWeightFlag);
    returnNodeType = argDB.isFlagSet(cNodeTypeFlag);
    returnAPINodeType = argDB.isFlagSet(cAPINodeTypeFlag);
    returnCount = argDB.isFlagSet(cCountFlag);

    if (argDB.isFlagSet(cTypeFlag))
    {
        specifyTypeArray = retrieveStringArrayFromMultiFlag(argDB, cTypeFlag);
    }
    else
    {
        const size_t len = sizeof(SUPPORT_TYPE) / sizeof(SUPPORT_TYPE[0]);
        for(int i=0; i < len; i++)
        {
            specifyTypeArray.append(SUPPORT_TYPE[i]);
        }
    }

    excludeObjectArray = retrieveStringArrayFromMultiFlag(argDB, cExcludeObjectFlag);

    return status;
}

MStatus SoftSelectionQueryCmd::doIt( const MArgList& argList )
{
    // parse syntax
    MStatus status = parseArgs(argList);

    if (status != MS::kSuccess)
    {
        return MS::kFailure;
    }

    // 
    MSelectionList selection;
    MRichSelection softSelection;
    MGlobal::getRichSelection(softSelection);
    softSelection.getSelection(selection);

    // 
    MDagPath dagPath;
    MObject component;

    MItSelectionList iter(selection);
    count = 0;
    while(!iter.isDone())
    {        
        iter.getDagPath(dagPath, component);

        bool isTransform = false;
        if (dagPath.hasFn(MFn::kTransform))
        {
            dagPath = getShapeDagPath(dagPath);
            isTransform = true;
        }

        MFnDagNode dagNodeFn(dagPath);

        // get parent dagPath
        MDagPath parentDagPath = getParentDagPath(dagPath);
        MFnDagNode parentDagNodeFn(parentDagPath);

        // skip the un-support type
        if (!isStrItemInArray(dagNodeFn.typeName(), specifyTypeArray))
        {
            iter.next();
            continue;
        }

        // skip the exclusive objects
        if (isExcludeObject(parentDagNodeFn.fullPathName(), excludeObjectArray))
        {
            iter.next();
            continue;
        }

        // store result
        getBaseData(dagPath);

        switch (dagPath.apiType())
        {
            case MFn::kMesh: 
            {
                getDataFromPolygon(dagPath, component, isTransform);
                break;
            }
            case MFn::kNurbsCurve: 
            {
                getDataFromNurbsCurve(dagPath, component, isTransform);
                break;
            }
            case MFn::kNurbsSurface: 
            {
                getDataFromNurbsSurface(dagPath, component, isTransform); 
                break;
            }
            case MFn::kSubdiv: 
            {
                getDataFromSubdiv(dagPath, component, isTransform);
                break;
            }
            case MFn::kLattice: 
            {
                getDataFromLattice(dagPath, component, isTransform); 
                break;
            }
            default:
                break;
        }

        iter.next();
    }
    
    // set results
    if (returnSelection)
    {
        setResult(transformArray);
        appendToResult(vertexArray);
    }

    if (returnTransforms)
    {
        setResult(transformArray);
    }

    if (returnShapes)
    {
        setResult(shapeArray);
    }

    if (returnNodeType)
    {
        setResult(nodeTypeArray);
    }

    if (returnAPINodeType)
    {
        setResult(apiNodeTypeArray);
    }

    if (returnCount)
    {
        setResult(count);
    }

    if (returnVertices)
    {
        setResult(vertexArray);
    }

    if (returnWeight)
    {
        setResult(weightArray);
    }

    return MStatus::kSuccess;
}

MObject SoftSelectionQueryCmd::nameToMObject(const MString& name)
{
    MSelectionList selList;
    selList.add (name);
    MObject node;
    selList.getDependNode(0, node);
    return node;
}

MStringArray SoftSelectionQueryCmd::retrieveStringArrayFromMultiFlag(MArgDatabase& argDB, const MString& flag)
{
    MStatus status;
    MArgList argList;
    MStringArray strArray;
    int index = 0;

    while (true)
    {
        MString itemName;
        status = argDB.getFlagArgumentList( flag.asChar(), index, argList );
        if (status == MStatus::kSuccess)
        {
            itemName = argList.asString(index);
            strArray.append(itemName);
        }
        else 
        {
            break;
        }
        index++;
    }
    return strArray;
}

MDagPath SoftSelectionQueryCmd::getShapeDagPath(const MDagPath& dagPath)
{
    MObject childObj = dagPath.child(0);
    MFnDagNode dagNodeFn(childObj);
    MDagPath shapeDagPath;
    dagNodeFn.getPath(shapeDagPath);
    return shapeDagPath;
}

MDagPath SoftSelectionQueryCmd::getParentDagPath(const MDagPath& dagPath)
{
    MObject transformObj = dagPath.transform();
    MFnDagNode parentDagNodeFn(transformObj);
    MDagPath parentDagPath;
    parentDagNodeFn.getPath(parentDagPath);
    return parentDagPath;
}

bool SoftSelectionQueryCmd::isStrItemInArray(const MString& item, const MStringArray& strArray)
{
    for(unsigned int i=0; i < strArray.length(); i++)
    {
        if (item == strArray[i])
        {
            return true;
        }
    }
    return false;
}

bool SoftSelectionQueryCmd::isExcludeObject(const MString& item, const MStringArray& strArray)
{
    MObject itemObj = nameToMObject(item);
    MFnDagNode dagFn(itemObj);

    for(unsigned int i = 0; i < strArray.length(); i++)
    {
        MObject currObj = nameToMObject(strArray[i]);
        if (itemObj == currObj || dagFn.isChildOf(currObj))
        {
            return true;
        }
    }
    return false;
}

void SoftSelectionQueryCmd::getBaseData(MDagPath& dagPath)
{
    MFnDagNode dagNodeFn(dagPath);
    MObject obj = dagPath.node();

    // get parent dagPath
    MDagPath parentDagPath = getParentDagPath(dagPath);
    MFnDagNode parentDagNodeFn(parentDagPath);

    // set data
    if (returnLongname)
    {
        transformArray.append(parentDagNodeFn.fullPathName());
        shapeArray.append(dagNodeFn.fullPathName());
    }
    else if (returnAllPath)
    {
        transformArray.append(parentDagNodeFn.partialPathName());
        shapeArray.append(dagNodeFn.partialPathName());
    }
    else
    {
        transformArray.append(parentDagNodeFn.name());
        shapeArray.append(dagNodeFn.name());
    }
    nodeTypeArray.append(dagNodeFn.typeName());
    apiNodeTypeArray.append(obj.apiTypeStr());
    count++;
}

void SoftSelectionQueryCmd::getDataFromLattice(MDagPath& dagPath, MObject& component, bool isTransform=false)
{
    int s, t, u;
    MFnTripleIndexedComponent tripleCompFn(component);
    MDagPath parentDagPath = getParentDagPath(dagPath);
    MFnDagNode parentDagNodeFn(parentDagPath);

    if (isTransform)
    {
        MItGeometry geoIter(dagPath);
        while (!geoIter.isDone())
        {
            MObject currItem = geoIter.currentItem();
            MFnTripleIndexedComponent tripleCompFn(currItem);
            tripleCompFn.getElement(0, s, t, u);
            weightArray.append(1.0);

            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".pt[") + s + MString("][") + t + MString("][") + u + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".pt[") + s + MString("][") + t + MString("][") + u + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".pt[") + s + MString("][") + t + MString("][") + u + MString("]"));
            geoIter.next();
        }
    }
    else
    {
        for(int i = 0; i < tripleCompFn.elementCount(); i++)
        {
            tripleCompFn.getElement(i, s, t, u);
            weightArray.append(tripleCompFn.weight(i).influence());
            
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".pt[") + s + MString("][") + t + MString("][") + u + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".pt[") + s + MString("][") + t + MString("][") + u + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".pt[") + s + MString("][") + t + MString("][") + u + MString("]"));
        }
    }
}

void SoftSelectionQueryCmd::getDataFromSubdiv(MDagPath& dagPath, MObject& component, bool isTransform=false)
{
    int u, v;
    MFnUint64SingleIndexedComponent uSingleCompFn(component);
    MDagPath parentDagPath = getParentDagPath(dagPath);
    MFnDagNode parentDagNodeFn(parentDagPath);
    
    if (isTransform)
    {
        MItGeometry geoIter(dagPath);
        while (!geoIter.isDone())
        {
            MObject currItem = geoIter.currentItem();
            MFnUint64SingleIndexedComponent uSingleCompFn(currItem);
            MUint64 element = uSingleCompFn.element(0);
            u = element / pow((double)2,32);
            v = (int)element;
            weightArray.append(1.0);
            
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".smp[") + u + MString("][") + v + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".smp[") + u + MString("][") + v + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".smp[") + u + MString("][") + v + MString("]"));
            geoIter.next();
        }
    }
    else
    {
        for(int i = 0; i < uSingleCompFn.elementCount(); i++)
        {
            MUint64 element = uSingleCompFn.element(i);
            u = element / pow((double)2,32);
            v = (int)element;
            weightArray.append(uSingleCompFn.weight(i).influence());
            
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".smp[") + u + MString("][") + v + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".smp[") + u + MString("][") + v + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".smp[") + u + MString("][") + v + MString("]"));
        }
    }    
}

void SoftSelectionQueryCmd::getDataFromNurbsSurface(MDagPath& dagPath, MObject& component, bool isTransform=false)
{
    int u, v;
    MFnDoubleIndexedComponent doubleCompFn(component);
    MDagPath parentDagPath = getParentDagPath(dagPath);
    MFnDagNode parentDagNodeFn(parentDagPath);

    //
    if (isTransform)
    {
        MItGeometry geoIter(dagPath);
        while (!geoIter.isDone())
        {
            MObject currItem = geoIter.currentItem();
            MFnDoubleIndexedComponent doubleCompFn(currItem);
            doubleCompFn.getElement(0, u, v);
            weightArray.append(1.0);
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".cv[") + u + MString("][") + v + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".cv[") + u + MString("][") + v + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".cv[") + u + MString("][") + v + MString("]"));
                
            geoIter.next();
        }
    }
    else
    {
        for(int i = 0; i < doubleCompFn.elementCount(); i++)
        {
            doubleCompFn.getElement(i, u, v);
            weightArray.append(doubleCompFn.weight(i).influence());
            
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".cv[") + u + MString("][") + v + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".cv[") + u + MString("][") + v + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".cv[") + u + MString("][") + v + MString("]"));
        }
    }
}

void SoftSelectionQueryCmd::getDataFromNurbsCurve(MDagPath& dagPath, MObject& component, bool isTransform=false)
{
    int index;
    MFnSingleIndexedComponent singleCompFn(component);
    MDagPath parentDagPath = getParentDagPath(dagPath);
    MFnDagNode parentDagNodeFn(parentDagPath);

    if (isTransform)
    {
        MItGeometry geoIter(dagPath);
        while (!geoIter.isDone())
        {
            MObject currItem = geoIter.currentItem();
            MFnSingleIndexedComponent singleCompFn(currItem);
            index = singleCompFn.element(0);
            weightArray.append(1.0);
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".cv[") + index + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".cv[") + index + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".cv[") + index + MString("]"));
            geoIter.next();
        }
    }
    else
    {
        for(int i = 0; i < singleCompFn.elementCount(); i++)
        {
            index = singleCompFn.element(i);
            weightArray.append(singleCompFn.weight(i).influence());
            
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".cv[") + index + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".cv[") + index + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".cv[") + index + MString("]"));
        }
    }
}

void SoftSelectionQueryCmd::getDataFromPolygon(MDagPath& dagPath, MObject& component, bool isTransform=false)
{
    int index;
    MFnSingleIndexedComponent singleCompFn(component);
    MDagPath parentDagPath = getParentDagPath(dagPath);
    MFnDagNode parentDagNodeFn(parentDagPath);

    if (isTransform)
    {
        MItGeometry geoIter(dagPath);
        while (!geoIter.isDone())
        {
            MObject currItem = geoIter.currentItem();
            MFnSingleIndexedComponent singleCompFn(currItem);
            index = singleCompFn.element(0);
            weightArray.append(1.0);
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".vtx[") + index + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".vtx[") + index + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".vtx[") + index + MString("]"));
            
            geoIter.next();
        }
    }
    else
    {
        for(int i = 0; i < singleCompFn.elementCount(); i++)
        {
            index = singleCompFn.element(i);
            weightArray.append(singleCompFn.weight(i).influence());
            if (returnAllPath)
            {
                vertexArray.append(parentDagNodeFn.partialPathName() + MString(".vtx[") + index + MString("]"));
            }
            else if (returnLongname)
            {
                vertexArray.append(parentDagNodeFn.fullPathName() + MString(".vtx[") + index + MString("]"));
            }
            else vertexArray.append(parentDagNodeFn.name() + MString(".vtx[") + index + MString("]"));
        }
    }
}
