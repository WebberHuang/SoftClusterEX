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
#include "version.h"
#include <maya/MFnPlugin.h>

#define _AUTHOR       "Webber Huang <xracz.fx@gmail.com>"

MStatus initializePlugin( MObject obj )
{
    MStatus status;

    char ver[32];
    sprintf (ver, "%d.%d.%d", MAYA_PLUGIN_MAJOR, MAYA_PLUGIN_MINOR, MAYA_PLUGIN_PATCH);

    char verInfo[512];
    sprintf (verInfo, "%s built on %s %s", ver, __TIME__, __DATE__);
    MFnPlugin plugin( obj, _AUTHOR, verInfo, "Any" );

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
