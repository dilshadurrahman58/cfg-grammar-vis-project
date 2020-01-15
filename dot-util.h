/*
    dot-util.h

-- Utility functions for generating dot output for tree types.
-- Specifically for grammar and parse trees.  Attempting to keep all
-- dotviz assumptions in this file.
--
----------------------------------
-- Have five different node types.  The default ellipse node that the parse
-- tree is currently using,
--      -- params are node identifier and node label
--      node(string,string) ==> string
--
-- and then the four different kind of html
-- table-based nodes that the grammar dot visualization is currently
-- using (all of the below have type String -> String -> String):
--      nodeDiamond,
--      nodeWHead (so have target for a back edge),
--      nodeWFoot (for alt, which doesn't care about child ordering), and
--      nodeWLeftRight (for seq, which does care about child ordering).
--
-- Nonterminals use nodeWHead in case there is a recursive use of the
-- nonterminal.  Seq uses nodeWLeftRight.  All other grammar nodes can just
-- use nodeWFoot.
--
----------------------------------
-- Have three different edge types and functions to create the
-- edge source and target strings.
--
--      edgeSolid
--      edgeDotted
--      edgeBold
--
--      Type for all the below is int -> string
--      from
--      to
--      fromLeft
--      fromRight
--      fromFoot
--      toHead
*/

#ifndef dot_util_H_
#define dot_util_H_

#include <string>
#include <map>

namespace dot {

std::string nodeEllipse(std::string id_str, std::string label);

std::string nodeDiamond(std::string id_str, std::string label);

std::string nodeWHead(std::string id_str, std::string label);

std::string nodeWLeftRight(std::string id_str, std::string label);

std::string nodeWFoot(std::string id_str, std::string label);

std::string nodeWFootList(std::string id_str, std::string label, int num);

std::string nodeEllipseWFootList(std::string id_str,std::string label, int num);

std::string from(std::string id);

std::string to(std::string id);

std::string fromLeft(std::string id);

std::string fromRight(std::string id);

std::string fromFoot(std::string id);

std::string fromFootList(std::string id, int whichItem);

std::string toHead(std::string id);

std::string edgeString(std::string id_str, std::string label);

std::string edgeSolid(std::string id_str, std::string label);

std::string edgeDotted(std::string id_str, std::string label);

std::string edgeBold(std::string id_str, std::string label);

//std::map<std::string, std::string>NodeToDotID;




}
#endif
