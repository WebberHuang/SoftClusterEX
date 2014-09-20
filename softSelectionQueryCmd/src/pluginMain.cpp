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
#include <maya/MFnPlugin.h>

#define __AUTHOR__       "Webber Huang"
#define __VERSION__      "1.5"

MStatus initializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj, __AUTHOR__, __VERSION__, "Any");

    status = plugin.registerCommand( "softSelectionQuery", SoftSelectionQueryCmd::creator, SoftSelectionQueryCmd::cmdSyntax);
    if (!status) {
        status.perror("registerCommand");
        return status;
    }
    return status;
}

MStatus uninitializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterCommand( "softSelectionQuery" );
    if (!status) {
        status.perror("deregisterCommand");
    }
    return status;
}
