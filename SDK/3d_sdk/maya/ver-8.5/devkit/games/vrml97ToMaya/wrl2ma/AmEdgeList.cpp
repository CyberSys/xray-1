#ifdef NT_ENV
#include "StdAfx.h"
#endif
//
// Class:   AmEdgeList
//
/*
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc. and/or its licensors.  All 
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors, which is protected by U.S. and Canadian federal copyright 
// law and by international treaties.
//
// The Data is provided for use exclusively by You. You have the right 
// to use, modify, and incorporate this Data into other products for 
// purposes authorized by the Autodesk software license agreement, 
// without fee.
//
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the 
// following disclaimer, must be included in all copies of the 
// Software, in whole or in part, and all derivative works of 
// the Software, unless such copies or derivative works are solely 
// in the form of machine-executable object code generated by a 
// source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
// PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
// TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
// BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
// DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
// AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
// OR PROBABILITY OF SUCH DAMAGES.
//
// ==========================================================================
//+
*/


#include <string.h>
#include "AmEdgeList.h"

#define kFALSE false
#define kTRUE true

AmEdgeList::AmEdgeList( )
:    fHeadNode  ( NULL ),
	fCurrentNode( NULL ),
	fLength     ( 0 )
{
}

AmEdgeList::~AmEdgeList( )
//
// Description:
//      This function traverses the edge list and deletes
//		all the edge nodes.
// 
{
	AmEdge *workingNode = NULL;
	AmEdge *nextNode    = NULL;
	
	workingNode = fHeadNode;
	fHeadNode = NULL;

	// Traverse the list to delete the nodes.
	//
	if( NULL != workingNode )
	{
		do 
		{
			nextNode = workingNode->nextEdge( );
			delete workingNode;
			workingNode = nextNode;
		} while( NULL != workingNode );
	}
}

void AmEdgeList::addEdge( int edgeId,
						  int startIndex,
						  int endVertex,
						  double *startNormal,
						  double *endNormal )
//
// Description:
//      This function builds up the edge list.
// 
// Arguments:
//      edgeId      - index to the edge
//      startIndex  - index of the start vertex
//      endIndex    - index of the end vertex
// 
{
	AmEdge *nextNode = new AmEdge;

	nextNode->setEdge( edgeId, startIndex, endVertex, startNormal, endNormal );
	nextNode->setNextEdge( NULL );

	if( 0 == fLength )
	{
		fHeadNode = nextNode;
		fCurrentNode = nextNode;
		fLength++;
	}
	else
	{
		fCurrentNode->setNextEdge( nextNode );
		fCurrentNode = nextNode;
		fLength++;
	}
}

void AmEdgeList::hardenEdge( int edgeId )
//
// Description:
//      This function ...
//
// Arguments:
//		
{
	AmEdge *workingNode = fHeadNode;

	// Traverse the list.
	//
	if( NULL != workingNode )
	{
		do 
		{

			if( edgeId == workingNode->edgeId() )
			{
				workingNode->hardenEdge();
				return;
			}
			workingNode = workingNode->nextEdge( );
		} while( NULL != workingNode );
	}
}

int AmEdgeList::findEdge( int startIndex,
						  int endIndex,
						  double *startNormal,
						  double *endNormal,
						  int &signEdge,
						  bool &hardEdge )
//
// Description:
//      This function finds the edge starting/ending
//      with vertex of startIndex and ending/starting
//      with vertex of endIndex. If the edge is found,
//      the edgeId is returned, otherwise -1 is returned.
//
// Arguments:
//		startIndex   - index of the start vertex
//      endIndex     - index of the end vertex
//      signEdge     - indicating the direction of the edge
//
{
	AmEdge *workingNode = fHeadNode;
	int i = 0;
	
	// Traverse the list.
	//
	if( NULL != workingNode )
	{
		do 
		{
			if( (startIndex == workingNode->startIndex()) &&
				(endIndex == workingNode->endIndex()) )
			{
				signEdge = 1;
				hardEdge = kFALSE;

				if( ( NULL != startNormal ) &&
					( NULL != endNormal ) )
				{
					double *wStartNormal = workingNode->startNormal();
					double *wEndNormal = workingNode->endNormal();
					for( i = 0; i < 3; i++ )
					{
						if( startNormal[i] != wStartNormal[i] )
						{
							hardEdge = kTRUE;
							break;
						}
						if( endNormal[i] != wEndNormal[i] )
						{
							hardEdge = kTRUE;
							break;
						}
					}
				}
				return workingNode->edgeId();
			}
			else if( (startIndex == workingNode->endIndex()) &&
					 (endIndex == workingNode->startIndex()) )
			{
				signEdge = -1;
				hardEdge = kFALSE;

				if( ( NULL != startNormal ) &&
					( NULL != endNormal ) )
				{
					double *wStartNormal = workingNode->startNormal();
					double *wEndNormal = workingNode->endNormal();
					for( i = 0; i < 3; i++ )
					{
						if( startNormal[i] != wEndNormal[i] )
						{
							hardEdge = kTRUE;
							break;
						}
						if( endNormal[i] != wStartNormal[i] )
						{
							hardEdge = kTRUE;
							break;
						}
					}
				}
				return workingNode->edgeId();
			}
			workingNode = workingNode->nextEdge( );
		} while( NULL != workingNode );
	}

	return -1;
}

void AmEdgeList::printEdges( )
//
// Description:
//      This function prints the edges in the list.
// 
{
	AmEdge *workingNode = fHeadNode;

	int counter = 1;
	// Traverse the list.
	//
	if( NULL != workingNode )
	{
		do 
		{
			workingNode->printEdge( counter );
			workingNode = workingNode->nextEdge( );
			counter++;
		} while( NULL != workingNode );
	}
}
